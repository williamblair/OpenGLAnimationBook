#ifndef UNIFORM_H_INCLUDED
#define UNIFORM_H_INCLUDED

#include <vector>

#include <GL/glew.h>

#include <Vec2.h>
#include <Vec3.h>
#include <Vec4.h>
#include <Mat4.h>
#include <Quat.h>
#include <DualQuaternion.h>

template<typename T>
class Uniform
{
public:

    static void Set(unsigned int slot, const T& value);
    static void Set(unsigned int slot, const T* data, unsigned int length);
    static void Set(unsigned int slot, std::vector<T>& arr);

private:
    // no instances; only static methods
    Uniform();
    ~Uniform();
    Uniform& operator=(const Uniform&);
    Uniform(const Uniform&);
};

#endif // UNIFORM_H_INCLUDED

