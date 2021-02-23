#ifndef MAT4_H_INCLUDED
#define MAT4_H_INCLUDED

#include <Vec3.h>
#include <Vec4.h>

#define MAT4_EPSILON 0.000001f

struct Mat4
{
    union {
        float v[16];
        struct {
            Vec4 right;     // first column
            Vec4 up;        // second column
            Vec4 forward;   // third column
            Vec4 position;  // fourth column
        } vectors;
        // basis vector notation
        struct {
                        // row 1    row 2       row 3       row 4
            /* col 1 */ float xx;   float xy;   float xz;   float xw; // right
            /* col 2 */ float yx;   float yy;   float yz;   float yw; // up
            /* col 3 */ float zx;   float zy;   float zz;   float zw; // forward
            /* col 4 */ float tx;   float ty;   float tz;   float tw; // position
        };
        // column-row notation
        struct {
            float c0r0; float c0r1; float c0r2; float c0r3;
            float c1r0; float c1r1; float c1r2; float c1r3;
            float c2r0; float c2r1; float c2r2; float c2r3;
            float c3r0; float c3r1; float c3r2; float c3r3;
        };
        // row-column notation
        struct {
            float r0c0; float r1c0; float r2c0; float r3c0;
            float r0c1; float r1c1; float r2c1; float r3c1;
            float r0c2; float r1c2; float r2c2; float r3c2;
            float r0c3; float r1c3; float r2c3; float r3c3;
        };
    };

    // defaults to identity matrix
    inline Mat4() :
        xx(1), xy(0), xz(0), xw(0),
        yx(0), yy(1), yz(0), yw(0),
        zx(0), zy(0), zz(1), zw(0),
        tx(0), ty(0), tz(0), tw(1)
    {}

    inline Mat4(float* fv) :
        xx(fv[0]),  xy(fv[1]),  xz(fv[2]),  xw(fv[3]),
        yx(fv[4]),  yy(fv[5]),  yz(fv[6]),  yw(fv[7]),
        zx(fv[8]),  zy(fv[9]),  zz(fv[10]), zw(fv[11]),
        tx(fv[12]), ty(fv[13]), tz(fv[14]), tw(fv[15])
    {}

    inline Mat4(float inXX, float inXY, float inXZ, float inXW,
                float inYX, float inYY, float inYZ, float inYW,
                float inZX, float inZY, float inZZ, float inZW,
                float inTX, float inTY, float inTZ, float inTW) :
        xx(inXX), xy(inXY), xz(inXZ), xw(inXW),
        yx(inYX), yy(inYY), yz(inYZ), yw(inYW),
        zx(inZX), zy(inZY), zz(inZZ), zw(inZW),
        tx(inTX), ty(inTY), tz(inTZ), tw(inTW)
    {}
};

// comparison
bool operator==(const Mat4& lhs, const Mat4& rhs) {
    for (int i = 0; i < 16; i++) {
        if (fabsf(lhs.v[i] - rhs.v[i]) > MAT4_EPSILON) {
            return false;
        }
    }
    return true;
}
bool operator!=(const Mat4& lhs, const Mat4& rhs) {
    return !(lhs == rhs);
}

// component-wise operations
inline Mat4 operator+(const Mat4& lhs, const Mat4& rhs) {
    return Mat4(
        lhs.xx + rhs.xx, lhs.xy + rhs.xy, lhs.xz + rhs.xz, lhs.xw + rhs.xw, 
        lhs.yx + rhs.yx, lhs.yy + rhs.yy, lhs.yz + rhs.yz, lhs.yw + rhs.yw,
        lhs.zx + rhs.zx, lhs.zy + rhs.zy, lhs.zz + rhs.zz, lhs.zw + rhs.zw,
        lhs.tx + rhs.tx, lhs.ty + rhs.ty, lhs.tz + rhs.tz, lhs.tw + rhs.tw
    );
}
inline Mat4 operator*(const Mat4& m, float f) {
    return Mat4(m.xx * f, m.xy * f, m.xz * f, m.xw * f, 
                m.yx * f, m.yy * f, m.yz * f, m.yw * f,
                m.zx * f, m.zy * f, m.zz * f, m.zw * f,
                m.tx * f, m.ty * f, m.tz * f, m.tw * f);
}

// dot product on the row and column of matrices a and b
static inline float 
M4D(const Mat4& a, const int aRow, const Mat4& b, const int bCol) {
    return a.v[0 * 4 + aRow] * b.v[bCol * 4 + 0] +
           a.v[1 * 4 + aRow] * b.v[bCol * 4 + 1] +
           a.v[2 * 4 + aRow] * b.v[bCol * 4 + 2] +
           a.v[3 * 4 + aRow] * b.v[bCol * 4 + 3];

}
inline Mat4 operator*(const Mat4& a, const Mat4& b) {
    return Mat4(M4D(a,0,b,0), M4D(a,1,b,0), M4D(a,2,b,0), M4D(a,3,b,0),
                M4D(a,0,b,1), M4D(a,1,b,1), M4D(a,2,b,1), M4D(a,3,b,1),
                M4D(a,0,b,2), M4D(a,1,b,2), M4D(a,2,b,2), M4D(a,3,b,2),
                M4D(a,0,b,3), M4D(a,1,b,3), M4D(a,2,b,3), M4D(a,3,b,3));
}

// Vector multiplications
static inline float
M4V4D(const Mat4& m, const int mRow, 
      const float x, const float y, const float z, const float w) {
    return x * m.v[0 * 4 + mRow] +
           y * m.v[1 * 4 + mRow] +
           z * m.v[2 * 4 + mRow] +
           w * m.v[3 * 4 + mRow];
}
Vec4 operator*(const Mat4& m, const Vec4& v) {
    return Vec4(M4V4D(m, 0, v.x, v.y, v.z, v.w),
                M4V4D(m, 1, v.x, v.y, v.z, v.w),
                M4V4D(m, 2, v.x, v.y, v.z, v.w),
                M4V4D(m, 3, v.x, v.y, v.z, v.w));
}

inline Vec3 transformVector(const Mat4& m, const Vec3& v) {
    return Vec3(M4V4D(m, 0, v.x, v.y, v.z, 0.0f),
                M4V4D(m, 1, v.x, v.y, v.z, 0.0f),
                M4V4D(m, 2, v.x, v.y, v.z, 0.0f));
}
inline Vec3 transformPoint(const Mat4& m, const Vec3& v) {
    return Vec3(M4V4D(m, 0, v.x, v.y, v.z, 1.0f),
                M4V4D(m, 1, v.x, v.y, v.z, 1.0f),
                M4V4D(m, 2, v.x, v.y, v.z, 1.0f));
}
inline Vec3 transformPoint(const Mat4& m, const Vec3& v, float& w) {
    float tempW = w;
    w = M4V4D(m, 3, v.x, v.y, v.z, tempW);
    return Vec3(M4V4D(m, 0, v.x, v.y, v.z, tempW),
                M4V4D(m, 1, v.x, v.y, v.z, tempW),
                M4V4D(m, 2, v.x, v.y, v.z, tempW));
}

// internal matrix operations
inline void transpose(Mat4& m) {
    float temp;
    #define M4SWAP(x, y) temp = y; y = x; x = temp
    M4SWAP(m.yx, m.xy);
    M4SWAP(m.zx, m.xz);
    M4SWAP(m.tx, m.xw);
    M4SWAP(m.zy, m.yz);
    M4SWAP(m.ty, m.yw);
    M4SWAP(m.tz, m.zw);
    #undef M4SWAP
}
inline Mat4 transposed(const Mat4& m) {
    return Mat4(m.xx, m.yx, m.zx, m.tx,
                m.xy, m.yy, m.zy, m.ty,
                m.xz, m.yz, m.zz, m.tz,
                m.xw, m.yw, m.zw, m.tw);
}

// minor determinant of a 3x3 matrix using the given rows and cols
#define M4_3X3MINOR(x, c0, c1, c2, r0, r1, r2) \
    (x[c0*4 + r0] *                                                   \
        (x[c1*4 + r1] * x[c2*4 + r2] - x[c1*4 + r2] * x[c2*4 + r1]) - \
     x[c1*4 + r0] *                                                   \
        (x[c0*4 + r1] * x[c2*4 + r2] - x[c0*4 + r2] * x[c2*4 + r1]) + \
     x[c2*4 + r0] *                                                   \
        (x[c0*4 + r1] * x[c1*4 + r2] - x[c0*4 + r2] * x[c1*4 + r1]))

inline float determinant(const Mat4& m) {
    return m.v[0] * M4_3X3MINOR(m.v,  1,2,3, 1,2,3) -
           m.v[4] * M4_3X3MINOR(m.v,  0,2,3, 1,2,3) +
           m.v[8] * M4_3X3MINOR(m.v,  0,1,3, 1,2,3) -
           m.v[12] * M4_3X3MINOR(m.v, 0,1,2, 1,2,3);
}
inline Mat4 adjugate(const Mat4& m) {
    // cofactor(M[i,j]) = Minor(M[i],j] * pow(-1, i+j))
    Mat4 cofactor;
    cofactor.v[0] = M4_3X3MINOR(m.v, 1,2,3, 1,2,3);
    cofactor.v[1] = M4_3X3MINOR(m.v, 1,2,3, 0,2,3);
    cofactor.v[2] = M4_3X3MINOR(m.v, 1,2,3, 0,1,3);
    cofactor.v[3] = M4_3X3MINOR(m.v, 1,2,3, 0,1,2);
    cofactor.v[4] = M4_3X3MINOR(m.v, 0,2,3, 1,2,3);
    cofactor.v[5] = M4_3X3MINOR(m.v, 0,2,3, 0,2,3);
    cofactor.v[6] = M4_3X3MINOR(m.v, 0,2,3, 0,1,3);
    cofactor.v[7] = M4_3X3MINOR(m.v, 0,2,3, 0,1,2);
    cofactor.v[8] = M4_3X3MINOR(m.v, 0,1,3, 1,2,3);
    cofactor.v[9] = M4_3X3MINOR(m.v, 0,1,3, 0,2,3);
    cofactor.v[10] = M4_3X3MINOR(m.v, 0,1,3, 0,1,3);
    cofactor.v[11] = M4_3X3MINOR(m.v, 0,1,3, 0,1,2);
    cofactor.v[12] = M4_3X3MINOR(m.v, 0,1,2, 1,2,3);
    cofactor.v[13] = M4_3X3MINOR(m.v, 0,1,2, 0,2,3);
    cofactor.v[14] = M4_3X3MINOR(m.v, 0,1,2, 0,1,3);
    cofactor.v[15] = M4_3X3MINOR(m.v, 0,1,2, 0,1,2);
    return transposed(cofactor);
}
inline Mat4 inverse(const Mat4& m) {
    float det = determinant(m);
    if (det == 0.0f) {
        // TODO error
        return Mat4();
    }
    Mat4 adj = adjugate(m);
    return adj * (1.0f / det);
}
inline void invert(Mat4& m) {
    float det = determinant(m);
    if (det == 0.0f) {
        // TODO error
        m = Mat4();
    }
    m = adjugate(m) * (1.0f / det);
}

// transformation matrices
// http://www.songho.ca/opengl/gl_projectionmatrix.html
inline Mat4 frustum(float l, // left
                    float r, // right
                    float b, // bottom
                    float t, // top
                    float n, // near
                    float f) // far
{
    if (l == r || t == b || n == f) {
        // TODO error
        return Mat4();
    }
    return Mat4(
        (2.0f*n) / (r-l),   0,                0,                 0,
        0,                  (2.0f*n) / (t-b), 0,                 0,
        (r+l) / (r-l),      (t+b) / (t-b),    (-(f+n)) / (f-n), -1,
        0,                  0,                (-2.0f * f * n) / (f-n), 0
    );
}

inline Mat4 perspective(float fovDegrees, 
                        float aspect, 
                        float near, 
                        float far)
{
    float yMax = near * tanf(fovDegrees * 3.14159265359f / 360.0f);
    float xMax = yMax * aspect;
    return frustum(-xMax, xMax, -yMax, yMax, near, far);
}

inline Mat4 ortho(float l,
                  float r,
                  float b,
                  float t,
                  float n, 
                  float f)
{
    if (l == r || t == b || n == f) {
        // TODO error
        return Mat4();
    }
    return Mat4(
        2.0f / (r-l),   0,                0,                 0,
        0,              2.0f / (t-b),     0,                 0,
        0,              0,                -2.0f / (f-n),     0,
        -((r+l)/(r-l)), -((t+b)/(t-b)),   -((f+n)/(f-n)),    1
    );
}

inline Mat4 lookAt(const Vec3& position,
                   const Vec3& target, 
                   const Vec3& up) {

    Vec3 f = normalized(target - position) * -1.0f; // forward
    Vec3 r = cross(up, f); // right
    if (r == Vec3(0,0,0)) {
        // TODO error
        return Mat4();
    }
    normalize(r);
    Vec3 u = normalized(cross(f, r)); // right handed coords; up
    Vec3 t = Vec3(-dot(r, position),
                  -dot(u, position),
                  -dot(f, position));

    // orthonormal basis vectors inverse equals its transpose
    return Mat4(r.x, u.x, f.x, 0,
                r.y, u.y, f.y, 0,
                r.z, u.z, f.z, 0,
                t.x, t.y, t.z, 1);
}

#endif // MAT4_H_INCLUDED
