#include <Sample.h>

bool Sample::Initialize(const char* title, const int width, const int height)
{
    if (!Application::Initialize(title, width, height)) {
        return false;
    }

    cgltf_data* gltf = LoadGLTFFile("Assets/dq.gltf");
    meshes = LoadMeshes(gltf);
    skeleton = LoadSkeleton(gltf);
    clips = LoadAnimationClips(gltf);
    FreeGLTFFile(gltf);

    linearBlendShader = new Shader("Shaders/skinned.vert", "Shaders/lit.frag");
    dualQuatShader = new Shader("Shaders/skinnedDualQuats.vert", "Shaders/lit.frag");
    diffuseTexture = new Texture("Assets/dq.png");

    currentPose = skeleton.GetRestPose();

    currentPose.GetDualQuaternionPalette(dqPosePalette);
    skeleton.GetInvBindPose(dqInvBindPalette);

    currentPose.GetMatrixPalette(lbPosePalette);
    lbInvBindPalette = skeleton.GetInvBindPose();

    currentClip = 0;
    playbackTime = 0.0f;

    return true;
}

void Sample::Update(float inDeltaTime)
{
    Application::Update(inDeltaTime);

    playbackTime = clips[currentClip].Sample(currentPose, playbackTime + inDeltaTime);
    currentPose.GetDualQuaternionPalette(dqPosePalette);
    currentPose.GetMatrixPalette(lbPosePalette);
}

void Sample::Render(float inAspectRatio)
{
    Mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
    Mat4 view = lookAt(Vec3(0, 3, 14), Vec3(0, 0, 0), Vec3(0, 1, 0));

    // Dual quaternion
    Transform model(Vec3(2, 2, 0), Quat(), Vec3(1,1,1));
    dualQuatShader->Bind();
    Uniform<Mat4>::Set(dualQuatShader->GetUniform("model"), transformToMat4(model));
    Uniform<Mat4>::Set(dualQuatShader->GetUniform("view"), view);
    Uniform<Mat4>::Set(dualQuatShader->GetUniform("projection"), projection);
    Uniform<Vec3>::Set(dualQuatShader->GetUniform("light"), Vec3(1,1,1));
    Uniform<DualQuaternion>::Set(dualQuatShader->GetUniform("pose"), dqPosePalette);
    Uniform<DualQuaternion>::Set(dualQuatShader->GetUniform("invBindPose"), dqInvBindPalette);

    diffuseTexture->Set(dualQuatShader->GetUniform("tex0"), 0);
    for (unsigned int i = 0, size = (unsigned int)meshes.size(); i < size; ++i)
    {
        meshes[i].Bind(dualQuatShader->GetAttribute("position"),
                        dualQuatShader->GetAttribute("normal"),
                        dualQuatShader->GetAttribute("texCoord"),
                        dualQuatShader->GetAttribute("weights"),
                        dualQuatShader->GetAttribute("joints"));
        meshes[i].Draw();
        meshes[i].Unbind(dualQuatShader->GetAttribute("position"),
                        dualQuatShader->GetAttribute("normal"),
                        dualQuatShader->GetAttribute("texCoord"),
                        dualQuatShader->GetAttribute("weights"),
                        dualQuatShader->GetAttribute("joints"));
        
    }
    diffuseTexture->Unset(0);
    dualQuatShader->UnBind();

    // Linear Blend
    linearBlendShader->Bind();
    model.position.x = -2;
    model.position.y = -2;
    linearBlendShader->Bind();
    Uniform<Mat4>::Set(linearBlendShader->GetUniform("model"), transformToMat4(model));
    Uniform<Mat4>::Set(linearBlendShader->GetUniform("view"), view);
    Uniform<Mat4>::Set(linearBlendShader->GetUniform("projection"), projection);
    Uniform<Vec3>::Set(linearBlendShader->GetUniform("light"), Vec3(1,1,1));
    Uniform<Mat4>::Set(linearBlendShader->GetUniform("pose"), lbPosePalette);
    Uniform<Mat4>::Set(linearBlendShader->GetUniform("invBindPose"), lbInvBindPalette);

    diffuseTexture->Set(linearBlendShader->GetUniform("tex0"), 0);
    for (unsigned int i = 0, size = (unsigned int)meshes.size(); i < size; ++i)
    {
        meshes[i].Bind(linearBlendShader->GetAttribute("position"),
                        linearBlendShader->GetAttribute("normal"),
                        linearBlendShader->GetAttribute("texCoord"),
                        linearBlendShader->GetAttribute("weights"),
                        linearBlendShader->GetAttribute("joints"));
        meshes[i].Draw();
        meshes[i].Unbind(linearBlendShader->GetAttribute("position"),
                        linearBlendShader->GetAttribute("normal"),
                        linearBlendShader->GetAttribute("texCoord"),
                        linearBlendShader->GetAttribute("weights"),
                        linearBlendShader->GetAttribute("joints"));
        
    }
    diffuseTexture->Unset(0);
    linearBlendShader->UnBind();
}

void Sample::Shutdown()
{
    Application::Shutdown();
   
    delete diffuseTexture;
    delete linearBlendShader;
    delete dualQuatShader;
    meshes.clear();
    clips.clear(); 
}

