#include <IndexBuffer.h>

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &id);
    count = 0;
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &id);
}

void IndexBuffer::Set(unsigned int* arr, unsigned int len)
{
    count = len;
    unsigned int size = sizeof(unsigned int);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * count, arr, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Set(std::vector<unsigned int>& input)
{
    Set(&input[0], (unsigned int)input.size());
}

