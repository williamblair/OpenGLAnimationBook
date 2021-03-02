#ifndef TRANSFORM_TRACK_H_INCLUDED
#define TRANSFORM_TRACK_H_INCLUDED

#include <Track.h>
#include <Transform.h>

class TransformTrack
{
public:
    TransformTrack();
    
    unsigned int GetId() const  { return id;     }
    void SetId(unsigned int id) { this->id = id; }

    VectorTrack&     GetPositionTrack() { return position; }
    QuaternionTrack& GetRotationTrack() { return rotation; }
    VectorTrack&     GetScaleTrack()    { return scale; }

    float GetStartTime();
    float GetEndTime();
    bool IsValid(); // true if any of the three tracks are valid

    Transform Sample(const Transform& ref, float time, bool looping);

protected:
    unsigned int id; // bone/joint id this track is used for
    VectorTrack     position;
    QuaternionTrack rotation;
    VectorTrack     scale;
};

#endif // TRANSFORM_TRACK_H_INCLUDED

