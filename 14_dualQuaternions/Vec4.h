#ifndef VEC4_H_INCLUDED
#define VEC4_H_INCLUDED

// data only, used to pass data to OpenGL

template<typename T>
struct TVec4 {
    
    union {
        struct {
            T x;
            T y;
            T z;
            T w;
        };
        T v[4];
    };

    inline TVec4() :
        x(T(0)),
        y(T(0)),
        z(T(0)),
        w(T(0))
    {}

    inline TVec4(T inX, T inY, T inZ, T inW) :
        x(inX),
        y(inY),
        z(inZ),
        w(inW)
    {}

    inline TVec4(T* fv) :
        x(fv[0]),
        y(fv[1]),
        z(fv[1]),
        w(fv[1])
    {}
};

typedef TVec4<float> Vec4;
typedef TVec4<int> iVec4;
typedef TVec4<unsigned int> uiVec4;

#endif

