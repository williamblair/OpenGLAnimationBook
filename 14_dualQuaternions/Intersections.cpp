#include <Intersections.h>

bool RaycastTriangle(const Ray& ray, const Triangle& triangle, Vec3& hitPoint)
{
    const float EPSILON = 0.0000001f;
    Vec3 v0 = triangle.v0;
    Vec3 v1 = triangle.v1;
    Vec3 v2 = triangle.v2;
    
    Vec3 rayVector = ray.direction;
    Vec3 rayOrigin = ray.origin;

    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;

    Vec3 h = cross(rayVector, edge2);
    float a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON) {
        return false;
    }
    
    float f = 1.0f / a;
    
    Vec3 s = rayOrigin - v0;
    float u = f * dot(s, h);
    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    Vec3 q = cross(s, edge1);
    float v = f * dot(rayVector, q);
    if (v < 0.0f || u + v > 1.0f)  {
        return false;
    }

    float t = f * dot(edge2, q);
    if (t > EPSILON) {
        hitPoint = rayOrigin + rayVector * t;
        return true;
    }

    return false;
}

std::vector<Triangle> MeshToTriangles(Mesh& mesh)
{
    std::vector<Triangle> result;
    std::vector<Vec3> vertices = mesh.GetPositions();
    std::vector<unsigned int> indices = mesh.GetIndices();
    if (indices.size() == 0) {
        unsigned int numVertices = (unsigned int)vertices.size();
        for (unsigned int i = 0; i < numVertices; i += 3) {
            result.push_back(Triangle(vertices[i + 0],
                                      vertices[i + 1],
                                      vertices[i + 2]));
        }
    } else {
        unsigned int numIndices = (unsigned int)indices.size();
        for (unsigned int i = 0; i < numIndices; i += 3) {
            result.push_back(Triangle(vertices[indices[i + 0]],
                                      vertices[indices[i + 1]],
                                      vertices[indices[i + 2]]));
        }
    }

    return result;
}

std::vector<Triangle> MeshesToTriangles(std::vector<Mesh>& meshes)
{
    std::vector<Triangle> result;
    unsigned int numMeshes = (unsigned int)meshes.size();
    for (unsigned int j = 0; j < numMeshes; ++j)
    {
        Mesh& mesh = meshes[j];
        std::vector<Vec3> vertices = mesh.GetPositions();
        std::vector<unsigned int> indices = mesh.GetIndices();
        
        if (indices.size() == 0) {
            unsigned int numVertices = (unsigned int)vertices.size();
            for (unsigned int i = 0; i < numVertices; i += 3) {
                result.push_back(Triangle(vertices[i + 0],
                                          vertices[i + 1],
                                          vertices[i + 2]));
            }
        } else {
            unsigned int numIndices = (unsigned int)indices.size();
            for (unsigned int i = 0; i < numIndices; i += 3) {
                result.push_back(Triangle(vertices[indices[i + 0]],
                                          vertices[indices[i + 1]],
                                          vertices[indices[i + 2]]));
            }
        }
    }

    return result;
}

