#ifndef IK_LEG_H_INCLUDED
#define IK_LEG_H_INCLUDED

#include <CCDSolver.h>
#include <FABRIKSolver.h>
#include <DebugDraw.h>
#include <Skeleton.h>
#include <Track.h>

class IKLeg
{
public:

    IKLeg(Skeleton& skeleton, 
          const std::string& hip, 
          const std::string& knee, 
          const std::string& ankle, 
          const std::string& toe);
    IKLeg();
    IKLeg(const IKLeg&);
    IKLeg& operator=(const IKLeg&);
    ~IKLeg();

    void SolveForLeg(const Transform& model, 
                     Pose& pose, 
                     const Vec3& ankleTargetPosition);

    Pose& GetAdjustedPose() { return IKPose; }
    ScalarTrack& GetTrack() { return pinTrack; }

    void Draw(const Mat4& vp, const Vec3& legColor);

    unsigned int Hip()   { return hipIndex;   }
    unsigned int Knee()  { return kneeIndex;  }
    unsigned int Ankle() { return ankleIndex; }
    unsigned int Toe()   { return toeIndex;   }

    inline void SetAnkleOffset(float offset) {
        ankleToGroundOffset = offset;
    }

private:

    ScalarTrack pinTrack;
    FABRIKSolver solver;
    Pose IKPose;

    unsigned int hipIndex;
    unsigned int kneeIndex;
    unsigned int ankleIndex;
    unsigned int toeIndex;
    
    DebugDraw* lineVisuals;
    DebugDraw* pointVisuals;
    
    float ankleToGroundOffset;
};

#endif // IK_LEG_H_INCLUDED

