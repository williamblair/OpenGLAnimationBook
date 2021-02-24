#include <GLTFLoader.h>
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

