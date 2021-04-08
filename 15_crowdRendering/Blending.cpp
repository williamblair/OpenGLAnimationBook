#include <Blending.h>
#include <Transform.h>

Pose MakeAdditivePose(Skeleton& skeleton, Clip& clip)
{
    Pose result = skeleton.GetRestPose();
    clip.Sample(result, clip.GetStartTime());
    return result;
}

void Add(Pose& output, Pose& inPose, Pose& addPose, Pose& additiveBasePose, int blendroot)
{
    unsigned int numJoints = addPose.GetSize();
    for (int i = 0; i < numJoints; ++i) {
        Transform input = inPose.GetLocalTransform(i);
        Transform additive = addPose.GetLocalTransform(i);
        Transform additiveBase = additiveBasePose.GetLocalTransform(i);
        
        if (blendroot >= 0 && !IsInHierarchy(addPose, blendroot, i)) {
            continue;
        }
        
        // outpose = inpose + (addpose - addbasepose)
        Transform result(input.position + (additive.position - additiveBase.position),
                        normalized(input.rotation * (inverse(additiveBase.rotation) * additive.rotation)),
                        input.scale + (additive.scale - additiveBase.scale));
        output.SetLocalTransform(i, result);
    }
}
