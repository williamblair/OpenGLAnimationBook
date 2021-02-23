#ifndef QUAT_H_INCLUDED
#define QUAT_H_INCLUDED

#include <Vec3.h>
#include <Mat4.h>

#define QUAT_EPSILON 0.000001f

// quaternion
struct Quat
{
    union {
        struct {
            float x;
            float y; 
            float z; 
            float w;
        };
        struct {
            Vec3 vector;
            float scalar;
        } vecScalar;
        float v[4];
    };

    inline Quat() :
        x(0),
        y(0),
        z(0),
        w(1)
    {}

    inline Quat(float inX, float inY, float inZ, float inW) :
        x(inX),
        y(inY),
        z(inZ),
        w(inW)
    {}
};

// 0.5 angle because a quaternion can track two full rotations (720 degrees),
// so the angle needs to be scaled into the range 0..360
inline Quat angleAxis(float angle, const Vec3& axis) {
    Vec3 norm = normalized(axis);
    float s = sinf(angle * 0.5f);
    return Quat(norm.x * s,
                norm.y * s,
                norm.z * s,
                cosf(angle * 0.5f));
}

// rotation upon from in order to get to 'to'
inline Quat fromTo(const Vec3& from, const Vec3& to) {
    Vec3 f = normalized(from);
    Vec3 t = normalized(to);
    if (f == t) {
        return Quat();
    } else if (f == t * -1.0f) { // opposite directions

        // the most orthogonal axis of from used to create quat
        Vec3 ortho = Vec3(1, 0, 0);
        if (fabsf(f.y) < fabsf(f.x)) {
            ortho = Vec3(0, 1, 0);
        }
        if (fabsf(f.z) < fabsf(f.y) &&
            fabsf(f.z) < fabsf(f.x)) {
            ortho = Vec3(0, 0, 1);
        }

        Vec3 axis = normalized(cross(f, ortho));
        return Quat(axis.x, axis.y, axis.z, 0);
    }

    // sin of the half vector between the two (cross product) is the axis of
    // rotation, and cos of the half vector (dot product) is the angle
    Vec3 half = normalized(f + t);
    Vec3 axis = cross(f, half);
    return Quat(axis.x, axis.y, axis.z, dot(f, half)); // dot is the angle
}

// axis is normalized x,y,z component vector
inline Vec3 getAxis(const Quat& quat) {
    return normalized(Vec3(quat.x, quat.y, quat.z));
}

// angle is double the inverse cosine of the real component (w)
inline float getAngle(const Quat& quat) {
    return 2.0f * acosf(quat.w);
}

// Quaternion operations
Quat operator+(const Quat& a, const Quat& b) {
    return Quat(a.x + b.x,
                a.y + b.y,
                a.z + b.z,
                a.w + b.w);
}
Quat operator-(const Quat& a, const Quat& b) {
    return Quat(a.x - b.x,
                a.y - b.y,
                a.z - b.z,
                a.w - b.w);
}
Quat operator*(const Quat& a, const float b) {
    return Quat(a.x * b,
                a.y * b,
                a.z * b,
                a.w * b);
}
// this equation derived from the fact of the imaginary components
// ijk = -1, and ii = jj = kk = -1
Quat operator*(const Quat& a, const Quat& b) {
    return Quat( b.x*a.w + b.y*a.z - b.z*a.y + b.w*a.x,
                -b.x*a.z + b.y*a.w + b.z*a.x + b.w*a.y,
                 b.x*a.y - b.y*a.x + b.z*a.w + b.w*a.z,
                -b.x*a.x - b.y*a.y - b.z*a.z + b.w*a.w);
}
// alternate multiply implementaiton:
//Quat operator*(const Quat& a, const Quat& b) {
//    Quat result;
//    result.vecScalar.scalar = b.vecScalar.scalar * a.vecScalar.scalar - 
//                             dot(b.vecScalar.vector, a.vecScalar.vector);
//    result.vecScalara.vector = (a.vecScalar.vector * b.vecScalar.scalar) +
//                               (b.vecScalar.vector * a.vecScalar.scalar) +
//                               cross(b.vecScalar.vector, a.vecScalar.vector);
//    return result;
//}
Quat operator-(const Quat& q) {
    return Quat(-q.x, -q.y, -q.z, -q.w);
}

// two quaternions are the same angle if one is the inverse and
// they end up rotating to the same spot
// so just because they aren't identical component wise doesn't mean they
// don't represent the same result rotation
bool operator==(const Quat& lhs, const Quat& rhs) {
    return (fabsf(lhs.x - rhs.x) <= QUAT_EPSILON &&
            fabsf(lhs.y - rhs.y) <= QUAT_EPSILON &&
            fabsf(lhs.z - rhs.z) <= QUAT_EPSILON &&
            fabsf(lhs.w - rhs.w) <= QUAT_EPSILON);
}
bool operator!=(const Quat& lhs, const Quat& rhs) {
    return !(lhs == rhs);
}

bool sameOrientation(const Quat& lhs, const Quat& rhs) {
    return (fabsf(lhs.x - rhs.x) <= QUAT_EPSILON &&
            fabsf(lhs.y - rhs.y) <= QUAT_EPSILON &&
            fabsf(lhs.z - rhs.z) <= QUAT_EPSILON &&
            fabsf(lhs.w - rhs.w) <= QUAT_EPSILON) ||
            
           (fabsf(lhs.x + rhs.x) <= QUAT_EPSILON &&
            fabsf(lhs.y + rhs.y) <= QUAT_EPSILON &&
            fabsf(lhs.z + rhs.z) <= QUAT_EPSILON &&
            fabsf(lhs.w + rhs.w) <= QUAT_EPSILON);
}

inline float dot(const Quat& a, const Quat& b) {
    return a.x * b.x + 
           a.y * b.y + 
           a.z * b.z + 
           a.w * b.w;
}
// length squared
// length squared of a normalized quat is always 1
inline float lenSq(const Quat& q) {
    return q.x * q.x + 
           q.y * q.y +
           q.z * q.z +
           q.w * q.w;
}
inline float len(const Quat& q) {
    float lenSqrd = lenSq(q);
    if (lenSqrd < QUAT_EPSILON) {
        return 0.0f;
    }
    return sqrtf(lenSqrd);
}

// a normalized quaternion represents a pure rotation, without any additional skew
inline void normalize(Quat& q) {
    float lenSqrd = lenSq(q);
    if (lenSqrd < QUAT_EPSILON) {
        return;
    }
    float invLen = 1.0f / sqrtf(lenSqrd);
    q.x *= invLen;
    q.y *= invLen;
    q.z *= invLen;
    q.w *= invLen;
}
inline Quat normalized(const Quat& q) {
    float lenSqrd = lenSq(q);
    if (lenSqrd < QUAT_EPSILON) {
        return Quat();
    }
    float invLen = 1.0f / sqrtf(lenSqrd);
    return Quat(q.x * invLen,
                q.y * invLen,
                q.z * invLen,
                q.w * invLen);
}

// inverse of a normalized quaternion is its conjugate
// conjugate of a quaternion flips the axis of rotation
inline Quat conjugate(const Quat& q) {
    return Quat(-q.x, -q.y, -q.z, q.w);
}
// non-normalized inverse
inline Quat inverse(const Quat& q) {
    float lenSqrd = lenSq(q);
    if (lenSqrd < QUAT_EPSILON) {
        return Quat();
    }
    float invLenSqrd = 1.0f / lenSqrd;
    return Quat(-q.x * invLenSqrd,
                -q.y * invLenSqrd,
                -q.z * invLenSqrd,
                 q.w * invLenSqrd);
}

// quaternion-vector multiplication, resulting in the rotated
// version of the vector
// this is the simplified version of the formula 
// (q*vec(vx,vy,vz,0)*q_inverse).xyz; vx,vy,vz,0 is the vector v turned into
// a 'pure quaternion', which is a quaternion with real/scalar component of 0
// and normalized imaginary/vector component
inline Vec3 operator*(const Quat& q, const Vec3& v) {
    return q.vecScalar.vector * 2.0f * dot(q.vecScalar.vector, v) +
            v * (q.vecScalar.scalar * q.vecScalar.scalar - 
                 dot(q.vecScalar.vector, q.vecScalar.vector)) +
            cross(q.vecScalar.vector, v) * 2.0f * q.vecScalar.scalar;
}

// neighborhooding figures out whether interpolation between two quaternions
// is going to take the shortest or longest route. if the longer route is
// begin taken (dot prod < 0), can be changed to take the shortest route
// by negating one of the quaternions. after negation, interpolcation
// will then take the shortest route
//inline Quat sampleNeighborhood(const Quat& a, const Quat& b) {
//    if (dot(a,b) < 0.0f) {
//        b = -b;
//    }
//    return slerp(a, b, 0.5f);
//}

// below functions assume quaternions have already been neighborhooded

// achieves same idea as lerp except an arc is still traveled
inline Quat Mix(const Quat& from, const Quat& to, float t) {
    return from * (1.0f - t) + to * t;
}

// normalized lerp, again assume are in desired neighborhood
// fast approximation of slerp
inline Quat nlerp(const Quat& from, const Quat& to, float t) {
    return normalized(from + (to - from) * t);
}

// power operator
// quat = (v*sin(t*theta/2), cos(t*theta/2))
Quat operator^(const Quat& q, float f) {
    float angle = 2.0f * acosf(q.vecScalar.scalar);
    Vec3 axis = normalized(q.vecScalar.vector);
    float halfCos = cosf(f * angle * 0.5f);
    float halfSin = sinf(f * angle * 0.5f);
    return Quat(axis.x * halfSin,
                axis.y * halfSin,
                axis.z * halfSin,
                halfCos);
}

// slerp only required if constant velocity required during interpolation
// inputs should be normalized, meaning we could use conjugate instead of inverse here
// calculated via delta between vectors to the power of desired interpolation
inline Quat slerp(const Quat& start, const Quat& end, float t) {
    if (fabsf(dot(start, end)) > 1.0f - QUAT_EPSILON) {
        return nlerp(start, end, t);
    }
    Quat delta = inverse(start) * end;
    return normalized((delta ^ t) * start);
}

// Creates rotation for this direction
inline Quat lookRotation(const Vec3& direction, const Vec3& up) {
    // find orthonormal basis vectors
    Vec3 f = normalized(direction); // object forward
    Vec3 u = normalized(up); // desired up
    Vec3 r = cross(u, f); // object right
    u = cross(f, r); // object up

    // from world forward to object forward
    Quat worldToObject = fromTo(Vec3(0,0,1), f);

    // new object up direction
    Vec3 objectUp = worldToObject * Vec3(0,1,0);

    // new object up to desired up
    Quat u2u = fromTo(objectUp, u);

    // rotate forward direction then twist up
    Quat result = worldToObject * u2u;

    // and then normalize the result
    return normalized(result);
}

// Quaternion to rotation matrix
inline Mat4 quatToMat4(const Quat& q) {
    Vec3 r = q * Vec3(1, 0, 0);
    Vec3 u = q * Vec3(0, 1, 0);
    Vec3 f = q * Vec3(0, 0, 1);
    return Mat4(r.x, r.y, r.z, 0,
                u.x, u.y, u.z, 0,
                f.x, f.y, f.z, 0,
                0,   0,   0,   1);
}

// Rotation matrix to quaternion
inline Quat mat4ToQuat(const Mat4& m) {
    Vec3 up = normalized(Vec3(m.vectors.up.x, m.vectors.up.y, m.vectors.up.z));
    Vec3 forward = normalized(Vec3(m.vectors.forward.x, 
                                   m.vectors.forward.y, 
                                   m.vectors.forward.z));
    Vec3 right = cross(up, forward);
    up = cross(forward, right);
    return lookRotation(forward, up);
}

#endif

