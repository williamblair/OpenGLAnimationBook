#ifndef DUAL_QUATERNION_H_INCLUDED
#define DUAL_QUATERNION_H_INCLUDED

#include <Transform.h>
#include <Quat.h>

struct DualQuaternion
{
    union
    {
        struct
        {
            Quat real; // holds rotation
            Quat dual; // holds position
        } quats;
        float v[8];
    };

    inline DualQuaternion()
    {
        quats.real = Quat(0,0,0,1);
        quats.dual = Quat(0,0,0,0);
    }
    inline DualQuaternion(const Quat& r, const Quat& d)
    {
        quats.real = r;
        quats.dual = d;
    }
};

inline DualQuaternion normalized(const DualQuaternion& d)
{
    float magSq = dot(d.quats.real, d.quats.real);
    if (magSq < 0.000001f) {
        return DualQuaternion();
    }
    float invMag = 1.0f / sqrtf(magSq);
    return DualQuaternion(d.quats.real * invMag,
                          d.quats.dual * invMag);
}
inline void normalize(DualQuaternion& d)
{
    float magSq = dot(d.quats.real, d.quats.real);
    if (magSq < 0.000001f) {
        return;
    }
    float invMag = 1.0f / sqrtf(magSq);
    d.quats.real = d.quats.real * invMag;
    d.quats.dual = d.quats.dual * invMag;
}

inline DualQuaternion operator+(const DualQuaternion& l, const DualQuaternion& r)
{
    return DualQuaternion(l.quats.real + r.quats.real,
                          l.quats.dual + r.quats.dual);    
}
inline DualQuaternion operator*(const DualQuaternion& l, float f)
{
    return DualQuaternion(l.quats.real * f, l.quats.dual * f);
}
// Multiplication order is from left to right; the opposite of matrices and quaternions
inline DualQuaternion operator*(const DualQuaternion& l, const DualQuaternion& r)
{
    DualQuaternion lhs = normalized(l);
    DualQuaternion rhs = normalized(r);

    return DualQuaternion(lhs.quats.real * rhs.quats.real,
                          lhs.quats.real * rhs.quats.dual +
                          lhs.quats.dual * rhs.quats.real);
}

inline bool operator==(const DualQuaternion& l, const DualQuaternion& r)
{
    return ((l.quats.real == r.quats.real) && (l.quats.dual == r.quats.dual));
}
inline bool operator!=(const DualQuaternion& l, const DualQuaternion& r)
{
    return !(l == r);
}

// If dot product > 0 -> both point in same direction
// If dot product < 0 -> they point in opposide directions
// If dot product == 0-> the are perpindicular
inline float dot(const DualQuaternion& l, const DualQuaternion& r)
{
    return dot(l.quats.real, r.quats.real);
}

inline DualQuaternion conjugate(const DualQuaternion& d)
{
    return DualQuaternion(conjugate(d.quats.real),
                          conjugate(d.quats.dual));
}

inline DualQuaternion transformToDualQuat(const Transform& t)
{
    Quat dual(t.position.x, t.position.y, t.position.z, 0);
    Quat real = t.rotation;
    Quat qd = real * dual * 0.5f;
    return DualQuaternion(real, qd);
}

inline Transform dualQuatToTransform(const DualQuaternion& d)
{
    Transform result;
    result.rotation = d.quats.real;
    Quat dual = conjugate(d.quats.real) * (d.quats.dual * 2.0f);
    result.position = Vec3(dual.x, dual.y, dual.z);
    return result;
}

inline Vec3 transformVector(const DualQuaternion& d, const Vec3& v)
{
    return d.quats.real * v;
}

inline Vec3 transformPoint(const DualQuaternion& d, const Vec3& v)
{
    Quat dual = conjugate(d.quats.real) * (d.quats.dual * 2.0f);
    Vec3 t = Vec3(dual.x, dual.y, dual.z);
    return d.quats.real * v + t;
}

#endif // DUAL_QUATERNION_H_INCLUDED

