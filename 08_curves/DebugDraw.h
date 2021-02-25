#ifndef DEBUG_DRAW_H_INCLUDED
#define DEBUG_DRAW_H_INCLUDED

#include <Shader.h>
#include <Attribute.h>
#include <Vec3.h>
#include <Mat4.h>
#include <vector>

enum class DebugDrawMode {
    Lines, Loop, Strip, Points
};

class DebugDraw
{
protected:
    std::vector<Vec3> mPoints;
    Attribute<Vec3>* mAttribs;
    Shader* mShader;
private:
    DebugDraw(const DebugDraw&);
    DebugDraw& operator=(const DebugDraw&);
public:
    DebugDraw();
    DebugDraw(unsigned int size);
    ~DebugDraw();

    unsigned int Size();
    void Resize(unsigned int newSize);
    Vec3& operator[](unsigned int index);
    void Push(const Vec3& v);

    void UpdateOpenGLBuffers();
    void Draw(DebugDrawMode mode, const Vec3& color, const Mat4& mvp);
};

#endif // DEBUG_DRAW_H_INCLUDED
