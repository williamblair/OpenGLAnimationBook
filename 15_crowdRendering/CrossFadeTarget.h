#ifndef CROSS_FADE_TARGET_H
#define CROSS_FADE_TARGET_H

#include <Pose.h>
#include <Clip.h>

struct CrossFadeTarget
{
    Pose pose;
    Clip* clip;
    float time;
    float duration;
    float elapsed;
    
    inline CrossFadeTarget() :
        clip(nullptr),
        time(0.0f),
        duration(0.0f),
        elapsed(0.0f)
    {}
    
    inline CrossFadeTarget(Clip* inTarget, Pose& inPose, float inDuration) :
        pose(inPose),
        clip(inTarget),
        time(inTarget->GetStartTime()),
        duration(inDuration),
        elapsed(0.0f)
    {}

};

#endif // CROSS_FADE_TARGET_H
