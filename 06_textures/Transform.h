#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

#include <Vec3.h>
#include <Quat.h>

struct Transform
{
    Vec3 position;
    Quat rotation;
    Vec3 scale;

    inline Transform(const Vec3& p, const Quat& r, const Vec3& s) :
        position(p),
        rotation(r),
        scale(s)
    {}

    inline Transform() :
        position(Vec3(0,0,0)),
        rotation(Quat(0,0,0,1)),
        scale(Vec3(1,1,1))
    {}
};

// operations apply from right to left like matrix multiplication
inline Transform combine(const Transform& a, const Transform& b) {
    Transform out;
    out.scale = a.scale * b.scale;
    out.rotation = b.rotation * a.rotation;
    out.position = a.rotation * (a.scale * b.position);
    out.position = a.position + out.position; // total operation is scale, rotate, translate
    return out;
}

inline Transform inverse(const Transform& t) {
    Transform inv;
    inv.rotation = inverse(t.rotation);
    inv.scale.x = fabs(t.scale.x) < VEC3_EPSILON ? 0.0f : 1.0f / t.scale.x;
    inv.scale.y = fabs(t.scale.y) < VEC3_EPSILON ? 0.0f : 1.0f / t.scale.y;
    inv.scale.z = fabs(t.scale.z) < VEC3_EPSILON ? 0.0f : 1.0f / t.scale.z;
    Vec3 invTrans = t.position * -1.0f;
    inv.position = inv.rotation * (inv.scale * invTrans);
    return inv;
}
inline Transform mix(const Transform& a, const Transform& b, float t) {
    // neighborhood check for optimal path interpolation
    Quat bRot = b.rotation;
    if (dot(a.rotation, bRot) < 0.0f) {
        bRot = -bRot;
    }

    return Transform(lerp(a.position, b.position, t),
                     nlerp(a.rotation, bRot, t),
                     lerp(a.scale, b.scale, t));
}
inline Mat4 transformToMat4(const Transform& t) {
    // extract rotation basis of transform
    Vec3 x = t.rotation * Vec3(1,0,0);
    Vec3 y = t.rotation * Vec3(0,1,0);
    Vec3 z = t.rotation * Vec3(0,0,1);
    // scale the basis vectors
    x = x * t.scale.x;
    y = y * t.scale.y;
    z = z * t.scale.z;
    // extract transform position
    Vec3 p = t.position;
    // put components into matrix
    return Mat4(x.x, x.y, x.z, 0, // x basis and scale
                y.x, y.y, y.z, 0, // y basis and scale
                z.x, z.y, z.z, 0, // z basis and scale
                p.x, p.y, p.z, 1); // position
}
inline Transform mat4ToTransform(const Mat4& m) {
    Transform out;
    out.position = Vec3(m.v[12], m.v[13], m.v[14]); // position = last column of mat
    out.rotation = mat4ToQuat(m);

    // get the rotate scale matrix, then estimate the scale from that
    Mat4 rotScaleMat(m.v[0], m.v[1], m.v[2],  0,
                     m.v[4], m.v[5], m.v[6],  0,
                     m.v[8], m.v[9], m.v[10], 0,
                     0,      0,      0,       1);
    Mat4 invRotMat = quatToMat4(inverse(out.rotation));
    Mat4 scaleSkewMat = rotScaleMat * invRotMat;
    out.scale = Vec3(scaleSkewMat.v[0], // the diagonal of the matrix is the scale
                     scaleSkewMat.v[5],
                     scaleSkewMat.v[10]);
    return out;
}
// operation order is right to left (scale, rotate, translate)
inline Vec3 transformPoint(const Transform& a, const Vec3& b) {
    Vec3 out;
    out = a.rotation * (a.scale * b);
    out = a.position + out;
    return out;
}
// vectors/directions cannot 'move', so translation is not applied
inline Vec3 transformVector(const Transform& a, const Vec3& b) {
    Vec3 out;
    out = a.rotation * (a.scale * b);
    return out;
}

#endif // TRANSFORM_H_INCLUDED

