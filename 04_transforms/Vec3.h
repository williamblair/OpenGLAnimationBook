#ifndef VEC3_H_INCLUDED
#define VEC3_H_INCLUDED

#include <cmath>

#define VEC3_EPSILON 0.000001f

struct Vec3
{

    union {
        struct {
            float x;
            float y;
            float z;
        };
        float v[3];
    };

    inline Vec3() : 
        x(0.0f), 
        y(0.0f), 
        z(0.0f)
    {}

    inline Vec3(float inX, float inY, float inZ) :
        x(inX),
        y(inY),
        z(inZ)
    {}

    inline Vec3(float* fv) :
        x(fv[0]),
        y(fv[1]),
        z(fv[2])
    {}
};

// component wise operations
Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
    return Vec3(lhs.x + rhs.x,
                lhs.y + rhs.y,
                lhs.z + rhs.z);
}
Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
    return Vec3(lhs.x - rhs.x,
                lhs.y - rhs.y,
                lhs.z - rhs.z);
}
Vec3 operator*(const Vec3& v, float f) {
    return Vec3(v.x * f, v.y * f, v.z * f);
}
Vec3 operator*(const Vec3& lhs, const Vec3& rhs) {
    return Vec3(lhs.x * rhs.x,
                lhs.y * rhs.y,
                lhs.z * rhs.z);
}
inline float dot(const Vec3& lhs, const Vec3& rhs) {
    return lhs.x * rhs.x +
           lhs.y * rhs.y +
           lhs.z * rhs.z;
}

// length in squared space to avoid a square root calculation
inline float lenSq(const Vec3& v) {
    return v.x*v.x + v.y*v.y + v.z*v.z;
}
// regular length calculation
inline float len(const Vec3& v) {
    float lenSq = v.x*v.x + v.y*v.y + v.z*v.z;
    if (lenSq < VEC3_EPSILON) {
        return 0.0f;
    }
    return sqrtf(lenSq);
}

// in-place and new vector normalization
inline void normalize(Vec3& v) {
    float lenSq = v.x*v.x + v.y*v.y + v.z*v.z;
    if (lenSq < VEC3_EPSILON) return;
    float invLen = 1.0f / sqrtf(lenSq);
    v.x *= invLen;
    v.y *= invLen;
    v.z *= invLen;
}
inline Vec3 normalized(const Vec3& v) {
    float lenSq = v.x*v.x + v.y*v.y + v.z*v.z;
    if (lenSq < VEC3_EPSILON) return v;
    float invLen = 1.0f / sqrtf(lenSq);
    return Vec3(v.x * invLen,
                v.y * invLen,
                v.z * invLen);
}

// non-normalized angle calculation (includes division by magnitude product)
inline float angle(const Vec3& lhs, const Vec3& rhs) {
    float lenSqL = lenSq(lhs);
    float lenSqR = lenSq(rhs);
    if (lenSqL < VEC3_EPSILON || lenSqR < VEC3_EPSILON ) {
        return 0.0f;
    }
    float len = sqrtf(lenSqL) * sqrtf(lenSqR);
    float dotProd = dot(lhs, rhs);
    return acosf(dotProd / len);
}

// projection and rejection of lhs onto rhs
inline Vec3 project(const Vec3& lhs, const Vec3& rhs) {
    float lenR = len(rhs);
    if (lenR < VEC3_EPSILON) return Vec3();
    float scale = dot(lhs, rhs) / lenR; // x component = cos() = dot(a,b)
    return rhs * scale;
}
inline Vec3 reject(const Vec3& lhs, const Vec3& rhs) {
    Vec3 projection = project(lhs, rhs);
    return lhs - projection;
}

// reflection
inline Vec3 reflect(const Vec3& lhs, const Vec3& rhs) {
    float lenR = len(rhs);
    if (lenR < VEC3_EPSILON) return Vec3();
    float scale = dot(lhs, rhs) / lenR;
    Vec3 proj2 = rhs * (scale * 2);
    return lhs - proj2;
}

// cross product
inline Vec3 cross(const Vec3& lhs, const Vec3& rhs) {
    return Vec3(lhs.y * rhs.z - lhs.z * rhs.y,
                lhs.z * rhs.x - lhs.x * rhs.z,
                lhs.x * rhs.y - lhs.y * rhs.x);
}

// linear interpolation (lerp)
inline Vec3 lerp(const Vec3& start, const Vec3& end, float t) {
    return Vec3(start.x + (end.x - start.x) * t,
                start.y + (end.y - start.y) * t,
                start.z + (end.z - start.z) * t);
}

// spherical linear interpolation (slerp)
inline Vec3 slerp(const Vec3& start, const Vec3& end, float t) {
    if (t < 0.01f) return lerp(start, end, t);

    Vec3 from = normalized(start);
    Vec3 to = normalized(end);
    float theta = angle(from, to);
    float sin_theta = sinf(theta);
    float a = sinf((1.0f - t) *  theta) / sin_theta;
    float b = sinf(t * theta) / sin_theta;

    return from * a + to * b;
}

// normalized linear interpolation (nlerp)
// close approximation to slerp and cheaper to calculate
inline Vec3 nlerp(const Vec3& start, const Vec3& end, float t) {
    return normalized(lerp(start, end, t));
}

// vector comparison
bool operator==(const Vec3& lhs, const Vec3& rhs) {
    Vec3 diff(lhs - rhs);
    return (lenSq(diff) < VEC3_EPSILON);
}
bool operator!=(const Vec3& lhs, const Vec3& rhs) {
    return !(lhs == rhs);
}

#endif // VEC3_H_INCLUDED

