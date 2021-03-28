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
#include <IKLeg.h>
#include <Intersections.h>

#define SAMPLE_1 0
#define SAMPLE_2 0
#define SAMPLE_3 1

class Sample : public Application
{
public:
    
    bool Initialize(const char* title, const int width, const int height);
    void Update(float inDeltaTime);
    void Render(float inAspectRatio);
    void Shutdown();

private:

#if (SAMPLE_1 || SAMPLE_2)
    Transform target;
    TransformTrack targetPath;
    
    DebugDraw* solverLines;
    DebugDraw* solverPoints;
    DebugDraw* targetVisual[3];
    
    float playTime;
    float camPitch;
    float camYaw;
    float camDist;

    void SetFrame(VectorTrack& track, int index, float time, const Vec3& value);
#endif

#if SAMPLE_1
    CCDSolver solver;
#endif
#if SAMPLE_2
    FABRIKSolver solver;
#endif


#if SAMPLE_3
    
    // The course for the character to walk on
    Texture* courseTexture;
    std::vector<Mesh> IKCourse;
    std::vector<Triangle> triangles;

    VectorTrack motionTrack;
    float walkingTime;

    Transform model;
    std::vector<Mesh> meshes;
    Pose currentPose;
    std::vector<Mat4> posePalette;
    float sinkIntoGround;
    
    IKLeg* leftLeg;
    IKLeg* rightLeg;
    float toeLength;
    float lastModelY;

    Texture* diffuseTexture;
    Shader* staticShader;
    Shader* skinnedShader;
    Skeleton skeleton;
    std::vector<Clip> clips;
    unsigned int currentClip;
    float playbackTime;
    DebugDraw* currentPoseVisual;
    
    bool showIKPose;
    bool showCurrentPose;
    float timeMod;
    bool depthTest;
    bool showMesh;
    bool showEnvironment;
#endif

};

#endif // SAMPLE_H_INCLUDED

