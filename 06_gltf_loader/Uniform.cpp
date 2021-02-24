#include <Uniform.h>

template class Uniform<int>;
template class Uniform<float>;
template class Uniform<Vec2>;
template class Uniform<Vec3>;
template class Uniform<Vec4>;
template class Uniform<iVec2>;
template class Uniform<iVec4>;
template class Uniform<Quat>;
template class Uniform<Mat4>;

template<typename T>
void Uniform<T>::Set(unsigned int slot, const T& value)
{
    Set(slot, (T*)&value, 1);
}

template<typename T>
void Uniform<T>::Set(unsigned int slot, std::vector<T>& arr)
{
    Set(slot, &arr[0], (unsigned int)arr.size());
}

#define UNIFORM_IMPL(gl_func, tType, dType)               \
    template<>                                            \
    void Uniform<tType>::Set(unsigned int slot,           \
                             const tType* data,           \
                             unsigned int length)         \
    {                                                     \
        gl_func(slot, (GLsizei)length, (dType*)&data[0]); \
    }

UNIFORM_IMPL(glUniform1iv, int, int)
UNIFORM_IMPL(glUniform4iv, iVec4, int)
UNIFORM_IMPL(glUniform2iv, iVec2, int)
UNIFORM_IMPL(glUniform1fv, float, float)
UNIFORM_IMPL(glUniform2fv, Vec2, float)
UNIFORM_IMPL(glUniform3fv, Vec3, float)
UNIFORM_IMPL(glUniform4fv, Vec4, float)
UNIFORM_IMPL(glUniform4fv, Quat, float)

#undef UNIFORM_IMPL

template<>
void Uniform<Mat4>::Set(unsigned int slot, const Mat4* data, unsigned int length)
{
    glUniformMatrix4fv(slot, (GLsizei)length, false, (float*)&data[0]);
}

