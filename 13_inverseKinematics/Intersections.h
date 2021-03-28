#ifndef INTERSECTIONS_H_INCLUDED
#define INTERSECTIONS_H_INCLUDED

#include <Vec3.h>
#include <Mesh.h>
#include <cmath>
#include <vector>

struct Ray
{
    Vec3 origin;
    Vec3 direction;

    inline Ray() : direction(Vec3(0, -1, 0)){}
    inline Ray(const Vec3& o) : origin(o), direction(Vec3(0, -1, 0)) {}
    inline Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d) {}
};

struct Triangle
{
    Vec3 v0;
    Vec3 v1;
    Vec3 v2;
    Vec3 normal;

    inline Triangle() {}
    inline Triangle(const Vec3& inV0,
                    const Vec3& inV1,
                    const Vec3& inV2) :
        v0(inV0),
        v1(inV1),
        v2(inV2)
    {
        normal = normalized(cross(v1 - v0, v2 - v0));
    }
};

bool RaycastTriangle(const Ray& ray, const Triangle& triangle, Vec3& hitPoint);
std::vector<Triangle> MeshToTriangles(Mesh& mesh);
std::vector<Triangle> MeshesToTriangles(std::vector<Mesh>& mesh);

#endif // INTERSECTIONS_H_INCLUDED

