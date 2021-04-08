#include <Skeleton.h>

Skeleton::Skeleton()
{
}

Skeleton::Skeleton(const Pose& rest, const Pose& bind, const std::vector<std::string>& names)
{
    Set(rest, bind, names);
}

Skeleton::~Skeleton()
{
}

void Skeleton::Set(const Pose& rest, const Pose& bind, const std::vector<std::string>& names)
{
    restPose = rest;
    bindPose = bind;
    jointNames = names;
    UpdateInverseBindPose();
}

void Skeleton::UpdateInverseBindPose()
{
    unsigned int size = bindPose.GetSize();
    invBindPose.resize(size);
    
    for (unsigned int i = 0; i < size; ++i)
    {
        Transform world = bindPose.GetGlobalTransform(i);
        invBindPose[i] = inverse(transformToMat4(world));
    }
}

void Skeleton::GetInvBindPose(std::vector<DualQuaternion>& out)
{
    if (out.size() != invBindPose.size()) {
        out.resize(invBindPose.size());
    }

    for (unsigned int i = 0; i < out.size(); ++i)
    {
        DualQuaternion world = bindPose.GetGlobalDualQuaternion(i);
        out[i] = conjugate(world);
    }
}

