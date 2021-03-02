#include "Mesh.h"

Mesh::Mesh()
{
    posAttrib = new Attribute<Vec3>();
    normAttrib = new Attribute<Vec3>();
    uvAttrib = new Attribute<Vec2>();
    weightAttrib = new Attribute<Vec4>();
    influenceAttrib = new Attribute<iVec4>();
    indexBuffer = new IndexBuffer();
}

Mesh::~Mesh()
{
    delete posAttrib;
    delete normAttrib;
    delete uvAttrib;
    delete weightAttrib;
    delete influenceAttrib;
    delete indexBuffer;
}

Mesh::Mesh(const Mesh& other)
{
    posAttrib = new Attribute<Vec3>();
    normAttrib = new Attribute<Vec3>();
    uvAttrib = new Attribute<Vec2>();
    weightAttrib = new Attribute<Vec4>();
    influenceAttrib = new Attribute<iVec4>();
    indexBuffer = new IndexBuffer();
    *this = other;
}

Mesh& Mesh::operator=(const Mesh& other)
{
    if (this == &other) {
        return *this;
    }
    
    positions = other.positions;
    normals = other.normals;
    texCoords = other.texCoords;
    weights = other.weights;
    influences = other.influences;
    indices = other.indices;
    UpdateOpenGLBuffers();
    return *this;
}

void Mesh::UpdateOpenGLBuffers()
{
    if (positions.size() > 0) {
        posAttrib->Set(positions);
    }
    if (normals.size() > 0) {
        normAttrib->Set(normals);
    }
    if (texCoords.size() > 0) {
        uvAttrib->Set(texCoords);
    }
    if (weights.size() > 0) {
        weightAttrib->Set(weights);
    }
    if (influences.size() > 0) {
        influenceAttrib->Set(influences);
    }
    if (indices.size() > 0) {
        indexBuffer->Set(indices);
    }
}

void Mesh::Bind(int position, int normal, int texCoord, int weight, int influence)
{
    if (position >= 0) {
        posAttrib->BindTo(position);
    }
    if (normal >= 0) {
        normAttrib->BindTo(normal);
    }
    if (texCoord >= 0) {
        uvAttrib->BindTo(texCoord);
    }
    if (weight >= 0) {
        weightAttrib->BindTo(weight);
    }
    if (influence >= 0) {
        influenceAttrib->BindTo(influence);
    }
}

void Mesh::Draw()
{
    if (indices.size() > 0) {
        ::Draw(*indexBuffer, DrawMode::Triangles);
    } else {
        ::Draw(positions.size(), DrawMode::Triangles);
    }
}

void Mesh::DrawInstanced(unsigned int numInstances)
{
    if (indices.size() > 0) {
        ::DrawInstanced(*indexBuffer, DrawMode::Triangles, numInstances);
    } else {
        ::DrawInstanced(positions.size(), DrawMode::Triangles, numInstances);
    }
}

void Mesh::Unbind(int position, int normal, int texCoord, int weight, int influence)
{
    if (position >= 0) {
        posAttrib->UnbindFrom(position);
    }
    if (normal >= 0) {
        normAttrib->UnbindFrom(normal);
    }
    if (texCoord >= 0) {
        uvAttrib->UnbindFrom(texCoord);
    }
    if (weight >= 0) {
        weightAttrib->UnbindFrom(weight);
    }
    if (influence >= 0) {
        influenceAttrib->UnbindFrom(influence);
    }
}

// implementation using Transform objects
#if 0
void Mesh::CPUSkin(Skeleton& skeleton, Pose& pose)
{
    unsigned int numVerts = positions.size();
    if (numVerts == 0) { 
        // TODO error
        return;
    }
    
    skinnedPositions.resize(numVerts);
    skinnedNormals.resize(numVerts);
    Pose& bindPose = skeleton.GetBindPose();
    
    for (unsigned int i = 0; i < numVerts; ++i) {
        iVec4& joint = influences[i];
        Vec4& weight = weights[i];
        
        // calculate skin transform
        Transform skin0 = combine(pose[joint.x],
                                  inverse(bindPose[joint.x]));
        Vec3 p0 = transformPoint(skin0, positions[i]);
        Vec3 n0 = transformVector(skin0, normals[i]);
        
        Transform skin1 = combine(pose[joint.y],
                                  inverse(bindPose[joint.y]));
        Vec3 p1 = transformPoint(skin1, positions[i]);
        Vec3 n1 = transformVector(skin1, normals[i]);
        
        Transform skin2 = combine(pose[joint.z],
                                  inverse(bindPose[joint.z]));
        Vec3 p2 = transformPoint(skin2, positions[i]);
        Vec3 n2 = transformVector(skin2, normals[i]);
        
        Transform skin3 = combine(pose[joint.w],
                                  inverse(bindPose[joint.w]));
        Vec3 p3 = transformPoint(skin3, positions[i]);
        Vec3 n3 = transformVector(skin3, normals[i]);
        
        // combine transforms
        skinnedPositions[i] = p0 * weight.x + 
                              p1 * weight.y + 
                              p2 * weight.z + 
                              p3 * weight.w;
        skinnedNormals[i] = n0 * weight.x + 
                            n1 * weight.y + 
                            n2 * weight.z + 
                            n3 * weight.w;
    }
    
    posAttrib->Set(skinnedPositions);
    normAttrib->Set(skinnedNormals);
}
#endif

// implementation using matrices
// this way is more easier to convert to GPU shader code
void Mesh::CPUSkin(Skeleton& skeleton, Pose& pose)
{
    unsigned int numVerts = positions.size();
    if (numVerts == 0) { 
        // TODO error
        return;
    }
    
    skinnedPositions.resize(numVerts);
    skinnedNormals.resize(numVerts);
    
    // stores pose transform matrices in posePalette
    pose.GetMatrixPalette(posePalette);
    std::vector<Mat4> invPosePalette = skeleton.GetInvBindPose();
    
    for (unsigned int i = 0; i < numVerts; ++i) {
        iVec4& joint = influences[i];
        Vec4& weight = weights[i];
        
        // calculate the skin matrix
        // up to 4 different joint influences (not all necessarily used)
        Mat4 m0 = (posePalette[joint.x] * invPosePalette[joint.x]) * weight.x; // first joint influence
        Mat4 m1 = (posePalette[joint.y] * invPosePalette[joint.y]) * weight.y; // second joint influence
        Mat4 m2 = (posePalette[joint.z] * invPosePalette[joint.z]) * weight.z; // third joint influence
        Mat4 m3 = (posePalette[joint.w] * invPosePalette[joint.w]) * weight.w; // fourth joint influence
        Mat4 skin = m0 + m1 + m2 + m3;
        
        // combine transforms
        skinnedPositions[i] = transformPoint(skin, positions[i]);
        skinnedNormals[i] = transformVector(skin, normals[i]);
    }
    
    posAttrib->Set(skinnedPositions);
    normAttrib->Set(skinnedNormals);
}