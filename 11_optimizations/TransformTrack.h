#ifndef TRANSFORM_TRACK_H_INCLUDED
#define TRANSFORM_TRACK_H_INCLUDED

#include <Track.h>
#include <Transform.h>

template<typename VTRACK, typename QTRACK>
class TTransformTrack
{
public:
    TTransformTrack();
    
    unsigned int GetId() const  { return id;     }
    void SetId(unsigned int id) { this->id = id; }

    VTRACK& GetPositionTrack() { return position; }
    QTRACK& GetRotationTrack() { return rotation; }
    VTRACK& GetScaleTrack()    { return scale; }

    float GetStartTime();
    float GetEndTime();
    bool IsValid(); // true if any of the three tracks are valid

    Transform Sample(const Transform& ref, float time, bool looping);

protected:
    unsigned int id; // bone/joint id this track is used for
    VTRACK position;
    QTRACK rotation;
    VTRACK scale;
};

typedef TTransformTrack<VectorTrack, QuaternionTrack> TransformTrack;
typedef TTransformTrack<FastVectorTrack, FastQuaternionTrack> FastTransformTrack;

// converts input transform track to a fast transform track
FastTransformTrack OptimizeTransformTrack(TransformTrack& input);

#endif // TRANSFORM_TRACK_H_INCLUDED

