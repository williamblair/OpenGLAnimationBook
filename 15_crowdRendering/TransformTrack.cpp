#include <iostream>
#include <TransformTrack.h>

// template declarations
template class TTransformTrack<VectorTrack, QuaternionTrack>;
template class TTransformTrack<FastVectorTrack, FastQuaternionTrack>;

template<typename VTRACK, typename QTRACK>
TTransformTrack<VTRACK,QTRACK>::TTransformTrack()
{
    id = 0;
}

template<typename VTRACK, typename QTRACK>
bool TTransformTrack<VTRACK,QTRACK>::IsValid()
{
    return (position.GetSize() > 1 ||
           rotation.GetSize() > 1 ||
           scale.GetSize() > 1);
}

// returns the smallest start time of the 3 tracks
template<typename VTRACK, typename QTRACK>
float TTransformTrack<VTRACK,QTRACK>::GetStartTime()
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
template<typename VTRACK, typename QTRACK>
float TTransformTrack<VTRACK,QTRACK>::GetEndTime()
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

template<typename VTRACK, typename QTRACK>
Transform TTransformTrack<VTRACK,QTRACK>::Sample(const Transform& ref, float time, bool looping)
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
    
    return result;
}

FastTransformTrack OptimizeTransformTrack(TransformTrack& input)
{
    FastTransformTrack result;
    
    result.SetId(input.GetId());
    result.GetPositionTrack() = OptimizeTrack<Vec3, 3>(input.GetPositionTrack());
    result.GetRotationTrack() = OptimizeTrack<Quat, 4>(input.GetRotationTrack());
    result.GetScaleTrack() = OptimizeTrack<Vec3, 3>(input.GetScaleTrack());
    
    return result;
}