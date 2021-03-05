#include <Sample.h>
#include <vector>
#include <cstdarg>
    

bool Sample::Initialize(const char* title, const int width, const int height)
{
    if (!Application::Initialize(title, width, height)) {
        return false;
    }

    cgltf_data* gltf = LoadGLTFFile("assets/Woman.gltf");

    CPUMeshes = LoadMeshes(gltf);
    skeleton = LoadSkeleton(gltf);
    clips = LoadAnimationClips(gltf);
    FreeGLTFFile(gltf);
    
    GPUMeshes = CPUMeshes;
    for (unsigned int i = 0, size = (unsigned int)GPUMeshes.size(); i < size; ++i) {
        GPUMeshes[i].UpdateOpenGLBuffers();
    }
    
    staticShader = new Shader("shaders/static.vert", "shaders/lit.frag");
    skinnedShader = new Shader("shaders/skinned.vert", "shaders/lit.frag");
    diffuseTexture = new Texture("assets/Woman.png");
    
    GPUAnimInfo.animatedPose = skeleton.GetRestPose();
    GPUAnimInfo.posePalette.resize(skeleton.GetRestPose().GetSize());
    CPUAnimInfo.animatedPose = skeleton.GetRestPose();
    CPUAnimInfo.posePalette.resize(skeleton.GetRestPose().GetSize());

    GPUAnimInfo.model.position = Vec3(-2, 0, 0);
    CPUAnimInfo.model.position = Vec3(2, 0, 0);
    
    unsigned int numUIClips = clips.size();
    for (unsigned int i = 0; i < numUIClips; ++i) {
        if (clips[i].GetName() == "Walking") {
            std::cout << "Found walking clip" << std::endl;
            CPUAnimInfo.clip = i;
            break;
        } else if (clips[i].GetName() == "Running") {
            std::cout << "Found running clip" << std::endl;
            GPUAnimInfo.clip = i;
        }
    }

    return true;
}

void Sample::Update(float inDeltaTime)
{
    Application::Update(inDeltaTime);

    CPUAnimInfo.playback = clips[CPUAnimInfo.clip].Sample(CPUAnimInfo.animatedPose, CPUAnimInfo.playback + inDeltaTime);
    GPUAnimInfo.playback = clips[GPUAnimInfo.clip].Sample(GPUAnimInfo.animatedPose, GPUAnimInfo.playback + inDeltaTime);
    
    // pre-compute the skin matrices
    CPUAnimInfo.animatedPose.GetMatrixPalette(CPUAnimInfo.posePalette);
    std::vector<Mat4>& invBindPose = skeleton.GetInvBindPose();
    for (int i = 0; i < CPUAnimInfo.posePalette.size(); ++i) {
        CPUAnimInfo.posePalette[i] = CPUAnimInfo.posePalette[i] * invBindPose[i];
    }
    // apply to each mesh
    for (unsigned int i = 0, size = (unsigned int)CPUMeshes.size(); i < size; ++i) {
        CPUMeshes[i].CPUSkin(CPUAnimInfo.posePalette);
    }
    
 
    // pre-compute skin matrix
    GPUAnimInfo.animatedPose.GetMatrixPalette(GPUAnimInfo.posePalette);
    //std::vector<Mat4>& invBindPose = skeleton.GetInvBindPose();
    for (int i = 0; i < GPUAnimInfo.posePalette.size(); ++i) {
        GPUAnimInfo.posePalette[i] = GPUAnimInfo.posePalette[i] * invBindPose[i];
    }
}

void Sample::Render(float inAspectRatio)
{
    Mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
    Mat4 view = lookAt(Vec3(0, 5, 7), Vec3(0, 3, 0), Vec3(0, 1, 0));
    Mat4 model;
    
    // CPU skinned mesh
    model = transformToMat4(CPUAnimInfo.model);
    staticShader->Bind();
    Uniform<Mat4>::Set(staticShader->GetUniform("model"), model);
    Uniform<Mat4>::Set(staticShader->GetUniform("view"), view);
    Uniform<Mat4>::Set(staticShader->GetUniform("projection"), projection);
    Uniform<Vec3>::Set(staticShader->GetUniform("light"), Vec3(1,1,1));
    diffuseTexture->Set(staticShader->GetUniform("tex0"), 0);
    
    // cpu meshing so weight and influences are not sent to the GPU
    for (unsigned int i = 0, size = (unsigned int)CPUMeshes.size(); i < size; ++i) {
        CPUMeshes[i].Bind(staticShader->GetAttribute("position"),
                          staticShader->GetAttribute("normal"),
                          staticShader->GetAttribute("texCoord"),
                          -1, 
                          -1);
        CPUMeshes[i].Draw();
        CPUMeshes[i].Unbind(staticShader->GetAttribute("position"),
                            staticShader->GetAttribute("normal"),
                            staticShader->GetAttribute("texCoord"),
                            -1, 
                            -1);
    }
    diffuseTexture->Unset(0);
    staticShader->UnBind();
    
    // GPU skinned mesh
    model = transformToMat4(GPUAnimInfo.model);
    skinnedShader->Bind();
    Uniform<Mat4>::Set(skinnedShader->GetUniform("model"), model);
    Uniform<Mat4>::Set(skinnedShader->GetUniform("view"), view);
    Uniform<Mat4>::Set(skinnedShader->GetUniform("projection"), projection);
    Uniform<Vec3>::Set(skinnedShader->GetUniform("light"), Vec3(1,1,1));
    
    // optimized from CPU pre-calc above to use a single array instead
    //Uniform<Mat4>::Set(skinnedShader->GetUniform("pose"), GPUAnimInfo.posePalette);
    //Uniform<Mat4>::Set(skinnedShader->GetUniform("invBindPose"), skeleton.GetInvBindPose());
    Uniform<Mat4>::Set(skinnedShader->GetUniform("skinMatrix"), GPUAnimInfo.posePalette);
    
    diffuseTexture->Set(skinnedShader->GetUniform("tex0"), 0);
    
    for (unsigned int i = 0, size = (unsigned int)GPUMeshes.size(); i < size; ++i) {
        GPUMeshes[i].Bind(skinnedShader->GetAttribute("position"),
                          skinnedShader->GetAttribute("normal"),
                          skinnedShader->GetAttribute("texCoord"),
                          skinnedShader->GetAttribute("weights"), 
                          skinnedShader->GetAttribute("joints"));
        GPUMeshes[i].Draw();
        GPUMeshes[i].Unbind(skinnedShader->GetAttribute("position"),
                            skinnedShader->GetAttribute("normal"),
                            skinnedShader->GetAttribute("texCoord"),
                            skinnedShader->GetAttribute("weights"), 
                            skinnedShader->GetAttribute("joints"));
    }
    diffuseTexture->Unset(0);
    skinnedShader->UnBind();
}

void Sample::Shutdown()
{
    delete staticShader;
    delete skinnedShader;
    delete diffuseTexture;
    clips.clear();
    CPUMeshes.clear();
    GPUMeshes.clear();

    Application::Shutdown();
}

