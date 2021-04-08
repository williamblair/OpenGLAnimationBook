#ifndef BLENDING_H_INCLUDED
#define BLENDING_H_INCLUDED

#include <Pose.h>
#include <Skeleton.h>
#include <Clip.h>

Pose MakeAdditivePose(Skeleton& skeleton, Clip& clip);
void Add(Pose& output, Pose& inPose, Pose& addPose, Pose& addititiveBasePose, int blendroot);

#endif // BLENDING_H_INCLUDED
