#include <CCDSolver.h>

CCDSolver::CCDSolver() :
    numSteps(15),
    threshold(0.00001f)
{
}

Transform CCDSolver::GetGlobalTransform(unsigned int index)
{
    unsigned int size = IKChain.size();
    Transform world = IKChain[index];
    for (int i = (int)index - 1; i >= 0; --i) {
        world = combine(IKChain[i], world);
    }
    return world;
}

bool CCDSolver::Solve(const Transform& target)
{
    unsigned int size = IKChain.size();
    if (size == 0) { return false; }
    unsigned int last = size - 1;
    float threshSq = threshold * threshold;
    Vec3 goal = target.position;

    for (unsigned int i = 0; i < numSteps; ++i)
    {
        Vec3 effector = GetGlobalTransform(last).position;

        // end early if close enough to goal
        if (lenSq(goal - effector) < threshSq) {
            return true;
        }

        for (int j = (int)size - 2; j >= 0; --j) {
            effector = GetGlobalTransform(last).position;
            
            Transform world = GetGlobalTransform(j);
            Vec3 position = world.position;
            Quat rotation = world.rotation;

            Vec3 toEffector = effector - position;
            Vec3 toGoal = goal - position;

            Quat effectorToGoal;
            if (lenSq(toGoal) > 0.00001f) {
                effectorToGoal = fromTo(toEffector, toGoal);
            }

            // transform joint into world space, then by inverse of previous
            // joint world space to get back into joint space
            Quat worldRotated = rotation * effectorToGoal;
            Quat localRotate = worldRotated * inverse(rotation);
            IKChain[j].rotation = localRotate * IKChain[j].rotation;

            // check for completion
            effector = GetGlobalTransform(last).position;
            if (lenSq(goal - effector) < threshSq) {
                return true;
            }
        }
    }

    // didn' reach close enough
    return false;
}

