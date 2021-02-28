#include <Clip.h>

Clip::Clip()
{
    name = "No name given";
    startTime = 0.0f;
    endTime = 0.0f;
    looping = true;
}

float Clip::Sample(Pose& outPose, float inTime)
{
    if (GetDuration() == 0.0f) {
        return 0.0f;
    }

    inTime = AdjustTimeToFitRange(inTime);
    unsigned int size = tracks.size();
    
    for (unsigned int i = 0; i < size; ++i) {
        unsigned int j = tracks[i].GetId(); // joint id
        Transform local = outPose.GetLocalTransform(j);
        Transform animated = tracks[i].Sample(local, inTime, looping);
        outPose.SetLocalTransform(j, animated);
    }

    return inTime;
}

TransformTrack& Clip::operator[](unsigned int index)
{
    int size = tracks.size();
    for (int i = 0; i < size; ++i) {
        if (tracks[i].GetId() == index) { // joint index
            return tracks[i];
        }
    }

    // add a default new one if the index doesn't exist
    tracks.push_back(TransformTrack());
    tracks[tracks.size()-1].SetId(index);
    return tracks[tracks.size()-1];
}

void Clip::RecalculateDuration()
{
    startTime = 0.0f;
    endTime = 0.0f;
    bool startSet = false;
    bool endSet = false;
    unsigned int trackSize = tracks.size();
    for (unsigned int i = 0; i < trackSize; ++i) {
        if (tracks[i].IsValid()) {
            float tStartTime = tracks[i].GetStartTime();
            float tEndTime = tracks[i].GetEndTime();
            if (tStartTime < startTime || !startSet) {
                startTime = tStartTime;
                startSet = true;
            }
            if (tEndTime > endTime || !endSet) {
                endTime = tEndTime;
                endSet = true;
            }
        }
    }
}

float Clip::AdjustTimeToFitRange(float inTime)
{
    // modulate between start and end
    if (looping) {
        float duration = endTime - startTime;
        if (duration <= 0.0f) {
            return 0.0f;
        }
        
        inTime = fmodf(inTime - startTime, duration);
        if (inTime < 0.0f) {
            inTime += duration;
        }
        inTime = inTime + startTime;

    // clamp between start and end
    } else {
        if (inTime < startTime) {
            inTime = startTime;
        }
        if (inTime > endTime) {
            inTime = endTime;
        }
    }
    
    return inTime;
}

