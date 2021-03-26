#include <Attribute.h>
#include <Vec2.h>
#include <Vec3.h>
#include <Vec4.h>

template class Attribute<int>;
template class Attribute<float>;
template class Attribute<Vec2>;
template class Attribute<Vec3>;
template class Attribute<Vec4>;
template class Attribute<iVec4>;

template<typename T>
Attribute<T>::Attribute()
{
    glGenBuffers(1, &id);
    count = 0;
}

template<typename T>
Attribute<T>::~Attribute()
{
    glDeleteBuffers(1, &id);
}

template<>
void Attribute<int>::SetAttribPointer(unsigned int slot)
{
    glVertexAttribIPointer(slot, 1, GL_INT, 0, (void*)0);
}
template<>
void Attribute<float>::SetAttribPointer(unsigned int slot)
{
    glVertexAttribPointer(slot, 1, GL_FLOAT, GL_FALSE, 0, 0);
}
template<>
void Attribute<Vec2>::SetAttribPointer(unsigned int slot)
{
    glVertexAttribPointer(slot, 2, GL_FLOAT, GL_FALSE, 0, 0);
}
template<>
void Attribute<Vec3>::SetAttribPointer(unsigned int slot)
{
    glVertexAttribPointer(slot, 3, GL_FLOAT, GL_FALSE, 0, 0);
}
template<>
void Attribute<Vec4>::SetAttribPointer(unsigned int slot)
{
    glVertexAttribPointer(slot, 4, GL_FLOAT, GL_FALSE, 0, 0);
}
template<>
void Attribute<iVec4>::SetAttribPointer(unsigned int slot)
{
    glVertexAttribIPointer(slot, 4, GL_INT, 0, (void*)0);
}

template<typename T>
void Attribute<T>::Set(T* inputArray, unsigned int length)
{
    count = length;
    unsigned int size = sizeof(T); // each element size
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size * count, inputArray, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T>
void Attribute<T>::Set(std::vector<T>& input)
{
    Set(&input[0], (unsigned int)input.size());
}

template<typename T>
void Attribute<T>::BindTo(unsigned int slot)
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glEnableVertexAttribArray(slot);
    SetAttribPointer(slot);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T>
void Attribute<T>::UnbindFrom(unsigned int slot)
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glDisableVertexAttribArray(slot);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



