#ifndef CCD_SOLVER_H_INCLUDED
#define CCD_SOLVER_H_INCLUDED

#include <vector>
#include <Transform.h>

class CCDSolver
{
public:

    CCDSolver();

    inline unsigned int GetSize() const      { return IKChain.size();   }
    inline void Resize(unsigned int newSize) { IKChain.resize(newSize); }

    // gets local transform
    inline Transform& operator[](unsigned int index) { return IKChain[index]; }

    Transform GetGlobalTransform(unsigned int index);

    inline unsigned int GetNumSteps() const { return numSteps; }
    inline void SetNumSteps(unsigned int newNumSteps) { numSteps = newNumSteps; }

    inline float GetThreshold() const { return threshold; }
    inline void SetThreshold(float newThreshold) { threshold = newThreshold; }

    // Runs the CCD algorithm for numSteps or threshold reached
    // Only the position component of the input transform is used, which is
    // the goal position
    bool Solve(const Transform& target);

protected:

    std::vector<Transform> IKChain;
    unsigned int numSteps; // how many iterations to run before 'convergence'
    float threshold; // target convergence value

};

#endif // CCD_SOLVER_H_INCLUDED

