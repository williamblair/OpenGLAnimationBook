#ifndef ANIM_TEXTURE_H_INCLUDED
#define ANIM_TEXTURE_H_INCLUDED

#include <GL/glew.h>
#include <stb_image.h>

#include <Vec3.h>
#include <Vec4.h>
#include <Quat.h>

class AnimTexture
{
public:

    AnimTexture();
    AnimTexture(const AnimTexture&);
    ~AnimTexture(); 

    AnimTexture& operator=(const AnimTexture&);

    void Load(const char* path);
    void Save(const char* path);

    void UploadToGPU();

    void Resize(unsigned int newSize);
    unsigned int GetSize() const { return size; }

    float* GetData()     const { return data; }
    unsigned int GetId() const { return id; }

    void SetTexel(unsigned int x, unsigned int y, const Vec3& v);
    void SetTexel(unsigned int x, unsigned int y, const Quat& q);
    Vec4  GetTexel(unsigned int x, unsigned int y);

    void Bind(unsigned int uniform, unsigned int texture);
    void Unbind(unsigned int texture);

private:

    float* data;
    unsigned int size; // width and height assumed to be equal
    unsigned int id;
};

#endif // ANIM_TEXTURE_H_INCLUDED

