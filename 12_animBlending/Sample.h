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
#include <RearrangeBones.h>
#include <CrossFadeController.h>
#include <CrossFadeTarget.h>
#include <Blending.h>

// uncomment to run a different sample
//#define SAMPLE1
//#define SAMPLE2
#define SAMPLE3

#ifdef SAMPLE1
struct AnimationInstance {
    unsigned int clip;
    float time;
    Pose pose;
};
#endif

class Sample : public Application
{
public:
    
    bool Initialize(const char* title, const int width, const int height);
    void Update(float inDeltaTime);
    void Render(float inAspectRatio);
    void Shutdown();

private:

    Shader* shader;
    Texture* texture;
    std::vector<Mesh> meshes;
    std::vector<Clip> clips;
    Skeleton skeleton;
    std::vector<Mat4> posePalette;
    
#ifdef SAMPLE1
    Pose pose;
    std::vector<Mat4> skinPalette;
    AnimationInstance animA;
    AnimationInstance animB;
    float blendTime;
    float invertBlend;
#endif
#ifdef SAMPLE2
    CrossFadeController fadeController;
    unsigned int currentClip;
    float fadeTimer;
#endif
#ifdef SAMPLE3
    unsigned int clip;
    unsigned int additiveIndex;
    float playbackTime;
    float additiveTime;
    float additiveDirection;
    Pose currentPose;
    Pose addPose;
    Pose additiveBase;
#endif

};

#endif // SAMPLE_H_INCLUDED

