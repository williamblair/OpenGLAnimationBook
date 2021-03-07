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
    
    shader = new Shader("Shaders/skinned.vert", "Shaders/lit.frag");
    texture = new Texture("Assets/Woman.png");
    
#ifdef SAMPLE1
    pose = skeleton.GetRestPose();
    pose.GetMatrixPalette(posePalette);
    skinPalette = skeleton.GetInvBindPose();

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
#endif

#ifdef SAMPLE2
    fadeController.SetSkeleton(skeleton);
    fadeController.Play(&clips[0]);
    fadeController.Update(0.0f);
    fadeController.GetCurrentPose().GetMatrixPalette(posePalette);
    
    fadeTimer = 3.0f;
    currentClip = 0;
#endif

#ifdef SAMPLE3
    additiveTime = 0.0f;
    additiveDirection = 1.0f;
    
    clip = 0;
    additiveIndex = 0;
    for (unsigned int i = 0, size = (unsigned int)clips.size();
        i < size; ++i)
    {
        if (clips[i].GetName() == "Lean_Left") {
            additiveIndex = i;
        }
        else if (clips[i].GetName() == "Walking") {
            clip = i;
        }
    }
    
    additiveBase = MakeAdditivePose(skeleton, clips[additiveIndex]);
    clips[additiveIndex].SetLooping(false);
    
    currentPose = skeleton.GetRestPose();
    addPose = skeleton.GetRestPose();
    playbackTime = 0.0f;
#endif

    return true;
}

void Sample::Update(float inDeltaTime)
{
    Application::Update(inDeltaTime);

#ifdef SAMPLE1
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
#endif

#ifdef SAMPLE2
    fadeController.Update(inDeltaTime);
    fadeTimer -= inDeltaTime;
    if (fadeTimer < 0.0f) {
        fadeTimer = 3.0f;
        unsigned int clip = currentClip;
        while (clip == currentClip) {
            clip = rand() % clips.size();
        }
        currentClip = clip;
        
        fadeController.FadeTo(&clips[clip], 0.5f);
    }
    fadeController.GetCurrentPose().GetMatrixPalette(posePalette);
#endif

#ifdef SAMPLE3
    additiveTime += inDeltaTime * additiveDirection;
    if (additiveTime < 0.0f) {
        additiveTime = 0.0f;
        additiveDirection *= -1.0f;
    }
    
    if (additiveTime > 1.0f) {
        additiveTime = 1.0f;
        additiveDirection *= -1.0f;
    }
    
    playbackTime = clips[clip].Sample(currentPose, playbackTime + inDeltaTime);
    float time = clips[additiveIndex].GetStartTime() + (clips[additiveIndex].GetDuration() * additiveTime);
    clips[additiveIndex].Sample(addPose, time);
    Add(currentPose, currentPose, addPose, additiveBase, -1);
    
    currentPose.GetMatrixPalette(posePalette);
#endif
}

void Sample::Render(float inAspectRatio)
{
#ifdef SAMPLE1
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
#endif

#ifdef SAMPLE2
    Mat4 model;
    Mat4 view = lookAt(Vec3(0,3,7), Vec3(0,3,0), Vec3(0,1,0));
    Mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
    
    shader->Bind();
    Uniform<Mat4>::Set(shader->GetUniform("model"), model);
    Uniform<Mat4>::Set(shader->GetUniform("view"), view);
    Uniform<Mat4>::Set(shader->GetUniform("projection"), projection);
    Uniform<Mat4>::Set(shader->GetUniform("pose"), posePalette);
    Uniform<Mat4>::Set(shader->GetUniform("invBindPose"), skeleton.GetInvBindPose());
    Uniform<Vec3>::Set(shader->GetUniform("light"), Vec3(1,1,1));
    texture->Set(shader->GetUniform("tex0"), 0);
    for (unsigned int i = 0, size = (unsigned int)meshes.size();
        i < size; ++i)
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
#endif

#ifdef SAMPLE3
    Mat4 model;
    Mat4 view = lookAt(Vec3(0,3,7), Vec3(0,3,0), Vec3(0,1,0));
    Mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
    
    shader->Bind();
    Uniform<Mat4>::Set(shader->GetUniform("model"), model);
    Uniform<Mat4>::Set(shader->GetUniform("view"), view);
    Uniform<Mat4>::Set(shader->GetUniform("projection"), projection);
    Uniform<Mat4>::Set(shader->GetUniform("pose"), posePalette);
    Uniform<Mat4>::Set(shader->GetUniform("invBindPose"), skeleton.GetInvBindPose());
    Uniform<Vec3>::Set(shader->GetUniform("light"), Vec3(1,1,1));
    texture->Set(shader->GetUniform("tex0"), 0);
    for (unsigned int i = 0, size = (unsigned int)meshes.size();
        i < size; ++i)
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
#endif
}

void Sample::Shutdown()
{
    clips.clear();
    meshes.clear();
    delete shader;
    delete texture;

    Application::Shutdown();
}

