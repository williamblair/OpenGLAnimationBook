#ifndef SAMPLE_H_INCLUDED
#define SAMPLE_H_INCLUDED

#include <vector>

#include <Application.h>
#include <Draw.h>
#include <DebugDraw.h>
#include <Pose.h>
#include <Clip.h>
#include <GLTFLoader.h>
#include <Skeleton.h>
#include <Texture.h>
#include <Shader.h>
#include <Uniform.h>
#include <RearrangeBones.h>
#include <CrossFadeController.h>
#include <CrossFadeTarget.h>
#include <Blending.h>
#include <CCDSolver.h>
#include <FABRIKSolver.h>

#define SAMPLE_2

class Sample : public Application
{
public:
    
    bool Initialize(const char* title, const int width, const int height);
    void Update(float inDeltaTime);
    void Render(float inAspectRatio);
    void Shutdown();

private:

    Transform target;
    TransformTrack targetPath;
    
    DebugDraw* solverLines;
    DebugDraw* solverPoints;
    DebugDraw* targetVisual[3];
    
    float playTime;
    float camPitch;
    float camYaw;
    float camDist;

#ifdef SAMPLE_1
    CCDSolver solver;
#endif
#ifdef SAMPLE_2
    FABRIKSolver solver;
#endif

    void SetFrame(VectorTrack& track, int index, float time, const Vec3& value);

};

#endif // SAMPLE_H_INCLUDED

