#ifndef CROWD_H_INCLUDED
#define CROWD_H_INCLUDED

#include <vector>

#include <Vec2.h>
#include <Vec3.h>
#include <Quat.h>
#include <Transform.h>
#include <Clip.h>
#include <Shader.h>
#include <Uniform.h>

#define CROWD_MAX_ACTORS 80

class Crowd
{
public:

    unsigned int GetSize() const { return currentPlayTimes.size(); }
    void Resize(unsigned int size);
    
    Transform GetActor(unsigned int index);
    void SetActor(unsigned int index, const Transform& t);

    void Update(float deltaTime, Clip& clip, unsigned int texWidth);
    void SetUniforms(Shader* shader);
    
    void RandomizeTimes(Clip& clip);
    void RandomizePositions(std::vector<Vec3>& existing, const Vec3& min, const Vec3& max, float radius);

private:
    std::vector<Vec3> positions;
    std::vector<Quat> rotations;
    std::vector<Vec3> scales;
    // u component = current animation frame
    // v component = next animation frame
    std::vector<iVec2> frames;
    std::vector<float> times;
    std::vector<float> currentPlayTimes;
    std::vector<float> nextPlayTimes;
    
    float AdjustTime(float t, float start, float end, bool looping);
    void UpdatePlaybackTimes(float dt, bool looping, float start, float end);
    void UpdateFrameIndices(float start, float duration, unsigned int texWidth);
    void UpdateInterpolationTimes(float start, float duration, unsigned int texWidth);
};

#endif // CROWD_H_INCLUDED
