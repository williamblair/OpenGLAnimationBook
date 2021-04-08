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
#include <AnimTexture.h>
#include <AnimBaker.h>
#include <Crowd.h>

class Sample : public Application
{
public:
    
    bool Initialize(const char* title, const int width, const int height);
    void Update(float inDeltaTime);
    void Render(float inAspectRatio);
    void Shutdown();

private:

    Texture* diffuseTexture;
    Shader* crowdShader;
    std::vector<Mesh> meshes;
    std::vector<Clip> clips;
    std::vector<AnimTexture> textures;
    std::vector<Crowd> crowds;
    Skeleton skeleton;
    
    void SetCrowdSize(unsigned int size);
    
};

#endif // SAMPLE_H_INCLUDED

