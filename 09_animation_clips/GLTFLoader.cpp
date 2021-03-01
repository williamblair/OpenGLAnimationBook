#include <GLTFLoader.h>
#include <Transform.h>
#include <Mat4.h>
#include <Vec3.h>
#include <Quat.h>
#include <Track.h>
#include <Interpolation.h>
#include <Clip.h>
#include <iostream>
#include <cstring>

cgltf_data* LoadGLTFFile(const char* path)
{
    cgltf_options options;
    memset(&options, 0, sizeof(options));

    cgltf_data* data = nullptr;
    cgltf_result result = cgltf_parse_file(&options, path, &data);
    if (result != cgltf_result_success) {
        std::cout << __func__ << ": failed to load " << path << std::endl;
        return nullptr;
    }

    result = cgltf_load_buffers(&options, data, path);
    if (result != cgltf_result_success) {
        cgltf_free(data);
        std::cout << __func__ << ": failed to load " << path << std::endl;
        return nullptr;
    }

    result = cgltf_validate(data);
    if (result != cgltf_result_success) {
        cgltf_free(data);
        std::cout << __func__ << ": invalid file: " << path << std::endl;
        return nullptr;
    }

    return data;
}

void FreeGLTFFile(cgltf_data* data)
{
    if (data == nullptr) {
        std::cout << __func__ << ": warning: given null data" << std::endl;
    } else {
        cgltf_free(data);
    }
}

namespace GLTFHelpers
{
    Transform GetLocalTransform(cgltf_node& n)
    {
        Transform result;

        if (n.has_matrix) {
            Mat4 mat(&n.matrix[0]);
            result = mat4ToTransform(mat);
        }
        if (n.has_translation) {
            result.position = Vec3(n.translation[0],
                                   n.translation[1],
                                   n.translation[2]);
        }
        if (n.has_rotation) {
            result.rotation = Quat(n.rotation[0],
                                   n.rotation[1],
                                   n.rotation[2],
                                   n.rotation[3]);
        }
        if (n.has_scale) {
            result.scale = Vec3(n.scale[0], n.scale[1], n.scale[2]);
        }

        return result;
    }

    int GetNodeIndex(cgltf_node* target,
                     cgltf_node* allNodes,
                     unsigned int numNodes) {
        if (target == nullptr) {
            std::cout << "Returning -1 node index" << std::endl;
            return -1;
        }

        for (unsigned int i = 0; i < numNodes; ++i) {
            if (target == &allNodes[i]) {
                return (int)i;
            }
        }

        std::cout << "Failed to find target node index" << std::endl;
        return -1;
    }

    void GetScalarValues(std::vector<float>& out,
                        unsigned int compCount,             // num elements per component
                        const cgltf_accessor& inAccessor) {
        out.resize(inAccessor.count * compCount);
        for (cgltf_size i = 0; i < inAccessor.count; ++i) {
            cgltf_accessor_read_float(&inAccessor,
                                      i,
                                      &out[i * compCount],
                                      compCount);
        }
    }

    // fills in all of the frames in a track with the CGLTF data
    template<typename T, int N>
    void TrackFromChannel(Track<T,N>& result,
                          const cgltf_animation_channel& channel) {

        cgltf_animation_sampler& sampler = *channel.sampler;        

        Interpolation interpolation = Interpolation::Constant;
        if (sampler.interpolation == cgltf_interpolation_type_linear) {
            interpolation = Interpolation::Linear;
        } else if (sampler.interpolation == cgltf_interpolation_type_cubic_spline) {
            interpolation = Interpolation::Cubic;
        }
        result.SetInterpolation(interpolation);

        bool isSamplerCubic = (interpolation == Interpolation::Cubic);
        
        std::vector<float> times;
        std::vector<float> vals;
        GetScalarValues(times, 1, *sampler.input);
        GetScalarValues(vals, N, *sampler.output); // groups of N components

        unsigned int numFrames = sampler.input->count;
        unsigned int compCount = vals.size() / times.size();
        result.Resize(numFrames);

        for (unsigned int i = 0; i < numFrames; ++i) {
            int baseIndex = i * compCount;
            Frame<N>& frame = result[i];
            int offset = 0;

            frame.time = times[i];

            for (int comp = 0; comp < N; ++comp) {
                frame.in[comp] = isSamplerCubic ? vals[baseIndex + offset++] : 0.0f;
            }
            for (int comp = 0; comp < N; ++comp) {
                frame.value[comp] = vals[baseIndex + offset++];
            }
            for (int comp = 0; comp < N; ++comp) {
                frame.out[comp] = isSamplerCubic ? vals[baseIndex + offset++] : 0.0f;
            }
        }
    }

}

Pose LoadRestPose(cgltf_data* data)
{
    unsigned int boneCount = data->nodes_count;
    Pose result(boneCount);

    for (unsigned int i = 0; i < boneCount; ++i)
    {
        cgltf_node* node = &(data->nodes[i]);

        Transform transform = GLTFHelpers::GetLocalTransform(data->nodes[i]);
        result.SetLocalTransform(i, transform);
        int parent = GLTFHelpers::GetNodeIndex(node->parent, data->nodes, boneCount);
        result.SetParent(i, parent);
    }

    return result;
}

std::vector<std::string> LoadJointNames(cgltf_data* data)
{
    unsigned int boneCount = (unsigned int)data->nodes_count;
    std::vector<std::string> result(boneCount, "Not Set");

    for (unsigned int i = 0; i < boneCount; ++i) {
        cgltf_node* node = &(data->nodes[i]);

        if (node->name == nullptr) {
            result[i] = "EMPTY NODE";
        } else {
            result[i] = std::string(node->name);
        }
    }

    return result;
}

std::vector<Clip> LoadAnimationClips(cgltf_data* data) {
    unsigned int numClips = data->animations_count;
    unsigned int numNodes = data->nodes_count;

    std::vector<Clip> result;
    result.resize(numClips);

    for (unsigned int i = 0; i < numClips; ++i) {
        result[i].SetName(data->animations[i].name);

        unsigned int numChannels = data->animations[i].channels_count;
        for (unsigned int j = 0; j < numChannels; ++j) {
            cgltf_animation_channel& channel = data->animations[i].channels[j];
            cgltf_node* target = channel.target_node;
            int nodeId = GLTFHelpers::GetNodeIndex(target, data->nodes, numNodes);

            if (channel.target_path == cgltf_animation_path_type_translation) {
                VectorTrack& track = result[i][nodeId].GetPositionTrack();
                GLTFHelpers::TrackFromChannel<Vec3, 3>(track, channel);
            } else if (channel.target_path == cgltf_animation_path_type_scale) {
                VectorTrack& track = result[i][nodeId].GetScaleTrack();
                GLTFHelpers::TrackFromChannel<Vec3, 3>(track, channel);
            } else if (channel.target_path == cgltf_animation_path_type_rotation) {
                QuaternionTrack& track = result[i][nodeId].GetRotationTrack();
                GLTFHelpers::TrackFromChannel<Quat, 4>(track, channel);
            }
        }

        result[i].RecalculateDuration();
    }

    return result;
}
