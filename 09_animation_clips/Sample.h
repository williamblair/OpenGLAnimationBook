#ifndef SAMPLE_H_INCLUDED
#define SAMPLE_H_INCLUDED

#include <vector>

#include <Application.h>
#include <Draw.h>
#include <DebugDraw.h>
#include <Pose.h>
#include <Clip.h>
#include <GLTFLoader.h>

class Sample : public Application
{
public:
    
    bool Initialize(const char* title, const int width, const int height);
    void Update(float inDeltaTime);
    void Render(float inAspectRatio);
    void Shutdown();

private:

    Pose              restPose;  
    Pose              currentPose;
    std::vector<Clip> clips;
    unsigned int      currentClip;
    float             playbackTime;
    
    DebugDraw* restPoseVisual;
    DebugDraw* currentPoseVisual;
};

#endif // SAMPLE_H_INCLUDED

