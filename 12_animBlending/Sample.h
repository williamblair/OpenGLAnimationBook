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

struct AnimationInstance {
    unsigned int clip;
    float time;
    Pose pose;
};

class Sample : public Application
{
public:
    
    bool Initialize(const char* title, const int width, const int height);
    void Update(float inDeltaTime);
    void Render(float inAspectRatio);
    void Shutdown();

private:

    Shader* shader;
    Texture* texture;
    std::vector<Mesh> meshes;
    std::vector<Clip> clips;
    Skeleton skeleton;
    Pose pose;
    std::vector<Mat4> posePalette;
    std::vector<Mat4> skinPalette;
    
    AnimationInstance animA;
    AnimationInstance animB;
    float blendTime;
    float invertBlend;
};

#endif // SAMPLE_H_INCLUDED

