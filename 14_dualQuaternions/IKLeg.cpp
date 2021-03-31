#include <IKLeg.h>

IKLeg::IKLeg(Skeleton& skeleton, 
             const std::string& hip, 
             const std::string& knee, 
             const std::string& ankle, 
             const std::string& toe)
{
    lineVisuals = new DebugDraw();
    pointVisuals = new DebugDraw();

    solver.Resize(3);
    pointVisuals->Resize(3);
    lineVisuals->Resize(4);

    ankleToGroundOffset = 0.0f;

    hipIndex = kneeIndex = ankleIndex = toeIndex = 0;
    for (unsigned int i = 0, size = skeleton.GetRestPose().GetSize(); i < size; ++i)
    {
        std::string& jointName = skeleton.GetJointName(i);
        if (jointName == hip) {
            hipIndex = i;
        } else if (jointName == knee) {
            kneeIndex = i;
        } else if (jointName == ankle) {
            ankleIndex = i;
        } else if (jointName == toe) {
            toeIndex = i;
        }
    }
}

IKLeg::IKLeg()
{
    ankleToGroundOffset = 0.0f;
    lineVisuals = new DebugDraw();
    pointVisuals = new DebugDraw();

    solver.Resize(3);
    pointVisuals->Resize(3);
    lineVisuals->Resize(4);
}

IKLeg::IKLeg(const IKLeg& other)
{
    lineVisuals = new DebugDraw();
    pointVisuals = new DebugDraw();

    ankleToGroundOffset = 0.0f;
    solver.Resize(3);
    pointVisuals->Resize(3);
    lineVisuals->Resize(4);

    *this = other;
}

IKLeg& IKLeg::operator=(const IKLeg& other)
{
    if (this == &other) {
        return *this;
    }

    solver = other.solver;
    ankleToGroundOffset = other.ankleToGroundOffset;
    hipIndex = other.hipIndex;
    kneeIndex = other.kneeIndex;
    ankleIndex = other.ankleIndex;
    toeIndex = other.toeIndex;

    return *this;
}

IKLeg::~IKLeg()
{
    delete pointVisuals;
    delete lineVisuals;
}

void IKLeg::SolveForLeg(const Transform& model,
                        Pose& pose, 
                        const Vec3& ankleTargetPosition)
{
    solver.SetLocalTransform(0, combine(model, pose.GetGlobalTransform(hipIndex)));
    solver.SetLocalTransform(1, pose.GetLocalTransform(kneeIndex));
    solver.SetLocalTransform(2, pose.GetLocalTransform(ankleIndex));
    IKPose = pose;

    Transform target(ankleTargetPosition + Vec3(0,1,0) * ankleToGroundOffset, 
                     Quat(), 
                     Vec3(1, 1, 1));
    solver.Solve(target);

    Transform rootWorld = combine(model, pose.GetGlobalTransform(pose.GetParent(hipIndex)));
    IKPose.SetLocalTransform(hipIndex, combine(inverse(rootWorld), solver.GetLocalTransform(0)));
    IKPose.SetLocalTransform(kneeIndex, solver.GetLocalTransform(1));
    IKPose.SetLocalTransform(ankleIndex, solver.GetLocalTransform(2));

    lineVisuals->LinesFromIKSolver(solver);
    pointVisuals->PointsFromIKSolver(solver);
}

void IKLeg::Draw(const Mat4& vp, const Vec3& legColor)
{
    lineVisuals->UpdateOpenGLBuffers();
    pointVisuals->UpdateOpenGLBuffers();
    lineVisuals->Draw(DebugDrawMode::Lines, legColor, vp);
    pointVisuals->Draw(DebugDrawMode::Points, legColor, vp);
}

