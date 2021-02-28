#include <TransformTrack.h>

TransformTrack::TransformTrack()
{
    id = 0;
}

bool TransformTrack::IsValid()
{
    return (position.GetSize() > 1 ||
           rotation.GetSize() > 1 ||
           scale.GetSize() > 1);
}

// returns the smallest start time of the 3 tracks
float TransformTrack::GetStartTime()
{
    float result = 0.0f;
    bool isSet = false;
    if (position.GetSize() > 1) {
        result = position.GetStartTime();
        isSet = true;
    }

    if (rotation.GetSize() > 1) {
        float rotationStart = rotation.GetStartTime();
        if (rotationStart < result || !isSet) {
            result = rotationStart;
            isSet = true;
        }
    }
    if (scale.GetSize() > 1) {
        float scaleStart = scale.GetStartTime();
        if (scaleStart < result || !isSet) {
            result = scaleStart;
            isSet = true;
        }
    }

    return result;
}


// returns the largest end time of the 3 tracks
float TransformTrack::GetEndTime()
{
    float result = 0.0f;
    bool isSet = false;
    if (position.GetSize() > 1) {
        result = position.GetEndTime();
        isSet = true;
    }

    if (rotation.GetSize() > 1) {
        float rotationEnd = rotation.GetEndTime();
        if (rotationEnd > result || !isSet) {
            result = rotationEnd;
            isSet = true;
        }
    }
    if (scale.GetSize() > 1) {
        float scaleEnd = scale.GetEndTime();
        if (scaleEnd > result || !isSet) {
            result = scaleEnd;
            isSet = true;
        }
    }

    return result;
}

Transform TransformTrack::Sample(const Transform& ref, float time, bool looping)
{
    Transform result = ref; // default values
    if (position.GetSize() > 1) {
        result.position = position.Sample(time, looping);
    }
    if (rotation.GetSize() > 1) {
        result.rotation = rotation.Sample(time, looping);
    }
    if (scale.GetSize() > 1) {
        result.scale = scale.Sample(time, looping);
    }
    
    return ref;
}

