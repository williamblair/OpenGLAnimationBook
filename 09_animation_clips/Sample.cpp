#include <Sample.h>
#include <vector>
#include <cstdarg>
    

bool Sample::Initialize(const char* title, const int width, const int height)
{
    if (!Application::Initialize(title, width, height)) {
        return false;
    }

    cgltf_data* gltf = LoadGLTFFile("assets/Woman.gltf");
    restPose = LoadRestPose(gltf);
    clips = LoadAnimationClips(gltf);
    FreeGLTFFile(gltf);

    restPoseVisual = new DebugDraw();
    restPoseVisual->FromPose(restPose);
    restPoseVisual->UpdateOpenGLBuffers();

    currentClip = 0;
    currentPose = restPose;
    
    playbackTime = 0.0f;

    currentPoseVisual = new DebugDraw();
    currentPoseVisual->FromPose(currentPose);
    currentPoseVisual->UpdateOpenGLBuffers();

    unsigned int numUIClips = (unsigned int)clips.size();
    for (unsigned int i = 0 ; i < numUIClips; ++i) {
        if (clips[i].GetName() == "Walking") {
            std::cout << "Found walking anim" << std::endl;
            currentClip = i;
            break;
        }
    }

    return true;
}

void Sample::Update(float inDeltaTime)
{
    Application::Update(inDeltaTime);
    Pose lastPose = currentPose;
    playbackTime = clips[currentClip].Sample(currentPose, playbackTime + inDeltaTime);
    currentPoseVisual->FromPose(currentPose);
}

void Sample::Render(float inAspectRatio)
{
    Mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
    Mat4 view = lookAt(Vec3(0,4,7), Vec3(0,4,0), Vec3(0,1,0));
    Mat4 mvp = projection * view; // no model matrix

    //restPoseVisual->Draw(DebugDrawMode::Lines, Vec3(1,0,0), mvp);
    currentPoseVisual->UpdateOpenGLBuffers();
    currentPoseVisual->Draw(DebugDrawMode::Lines, Vec3(0,0,1), mvp);
}

void Sample::Shutdown()
{
    delete restPoseVisual;
    delete currentPoseVisual;
    clips.clear();
    Application::Shutdown();
}

