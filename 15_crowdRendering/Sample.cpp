#include <Sample.h>
#include <fstream>

bool Sample::Initialize(const char* title, const int width, const int height)
{
    if (!Application::Initialize(title, width, height)) {
        return false;
    }
    
    cgltf_data* gltf = LoadGLTFFile("Assets/Woman.gltf");
    meshes = LoadMeshes(gltf);
    skeleton = LoadSkeleton(gltf);
    clips = LoadAnimationClips(gltf);
    FreeGLTFFile(gltf);
    
    crowdShader = new Shader("Shaders/crowd.vert", "Shaders/lit.frag");
    diffuseTexture = new Texture("Assets/Woman.png");
    
    unsigned int numClips = (unsigned int)clips.size();
    textures.resize(numClips);
    crowds.resize(numClips);
    for (unsigned int i = 0; i < numClips; ++i)
    {
        std::string fileName = "Assets/";
        fileName += clips[i].GetName();
        fileName += ".animTex";
        bool fileExists = true;
        {
            std::ifstream testFile(fileName);
            if (!testFile.is_open()) {
                fileExists = false;
            } else {
                testFile.close();
            }
        }
        if (fileExists) {
            textures[i].Load(fileName.c_str());
        } else {
            textures[i].Resize(512);
            BakeAnimationTexture(skeleton, clips[i], textures[i]);
            textures[i].Save(fileName.c_str());
        }
    }
    
    SetCrowdSize(20);

    return true;
}

void Sample::SetCrowdSize(unsigned int size)
{
    std::vector<Vec3> occupied;
    unsigned int numCrowds = (unsigned int)crowds.size();
    for (unsigned int i = 0; i < numCrowds; ++i)
    {
        crowds[i].Resize(20);
        crowds[i].RandomizeTimes(clips[i]);
        crowds[i].RandomizePositions(occupied, Vec3(-40, 0, -80.0f), Vec3(40, 0, 30.0f), 2.0f);
    }
}

void Sample::Update(float inDeltaTime)
{
    Application::Update(inDeltaTime);

    unsigned int numCrowds = (unsigned int)crowds.size();
    for (unsigned int i = 0; i < numCrowds; ++i) {
        crowds[i].Update(inDeltaTime, clips[i], textures[i].GetSize());
    }
}

void Sample::Render(float inAspectRatio)
{
    Mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
    Mat4 view = lookAt(Vec3(0,15,40), Vec3(0,3,0), Vec3(0,1,0));
    Mat4 mvp = projection * view; // no model matrix
    
    crowdShader->Bind();
    Uniform<Mat4>::Set(crowdShader->GetUniform("view"), view);
    Uniform<Mat4>::Set(crowdShader->GetUniform("projection"), projection);
    Uniform<Vec3>::Set(crowdShader->GetUniform("light"), Vec3(1,1,1));
    Uniform<Mat4>::Set(crowdShader->GetUniform("invBindPose"), skeleton.GetInvBindPose());
    diffuseTexture->Set(crowdShader->GetUniform("tex0"), 0);
    
    unsigned int numCrowds = (unsigned int)crowds.size();
    for (unsigned int c = 0; c < numCrowds; ++c)
    {
        textures[c].Bind(crowdShader->GetUniform("animTex"), 1); // GL_TEXTURE1
        crowds[c].SetUniforms(crowdShader);
        for (unsigned int i = 0, size = (unsigned int)meshes.size(); i < size; ++i)
        {
            meshes[i].Bind(crowdShader->GetAttribute("position"),
                           crowdShader->GetAttribute("normal"),
                           crowdShader->GetAttribute("texCoord"),
                           crowdShader->GetAttribute("weights"),
                           crowdShader->GetAttribute("joints"));
            meshes[i].DrawInstanced(crowds[c].GetSize());
            meshes[i].Unbind(crowdShader->GetAttribute("position"),
                             crowdShader->GetAttribute("normal"),
                             crowdShader->GetAttribute("texCoord"),
                             crowdShader->GetAttribute("weights"),
                             crowdShader->GetAttribute("joints"));
        }
        textures[c].Unbind(1);
    }
    diffuseTexture->Unset(0);
    crowdShader->UnBind();
}

void Sample::Shutdown()
{
    Application::Shutdown();
}

