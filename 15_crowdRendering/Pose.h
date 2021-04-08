#ifndef POSE_H_INCLUDED
#define POSE_H_INCLUDED

#include <vector>

#include <Mat4.h>
#include <Transform.h>
#include <DualQuaternion.h>

class Pose
{
public:

    Pose();
    Pose(const Pose& p);
    Pose& operator=(const Pose& p);
    Pose(unsigned int numJoints);

    void Resize(unsigned int size) { joints.resize(size); parents.resize(size); }
    inline unsigned int GetSize() const { return joints.size(); }

    inline int GetParent(unsigned int index) const { return parents[index]; }
    inline void SetParent(unsigned int index, int parent) { parents[index] = parent; }

    inline Transform GetLocalTransform(unsigned int index) const { return joints[index]; }
    inline void SetLocalTransform(unsigned int index, const Transform& transform) {
        joints[index] = transform;
    }

    // combines transforms from the root up until the desired local joint index
    Transform GetGlobalTransform(unsigned int index);
    Transform operator[](unsigned int index);

    DualQuaternion GetGlobalDualQuaternion(unsigned int index);

    // fills out with a linear array of matrices as the gobal transform matrix of each
    // joint in the pose
    void GetMatrixPalette(std::vector<Mat4>& out);
    // Global transform dual quaternion of each joint in the pose
    void GetDualQuaternionPalette(std::vector<DualQuaternion>& out);

    bool operator==(const Pose& other);
    bool operator!=(const Pose& other);

protected:
    
    std::vector<Transform> joints;
    std::vector<int> parents;
};

// For blending between animations
// returns true if search is within the root node hierarchy
bool IsInHierarchy(Pose& pose, unsigned int root, unsigned int search);
// assumes ths poses are similar and have the same number of joints
void Blend(Pose& output, Pose& a, Pose& b, float t, int root);

#endif // POSE_H_INCLUDED

