#include <Crowd.h>
#include <cstring>

void Crowd::Resize(unsigned int size)
{
    if (size > CROWD_MAX_ACTORS) {
        size = CROWD_MAX_ACTORS;
    }
    
    positions.resize(size);
    rotations.resize(size);
    scales.resize(size, Vec3(1,1,1));
    frames.resize(size);
    times.resize(size);
    currentPlayTimes.resize(size);
    nextPlayTimes.resize(size);
}

Transform Crowd::GetActor(unsigned int index)
{
    return Transform( positions[index],
                      rotations[index],
                      scales[index] );
}

void Crowd::SetActor(unsigned int index, const Transform& t)
{
    positions[index] = t.position;
    rotations[index] = t.rotation;
    scales[index] = t.scale;
}

float Crowd::AdjustTime(float time, float start, float end, bool looping)
{
    if (looping) {
        time = fmodf(time - start, end - start);
        if (time < 0.0f) {
            time += end - start;
        }
        time = time + start;
    }
    else {
        if (time < start) { time = start; }
        else if (time > end) { time = end; }
    }
    return time;
}

void Crowd::UpdatePlaybackTimes(float deltaTime, bool looping, float start, float end)
{
    unsigned int size = currentPlayTimes.size();
    for (unsigned int i = 0; i < size; ++i)
    {
        float time = currentPlayTimes[i] + deltaTime;
        currentPlayTimes[i] = AdjustTime(time, start, end, looping);
        time = currentPlayTimes[i] + deltaTime;
        nextPlayTimes[i] = AdjustTime(time, start, end, looping);
    }
}

void Crowd::UpdateFrameIndices(float start, float duration, unsigned int texWidth)
{
    unsigned int size = currentPlayTimes.size();
    for (unsigned int i = 0; i < size; ++i)
    {
        float thisNormalizedTime = (currentPlayTimes[i] - start) / duration;
        unsigned int thisFrame = thisNormalizedTime * (texWidth - 1);
        float nextNormalizedTime = (nextPlayTimes[i] - start) / duration;
        unsigned int nextFrame = nextNormalizedTime * (texWidth - 1);
        frames[i].x = thisFrame;
        frames[i].y = nextFrame;
    }
}

void Crowd::UpdateInterpolationTimes(float start, float duration, unsigned int texWidth)
{
    unsigned int size = currentPlayTimes.size();
    for (unsigned int i = 0; i < size; ++i)
    {
        if (frames[i].x == frames[i].y) {
            times[i] = 1.0f;
            continue;
        }
        
        float thisT = (float)frames[i].x / (float)(texWidth - 1);
        float thisTime = start + duration * thisT;
        float nextT = (float)frames[i].y / (float)(texWidth - 1);
        float nextTime = start + duration * nextT;
        if (nextTime < thisTime) {
            nextTime += duration;
        }
        float frameDuration = nextTime - thisTime;
        times[i] = (currentPlayTimes[i] - thisTime) / frameDuration;
    }
}

void Crowd::Update(float deltaTime, Clip& clip, unsigned int texWidth)
{
    bool looping = clip.GetLooping();
    float start = clip.GetStartTime();
    float end = clip.GetEndTime();
    float duration = clip.GetDuration();
    
    UpdatePlaybackTimes(deltaTime, looping, start, end);
    UpdateFrameIndices(start, duration, texWidth);
    UpdateInterpolationTimes(start, duration, texWidth);
}

void Crowd::SetUniforms(Shader* shader)
{
    Uniform<Vec3>::Set(shader->GetUniform("model_pos"), positions);
    Uniform<Quat>::Set(shader->GetUniform("model_rot"), rotations);
    Uniform<Vec3>::Set(shader->GetUniform("model_scl"), scales);
    Uniform<iVec2>::Set(shader->GetUniform("frames"), frames);
    Uniform<float>::Set(shader->GetUniform("time"), times);
}

void Crowd::RandomizeTimes(Clip& clip)
{
    float start = clip.GetStartTime();
    float duration = clip.GetDuration();
    
    unsigned int size = (unsigned int)currentPlayTimes.size();
    for (unsigned int i = 0; i < size; ++i)
    {
        float rnd = (float)rand() / (float)RAND_MAX;
        currentPlayTimes[i] = rnd * duration + start;
    }
}

void Crowd::RandomizePositions(std::vector<Vec3>& existing,
                               const Vec3& min,
                               const Vec3& max,
                               float radius)
{
    std::vector<Vec3> newPositions;
    unsigned int size = (unsigned int)currentPlayTimes.size();
    newPositions.reserve(size);
    Vec3 delta = max - min;
    
    unsigned int breakLoop = 0;
    while (newPositions.size() < size)
    {
        if (breakLoop >= 2000) {
            break;
        }
        Vec3 rnd((float)rand() / (float)RAND_MAX,
                 (float)rand() / (float)RAND_MAX,
                 (float)rand() / (float)RAND_MAX);
        Vec3 point = min + delta * rnd;
        
        bool valid = true;
        for (unsigned int i = 0, iSize = (unsigned int)existing.size(); i < iSize; ++i) {
            if (lenSq(existing[i] - point) < radius * radius) {
                valid = false;
                breakLoop++;
                break;
            }
        }
        
        if (valid) {
            breakLoop = 0;
            newPositions.push_back(point);
            existing.push_back(point);
        }
    }
    
    if (newPositions.size() != size) {
        Resize((unsigned int)newPositions.size());
    }
    
    memcpy(positions[0].v, newPositions[0].v, sizeof(float) * newPositions.size() * 3);
}
