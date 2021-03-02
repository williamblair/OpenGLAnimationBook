#ifndef SAMPLE_H_INCLUDED
#define SAMPLE_H_INCLUDED

#include <vector>

#include <Application.h>
#include <Draw.h>
#include <DebugDraw.h>
#include <Pose.h>
#include <Clip.h>
#include <GLTFLoader.h>
#include <Skeleton.h>
#include <Texture.h>
#include <Shader.h>
#include <Uniform.h>

struct AnimationInstance {
    Pose animatedPose;
    std::vector<Mat4> posePalette;
    unsigned int clip;
    float playback;
    Transform model;
    
    inline AnimationInstance() : clip(0), playback(0.0f) {}
};

class Sample : public Application
{
public:
    
    bool Initialize(const char* title, const int width, const int height);
    void Update(float inDeltaTime);
    void Render(float inAspectRatio);
    void Shutdown();

private:

// sample 1 - rest, bind, and current poses skeletons
#if 0
    Skeleton skeleton;
    Pose currentPose;
    std::vector<Clip> clips;
    unsigned int currentClip;
    float playbackTime;
    DebugDraw* bindPoseVisual;
    DebugDraw* restPoseVisual;
    DebugDraw* currentPoseVisual;
#endif

    Texture* diffuseTexture;
    Shader* staticShader;
    Shader* skinnedShader;
    std::vector<Mesh> CPUMeshes;
    std::vector<Mesh> GPUMeshes;
    Skeleton skeleton;
    std::vector<Clip> clips;
    
    AnimationInstance GPUAnimInfo;
    AnimationInstance CPUAnimInfo;
};

#endif // SAMPLE_H_INCLUDED

