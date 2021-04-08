#include "CrossFadeController.h"

CrossFadeController::CrossFadeController()
{
    clip = nullptr;
    time = 0.0f;
    wasSkeletonSet = false;
}

CrossFadeController::CrossFadeController(Skeleton& skeleton)
{
    clip = nullptr;
    time = 0.0f;
    SetSkeleton(skeleton);
}

CrossFadeController::~CrossFadeController()
{
}

void CrossFadeController::SetSkeleton(Skeleton& skeleton)
{
    this->skeleton = skeleton;
    pose = skeleton.GetRestPose();
    wasSkeletonSet = true;
}

void CrossFadeController::Play(Clip* target)
{
    targets.clear();
    clip = target;
    pose = skeleton.GetRestPose();
    time = target->GetStartTime();
}

void CrossFadeController::FadeTo(Clip* target, float fadeTime)
{
    if (clip == nullptr) {
        Play(target);
        return;
    }
    
    if (targets.size() >= 1) {
        Clip* tmpClip = targets[targets.size()-1].clip;
        if (tmpClip == target) {
            return;
        }
    } else {
        if (clip == target) {
            return;
        }
    }
    
    targets.push_back(CrossFadeTarget(target, skeleton.GetRestPose(), fadeTime));
}

void CrossFadeController::Update(float dt)
{
    if (clip == nullptr || !wasSkeletonSet) {
        return;
    }
    
    // set current animation if enough time elapsed
    unsigned int numTargets = targets.size();
    for (unsigned int i = 0; i < numTargets; ++i) {
        float duration = targets[i].duration;
        if (targets[i].elapsed >= duration) {
            clip = targets[i].clip;
            time = targets[i].time;
            pose = targets[i].pose;
            targets.erase(targets.begin() + i);
            break;
        }
    }
    
    // blend fade list with current animation
    numTargets = targets.size();
    pose = skeleton.GetRestPose();
    time = clip->Sample(pose, time + dt);
    for (unsigned int i = 0; i < numTargets; ++i) {
        CrossFadeTarget& target = targets[i];
        target.time = target.clip->Sample(target.pose, target.time + dt);
        target.elapsed += dt;
        float t = target.elapsed / target.duration;
        if (t > 1.0f) { t = 1.0f; }
        Blend(pose, pose, target.pose, t, -1);
    }
}


