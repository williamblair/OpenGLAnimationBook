#include <cstring>
#include <iostream>
#include <Pose.h>

Pose::Pose()
{
}

Pose::Pose(const Pose& p)
{
    *this = p;
}

Pose& Pose::operator=(const Pose& p)
{
    if (&p == this) {
        return *this;
    }

    parents.resize(p.parents.size());
    joints.resize(p.joints.size());

    if (p.parents.size() != 0) {
        memcpy(parents.data(), p.parents.data(), p.parents.size()*sizeof(int));
    }
    if (p.joints.size() != 0) {
        memcpy(joints.data(), p.joints.data(), p.joints.size()*sizeof(Transform));
    }

    return *this;
}

Pose::Pose(unsigned int numJoints)
{
    Resize(numJoints);
}

Transform Pose::GetGlobalTransform(unsigned int index)
{
    Transform result = joints[index];
    for (int p = parents[index]; p >= 0; p = parents[p]) {
        result = combine(joints[p], result);
    }
    return result;
}
DualQuaternion Pose::GetGlobalDualQuaternion(unsigned int index)
{
    DualQuaternion result = transformToDualQuat(joints[index]);
    for (int p = parents[index]; p >= 0; p = parents[p])
    {
        DualQuaternion parent = transformToDualQuat(joints[p]);
        // multiplication is left to right/reverse of matrix/vec multiplication
        result = result * parent;
    }

    return result;
}

Transform Pose::operator[](unsigned int index)
{
    return GetGlobalTransform(index);
}

void Pose::GetMatrixPalette(std::vector<Mat4>& out)
{
// unoptimized version
#if 0
    unsigned int size = GetSize();
    if (out.size() != size) {
        out.resize(size);
    }

    for (unsigned int i = 0; i < size; ++i)
    {
        Transform t = GetGlobalTransform(i);
        out[i] = transformToMat4(t);
    }
#endif

    int size = (int)GetSize();
    if ((int)out.size() != size) {
        out.resize(size);
    }
    
    int i = 0;
    for (; i < size; ++i) {
        int parent = parents[i];
        // this breaks ascending order, so the previous calculated results cannot be used
        if (parent > i) { 
            break;
        }
        
        Mat4 global = transformToMat4(joints[i]);
        if (parent >= 0) {
            global = out[parent] * global;
        }
        out[i] = global;
    }
    
    // will only run if we called the break above, meaning parent ascending order wasn't used
    // so fallback to the less efficient version
    for (; i < size; ++i) {
        Transform t = GetGlobalTransform(i);
        out[i] = transformToMat4(t);
    }
}

void Pose::GetDualQuaternionPalette(std::vector<DualQuaternion>& out)
{
    unsigned int size = GetSize();
    if (out.size() != size) {
        out.resize(size);
    }

    for (unsigned int i = 0; i < size; ++i) {
        out[i] = GetGlobalDualQuaternion(i);
    }
}


bool Pose::operator==(const Pose& other)
{
    if (other.joints.size() != joints.size() ||
        other.parents.size() != parents.size()) {
        return false;
    }

    unsigned int size = (unsigned int)joints.size();
    for (unsigned int i = 0; i < size; ++i) {
        Transform thisLocal = joints[i];
        Transform otherLocal = other.joints[i];

        int thisParent = parents[i];
        int otherParent = other.parents[i];

        if (thisLocal.position != otherLocal.position ||
            thisLocal.scale != otherLocal.scale ||
            thisLocal.rotation != otherLocal.rotation ||
            thisParent != otherParent) {
            return false;
        }
    }

    return true;
}

bool Pose::operator!=(const Pose& other)
{
    return !(*this == other);
}

// returns true if the search node is a descendant of the given root node
bool IsInHierarchy(Pose& pose, unsigned int root, unsigned int search)
{
    if (search == root) {
        return true;
    }
    int p = pose.GetParent(search);
    while (p >= 0) {
        if (p == (int)root) {
            return true;
        }
        p = pose.GetParent(p);
    }
    
    return false;
}

void Blend(Pose& output, Pose& a, Pose& b, float t, int root)
{
    unsigned int numJoints = output.GetSize();
    for (unsigned int i = 0; i < numJoints; ++i) {
        if (root >= 0) {
            // don't blend if they aren't within the same hierarchy
            if (!IsInHierarchy(output, root, i)) {
                continue;
            }
        }
        
        output.SetLocalTransform(i, 
                                 mix(a.GetLocalTransform(i), b.GetLocalTransform(i), t));
    }
}
