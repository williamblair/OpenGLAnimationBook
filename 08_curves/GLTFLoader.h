#ifndef GLTF_LOADER_H_INCLUDED
#define GLTF_LOADER_H_INCLUDED

#include <cgltf.h>

cgltf_data* LoadGLTFFile(const char* path);
void        FreeGLTFFile(cgltf_data* data);

#endif // GLTF_LOADER_H_INCLUDED

