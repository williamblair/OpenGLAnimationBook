#include <Texture.h>


Texture::Texture()
{
    width = 0;
    height = 0;
    numChannels = 0;
    glGenTextures(1, &id);
}
Texture::Texture(const char* path)
{
    glGenTextures(1, &id);
    Load(path);
}
Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void Texture::Load(const char* path)
{
    glBindTexture(GL_TEXTURE_2D, id);
    int tmpWidth, tmpHeight, tmpNumChannels;
    unsigned char* data = stbi_load(path, &tmpWidth, &tmpHeight, &tmpNumChannels, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmpWidth, tmpHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    width = tmpWidth;
    height = tmpHeight;
    numChannels = tmpNumChannels;
}

void Texture::Set(unsigned int uniform, unsigned int texIndex)
{
    glActiveTexture(GL_TEXTURE0 + texIndex);
    glBindTexture(GL_TEXTURE_2D, id);
    glUniform1i(uniform, texIndex);
}
void Texture::Unset(unsigned int texIndex)
{
    glActiveTexture(GL_TEXTURE0 + texIndex);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
}

