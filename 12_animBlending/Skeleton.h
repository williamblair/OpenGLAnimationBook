#ifndef SKELETON_H_INCLUDED
#define SKELETON_H_INCLUDED

#include <vector>
#include <string>

#include <Mat4.h>
#include <Pose.h>
#include <Transform.h>

class Skeleton
{
public:
    Skeleton();
    Skeleton(const Pose& rest, const Pose& bind, const std::vector<std::string>& names);
    ~Skeleton();
    
    void Set(const Pose& rest, const Pose& bind, const std::vector<std::string>& names);
    
    inline Pose& GetBindPose() { return bindPose; }
    inline Pose& GetRestPose() { return restPose; }

    inline std::vector<Mat4>& GetInvBindPose()           { return invBindPose; }
    inline std::vector<std::string>& GetJointNames()     { return jointNames;  }
    inline std::string& GetJointName(unsigned int index) { return jointNames[index]; }

protected:

    Pose restPose;
    Pose bindPose;
    std::vector<Mat4>        invBindPose;
    void UpdateInverseBindPose();
    std::vector<std::string> jointNames;
};

#endif // SKELETON_H_INCLUDED
