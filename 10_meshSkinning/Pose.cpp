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

Transform Pose::operator[](unsigned int index)
{
    return GetGlobalTransform(index);
}

void Pose::GetMatrixPalette(std::vector<Mat4>& out)
{
    unsigned int size = GetSize();
    if (out.size() != size) {
        out.resize(size);
    }

    for (unsigned int i = 0; i < size; ++i)
    {
        Transform t = GetGlobalTransform(i);
        out[i] = transformToMat4(t);
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

