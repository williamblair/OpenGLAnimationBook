#include <Transform.h>
#include <cstring>
#include <cstdio>

inline void printMat4( Mat4& m )
{
    float* f = m.v;
    printf("%f,%f,%f,%f\n"
           "%f,%f,%f,%f\n"
           "%f,%f,%f,%f\n"
           "%f,%f,%f,%f\n",
        f[0], f[1], f[2], f[3],
        f[4], f[5], f[6], f[7],
        f[8], f[9], f[10], f[11],
        f[12], f[13], f[14], f[15]);
}

inline void printTransform( Transform& t )
{
    float* p = t.position.v;
    float* r = t.rotation.v;
    float* s = t.scale.v;
    printf("Position: %f,%f,%f\n", p[0], p[1], p[2]);
    printf("Rotation: %f,%f,%f,%f\n", r[0], r[1], r[2], r[3]);
    printf("Scale: %f,%f,%f\n", s[0], s[1], s[2]);
}

int main()
{
    Transform t1(
        Vec3(1.0f, 2.0f, 3.0f), // position
        /*angleAxis(45.0f, Vec3(0.1f, 0.2f, 0.7f)), // rotation*/
        Quat(-0.048717f,-0.097435f,-0.341022f,-0.873305f),
        Vec3(1.0f, 2.0f, 3.0f) // scale
    );
    float* quatPtr = t1.rotation.v;
    printf("Transform:\n");
    printTransform(t1);
    Mat4 mat = transformToMat4( t1 );
    printf("Mat:\n");
    printMat4( mat );
    Transform t2 = mat4ToTransform( mat );
    printf("New transform:\n");
    printTransform( t2 );

    return 0;
}

