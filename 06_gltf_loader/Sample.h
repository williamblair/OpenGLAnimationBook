#ifndef SAMPLE_H_INCLUDED
#define SAMPLE_H_INCLUDED

#include <Application.h>

#include <Mat4.h>
#include <Quat.h>
#include <Vec2.h>
#include <Vec3.h>
#include <Vec4.h>

#include <Shader.h>
#include <Attribute.h>
#include <Uniform.h>
#include <IndexBuffer.h>
#include <Texture.h>
#include <Draw.h>

class Sample : public Application
{
public:
    
    bool Initialize(const char* title, const int width, const int height);
    void Update(float inDeltaTime);
    void Render(float inAspectRatio);
    void Shutdown();

private:

    Shader*          shader;
    Attribute<Vec3>* vertexPositions;
    Attribute<Vec3>* vertexNormals;
    Attribute<Vec2>* vertexTexCoords;
    IndexBuffer*     indexBuffer;
    Texture*         displayTexture;
    float            rotation;

};

#endif // SAMPLE_H_INCLUDED

