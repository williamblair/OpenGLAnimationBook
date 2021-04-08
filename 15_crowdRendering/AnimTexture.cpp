#include <AnimTexture.h>
#include <iostream>
#include <fstream>

AnimTexture::AnimTexture()
{
    data = nullptr;
    size = 0;
    glGenTextures(1, &id);
}

AnimTexture::AnimTexture(const AnimTexture& other)
{
    data = nullptr;
    size = 0;
    glGenTextures(1, &id);
    *this = other;
}

AnimTexture::~AnimTexture()
{
    if (data != nullptr) {
        delete[] data;
    }
    glDeleteTextures(1, &id);
}

AnimTexture& AnimTexture::operator=(const AnimTexture& other)
{
    if (this == &other) {
        return *this;
    }
    size = other.size;
    if (data != nullptr) {
        delete[] data;
    }
    data = nullptr;
    if (size != 0) {
        data = new float[size * size * 4];
        std::copy(other.data, 
                  other.data + (other.size * other.size * 4), 
                  data);
    }
    
    return *this;
}

void AnimTexture::Save(const char* path)
{
    std::ofstream file(path, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << __func__ << ": failed to open " << path << std::endl;
        return;
    }
    file << size;
    if (size != 0) {
        file.write((char*)data, sizeof(float) * size * size * 4);
    }
    file.close();
}

void AnimTexture::Load(const char* path)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << __func__ << ": failed to open " << path << std::endl;
        return;
    }
    file >> size;
    data = new float[size * size * 4];
    file.read((char*)data, sizeof(float) * size * size * 4);
    file.close();
    UploadToGPU();
}

void AnimTexture::UploadToGPU()
{
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA32F,
                 size,
                 size, 
                 0, 
                 GL_RGBA,
                 GL_FLOAT,
                 data);

    // prevent mipmapping and interpolation so we get the exact desired values
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, 0); // unbind
}

void AnimTexture::Resize(unsigned int newSize)
{
    if (data != nullptr) {
        delete[] data;
    }
    size = newSize;
    data = new float[size * size * 4];
}

void AnimTexture::Bind(unsigned int uniform, unsigned int texture)
{
    glActiveTexture(GL_TEXTURE0 + texture);
    glBindTexture(GL_TEXTURE_2D, id);
    glUniform1i(uniform, texture); // tell the uniform which GL texture to use
}

void AnimTexture::Unbind(unsigned int texture)
{
    glActiveTexture(GL_TEXTURE0 + texture);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
}

void AnimTexture::SetTexel(unsigned int x, unsigned int y, const Vec3& v)
{
    unsigned int index = (y * size * 4) + (x * 4);
    data[index + 0] = v.x;
    data[index + 1] = v.y;
    data[index + 2] = v.z;
    data[index + 3] = 0.0f;
}

void AnimTexture::SetTexel(unsigned int x, unsigned int y, const Quat& q)
{
    unsigned int index = (y * size * 4) + (x * 4);
    data[index + 0] = q.x;
    data[index + 1] = q.y;
    data[index + 2] = q.z;
    data[index + 3] = q.w;
}

Vec4 AnimTexture::GetTexel(unsigned int x, unsigned int y)
{
    unsigned int index = (y * size * 4) + (x * 4);
    return Vec4(data[index + 0],
                data[index + 1],
                data[index + 2],
                data[index + 3]);
}

