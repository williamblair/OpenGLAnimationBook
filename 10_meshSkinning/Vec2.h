#ifndef VEC2_H_INCLUDED
#define VEC2_H_INCLUDED

// data only, used to pass data to OpenGL

template<typename T>
struct TVec2 {
    
    union {
        struct {
            T x;
            T y;
        };
        T v[2];
    };

    inline TVec2() :
        x(T(0)),
        y(T(0))
    {}

    inline TVec2(T inX, T inY) :
        x(inX),
        y(inY)
    {}

    inline TVec2(T* fv) :
        x(fv[0]),
        y(fv[1])
    {}
};

typedef TVec2<float> Vec2;
typedef TVec2<int> iVec2;

#endif

