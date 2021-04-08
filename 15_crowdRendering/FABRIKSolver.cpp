#include <FABRIKSolver.h>

FABRIKSolver::FABRIKSolver() :
    numSteps(15),
    threshold(0.00001f)
{}

Transform FABRIKSolver::GetGlobalTransform(unsigned int index)
{
    unsigned int size = IKChain.size();
    Transform world = IKChain[index];
    for (int i = (int)index - 1; i >= 0; --i) {
        world = combine(IKChain[i], world);
    }

    return world;
}

void FABRIKSolver::IKChainToWorld()
{
    unsigned int size = IKChain.size();
    for (unsigned int i = 0; i < size; ++i) {
        Transform world = GetGlobalTransform(i);
        worldChain[i] = world.position;

        if (i >= 1) {
            Vec3 prev = worldChain[i - 1];
            lengths[i] = len(world.position - prev);
        }
    }
    if (size >= 0) {
        lengths[0] = 0.0f; // difference between root joint and itself
    }
}

void FABRIKSolver::WorldToIKChain()
{
    unsigned int size = IKChain.size();
    if (size == 0) { return; }
    
    for (unsigned int i = 0; i < size - 1; ++i)
    {
        Transform world = GetGlobalTransform(i);
        Transform next = GetGlobalTransform(i + 1);
        
        Vec3 position = world.position;
        Quat rotation = world.rotation;

        Vec3 toNext = next.position - position;
        toNext = inverse(rotation) * toNext;

        Vec3 toDesired = worldChain[i + 1] - position;
        toDesired = inverse(rotation) * toDesired;

        Quat delta = fromTo(toNext, toDesired);
        IKChain[i].rotation = delta * IKChain[i].rotation;
    }
}

void FABRIKSolver::IterateBackward(const Vec3& goal)
{
    int size = (int)IKChain.size();
    if (size > 0) {
        worldChain[size - 1] = goal;
    }

    for (int i = size - 2; i >= 0; --i) {
        Vec3 direction = normalized(worldChain[i] - worldChain[i+1]);
        Vec3 offset = direction * lengths[i+1];
        worldChain[i] = worldChain[i + 1] + offset;
    }
}

void FABRIKSolver::IterateForward(const Vec3& base)
{
    unsigned int size = IKChain.size();
    if (size > 0) {
        worldChain[0] = base;
    }

    for (int i = 1; i < size; ++i) {
        Vec3 direction = normalized(worldChain[i] - worldChain[i-1]);
        Vec3 offset = direction * lengths[i];
        worldChain[i] = worldChain[i - 1] + offset;
    }
}

bool FABRIKSolver::Solve(const Transform& target)
{
    unsigned int size = IKChain.size();
    if (size == 0) { return false; }
    unsigned int last = size - 1;
    float threshSqd = threshold * threshold;

    IKChainToWorld();
    Vec3 goal = target.position;
    Vec3 base = worldChain[0];

    for (unsigned int i = 0; i < numSteps; ++i)
    {
        Vec3 effector = worldChain[last];
        if (lenSq(effector - goal) < threshSqd) {
            WorldToIKChain();
            return true;
        }
        
        IterateBackward(goal);
        IterateForward(base);
    }
    WorldToIKChain();
    Vec3 effector = GetGlobalTransform(last).position;
    if (lenSq(effector - goal) < threshSqd) {
        return true;
    }

    return false;
}

#if 0
void FABRIKSolver::ApplyBallSocketConstraint(int i, float limit)
{
    Quat parentRot = (i == 0) ? offset.rotation : GetWorldTransform(i - 1).rotation;
    Quat thisRot = GetWorldTransform(i).rotation;

    Vec3 parentDir = parentRot * Vec3(0,0,1);
    Vec3 thisDir = thisRot * Vec3(0,0,1);
    float angle = ::angle(parentDir, thisDir);

#define DEG2RAD(deg) ((deg) * M_PI / 180.0f)

    // if angle surpassed the socket constraint, hold back
    if (angle > DEG2RAD(limit)) {
        Vec3 correction = cross(parentDir, thisDir);
        Quat worldSpaceRotation = parentRot * angleAxis(DEG2RAD(limit), correction);
        chain[i].rotation = worldSpaceRotation * inverse(parentRot);
    }
#undef DEG2RAD
}
#endif

#if 0
void FABRIKSolver::ApplyHingeSocketConstraint(int i, Vec3 axis)
{
    Transform joint = GetWorldTransform(i);
    Transform parent = GetWorldTransform(i - 1);

    Vec3 currentHinge = joint.rotation * axis;
    Vec3 desiredHinge = parent.rotation * axis;

    chain[i].rotation = chain[i].rotation * fromToRotation(currentHinge, desiredHinge);
}
#endif

