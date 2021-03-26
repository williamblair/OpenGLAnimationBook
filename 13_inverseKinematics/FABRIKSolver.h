#ifndef FABRIK_SOLVER_H_INCLUDED
#define FABRIK_SOLVER_H_INCLUDED

#include <vector>
#include <Transform.h>

class FABRIKSolver
{
public:

    FABRIKSolver();

    inline unsigned int GetSize() const      { return IKChain.size();   }
    inline void Resize(unsigned int newSize) { 
        IKChain.resize(newSize); 
        worldChain.resize(newSize);
        lengths.resize(newSize);
    }

    inline Transform GetLocalTransform(unsigned int index) { return IKChain[index]; }
    inline void SetLocalTransform(const Transform& t, unsigned int index) {
        IKChain[index] = t;
    }

    Transform GetGlobalTransform(unsigned int index);

    inline unsigned int GetNumSteps() const { return numSteps; }
    inline void SetNumSteps(unsigned int newNumSteps) { numSteps = newNumSteps; }

    inline float GetThreshold() const { return threshold; }
    inline void SetThreshold(float newThreshold) { threshold = newThreshold; }

    bool Solve(const Transform& target);

private:

    std::vector<Transform> IKChain;
    unsigned int numSteps;
    float threshold;
    std::vector<Vec3> worldChain;
    std::vector<float> lengths;

    void IKChainToWorld();
    void IterateForward(const Vec3& base);
    void IterateBackward(const Vec3& goal);
    void WorldToIKChain();
#if 0
    void ApplyBallSocketConstraint(int i, float limit);
#endif

#if 0
    void ApplyHingeSocketConstraint(int i, Vec3 axis);
#endif

};

#endif // FABRIK_SOLVER_H_INCLUDED

