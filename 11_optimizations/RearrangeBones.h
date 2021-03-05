#ifndef REARRANGE_BONES_H_INCLUDED
#define REARRANGE_BONES_H_INCLUDED

#include <map>
#include <Skeleton.h>
#include <Pose.h>
#include <Clip.h>
#include <Mesh.h>

typedef std::map<int, int> BoneMap;

BoneMap RearrangeSkeleton(Skeleton& skeleton);
void RearrangeMesh(Mesh& mesh, BoneMap& boneMap);
void RearrangeClip(Clip& clip, BoneMap& boneMap);
//void RearrangeFastClip(FastClip& clip, BoneMap& boneMap);

#endif // REARRANGE_BONES_H_INCLUDED
