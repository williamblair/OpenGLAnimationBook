#ifndef GLTF_LOADER_H_INCLUDED
#define GLTF_LOADER_H_INCLUDED

#include <vector>
#include <string>
#include <cgltf.h>
#include <Pose.h>
#include <Clip.h>
#include <Skeleton.h>
#include <Mesh.h>

cgltf_data* LoadGLTFFile(const char* path);
void        FreeGLTFFile(cgltf_data* data);
Pose        LoadRestPose(cgltf_data* data);
Pose        LoadBindPose(cgltf_data* data);
Skeleton    LoadSkeleton(cgltf_data* data);
std::vector<Mesh>        LoadMeshes(cgltf_data* data);
std::vector<Clip>        LoadAnimationClips(cgltf_data* data);
std::vector<std::string> LoadJointNames(cgltf_data* data);

#endif // GLTF_LOADER_H_INCLUDED

