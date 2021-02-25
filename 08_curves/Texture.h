#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <GL/glew.h>
#include <stb_image.h>

class Texture
{
public:
    
    Texture();
    Texture(const char* path);
    ~Texture();

    void Load(const char* path);
    void Set(unsigned int uniform, unsigned int texIndex);
    void Unset(unsigned int texIndex);

    unsigned int GetHandle() const { return id; }

protected:

    unsigned int width;
    unsigned int height;
    unsigned int numChannels;
    unsigned int id;

private:
    
    // disallow copy/assignment
    Texture(const Texture&);
    Texture& operator=(const Texture&);
};

#endif // TEXTURE_H_INCLUDED

