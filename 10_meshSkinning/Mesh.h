#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <vector>
#include <Vec2.h>
#include <Vec3.h>
#include <Vec4.h>
#include <Mat4.h>
#include <Attribute.h>
#include <Skeleton.h>
#include <IndexBuffer.h>
#include <Draw.h>

class Mesh
{
public:
    Mesh();
    Mesh(const Mesh&);
    Mesh& operator=(const Mesh&);
    ~Mesh();

    inline std::vector<Vec3>& GetPositions()       { return positions;  }
    inline std::vector<Vec3>& GetNormals()         { return normals;    }
    inline std::vector<Vec2>& GetTexCoords()       { return texCoords;  }
    inline std::vector<Vec4>& GetWeights()         { return weights;    }
    inline std::vector<iVec4>& GetInfluences()     { return influences; }
    inline std::vector<unsigned int>& GetIndices() { return indices;    }
    
    // applies CPU skinning
    void CPUSkin(Skeleton& skeleton, Pose& pose);
    // sends CPU side data changes to the GPU attributes
    void UpdateOpenGLBuffers();
    // inputs are attribute locations
    void Bind(int position, int normal, int texCoord, int weight, int influence);
    void Draw();
    void DrawInstanced(unsigned int numInstances);
    void Unbind(int position, int normal, int texCoord, int weight, int influence);

protected:
    
    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> texCoords;
    std::vector<Vec4> weights;
    std::vector<iVec4> influences;
    std::vector<unsigned int> indices;
    
    Attribute<Vec3>* posAttrib;
    Attribute<Vec3>* normAttrib;
    Attribute<Vec2>* uvAttrib;
    Attribute<Vec4>* weightAttrib;
    Attribute<iVec4>* influenceAttrib;
    IndexBuffer* indexBuffer;
    
    std::vector<Vec3> skinnedPositions;
    std::vector<Vec3> skinnedNormals;
    std::vector<Mat4> posePalette;
};

#endif // MESH_H_INCLUDED
