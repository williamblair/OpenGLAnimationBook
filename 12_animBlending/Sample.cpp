#include <Sample.h>
#include <vector>
#include <cstdarg>
    

bool Sample::Initialize(const char* title, const int width, const int height)
{
    if (!Application::Initialize(title, width, height)) {
        return false;
    }

    cgltf_data* gltf = LoadGLTFFile("Assets/Woman.gltf");
    skeleton = LoadSkeleton(gltf);
    clips = LoadAnimationClips(gltf);
    meshes = LoadMeshes(gltf);
    FreeGLTFFile(gltf);
    
    pose = skeleton.GetRestPose();
    pose.GetMatrixPalette(posePalette);
    skinPalette = skeleton.GetInvBindPose();
    
    shader = new Shader("Shaders/skinned.vert", "Shaders/lit.frag");
    texture = new Texture("Assets/Woman.png");
    
    animA.pose = pose;
    animB.pose = pose;
    
    blendTime = 0.0f;
    invertBlend = false;
    
    animA.clip = 0;
    animB.clip = 1;

    for (unsigned int i = 0, size = (unsigned int)clips.size(); 
         i < size; ++i) {
        if (clips[i].GetName() == "Walking") {
            animA.clip = i;
            animA.time = clips[i].GetStartTime();
        }
        else if (clips[i].GetName() == "Running") {
            animB.clip = i;
            animB.time = clips[i].GetStartTime();
        }
    }

    return true;
}

void Sample::Update(float inDeltaTime)
{
    Application::Update(inDeltaTime);

    animA.time = clips[animA.clip].Sample(animA.pose, animA.time + inDeltaTime);
    animB.time = clips[animB.clip].Sample(animB.pose, animB.time + inDeltaTime);
    
    float bt = blendTime;
    if (bt < 0.0f) bt = 0.0f;
    if (bt > 1.0f) bt = 1.0f;
    if (invertBlend) bt = 1.0f - bt;
    Blend(pose, animA.pose, animB.pose, bt, -1);
    pose.GetMatrixPalette(posePalette);
    
    blendTime += inDeltaTime;
    if (blendTime >= 2.0f) {
        blendTime = 0.0f;
        invertBlend = !invertBlend;
        pose = skeleton.GetRestPose();
    }
}

void Sample::Render(float inAspectRatio)
{
    Mat4 model;
    Mat4 view = lookAt(Vec3(0,3,5), Vec3(0,3,0), Vec3(0,1,0));
    Mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
    
    shader->Bind();
    Uniform<Mat4>::Set(shader->GetUniform("model"), model);
    Uniform<Mat4>::Set(shader->GetUniform("view"), view);
    Uniform<Mat4>::Set(shader->GetUniform("projection"), projection);
    Uniform<Mat4>::Set(shader->GetUniform("pose"), posePalette);
    Uniform<Mat4>::Set(shader->GetUniform("invBindPose"), skinPalette);
    Uniform<Vec3>::Set(shader->GetUniform("light"), Vec3(1,1,1));
    texture->Set(shader->GetUniform("tex0"), 0);
    for (unsigned int i = 0, size = (unsigned int)meshes.size();
        i < size; i++)
    {
        meshes[i].Bind(shader->GetAttribute("position"), 
                    shader->GetAttribute("normal"), 
                    shader->GetAttribute("texCoord"), 
                    shader->GetAttribute("weights"), 
                    shader->GetAttribute("joints"));
        meshes[i].Draw();
        meshes[i].Unbind(shader->GetAttribute("position"), 
                    shader->GetAttribute("normal"), 
                    shader->GetAttribute("texCoord"), 
                    shader->GetAttribute("weights"), 
                    shader->GetAttribute("joints"));
    }
    texture->Unset(0);
    shader->UnBind();
}

void Sample::Shutdown()
{
    clips.clear();
    meshes.clear();
    delete shader;
    delete texture;

    Application::Shutdown();
}

