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
#include <DualQuaternion.h>

class Sample : public Application
{
public:
    
    bool Initialize(const char* title, const int width, const int height);
    void Update(float inDeltaTime);
    void Render(float inAspectRatio);
    void Shutdown();

private:

    Texture* diffuseTexture;
    Shader* linearBlendShader;
    Shader* dualQuatShader;
    std::vector<Mesh> meshes;
    Skeleton skeleton;
    Pose currentPose;

    std::vector<DualQuaternion> dqPosePalette;
    std::vector<DualQuaternion> dqInvBindPalette;
    std::vector<Mat4> lbPosePalette;
    std::vector<Mat4> lbInvBindPalette;
    
    std::vector<Clip> clips;
    unsigned int currentClip;
    float playbackTime;

};

#endif // SAMPLE_H_INCLUDED

