#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include <GL/glew.h>

#include <IndexBuffer.h>

enum class DrawMode
{
    Points,
    LineStrip,
    LineLoop,
    Lines,
    Triangles,
    TriangleStrip,
    TriangleFan
};

void Draw(IndexBuffer& inIndexBuffer, DrawMode mode); // draw via element array
void Draw(unsigned int vertexCount, DrawMode mode); // draw array

// draw multiple instances of the same object
void DrawInstanced(IndexBuffer& inIndexBuffer, 
                   DrawMode mode, 
                   unsigned int instanceCount);
void DrawInstanced(unsigned int vertexCount,
                   DrawMode mode, 
                   unsigned int instanceCount);

#endif // DRAW_H_INCLUDED

