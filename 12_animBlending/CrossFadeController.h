#ifndef CROSS_FADE_CONTROLLER
#define CROSS_FADE_CONTROLLER

#include <vector>
#include <CrossFadeTarget.h>
#include <Clip.h>
#include <Skeleton.h>

class CrossFadeController
{
public:
    CrossFadeController();
    CrossFadeController(Skeleton& skeleton);
    ~CrossFadeController();
    
    void SetSkeleton(Skeleton& skeleton);
    void Play(Clip* target);
    void FadeTo(Clip* target, float fadeTime);
    void Update(float dt);
    
    inline Pose& GetCurrentPose() { return pose; }
    inline Clip* GetCurrentClip() { return clip; }

protected:
    std::vector<CrossFadeTarget> targets;
    Clip*       clip;
    float       time;
    Pose        pose;
    Skeleton    skeleton;
    bool        wasSkeletonSet;
};

#endif // CROSS_FADE_CONTROLLER
