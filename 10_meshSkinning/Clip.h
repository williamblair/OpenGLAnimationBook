#ifndef CLIP_H_INCLUDED
#define CLIP_H_INCLUDED

#include <vector>
#include <string>
#include <iostream>
#include <TransformTrack.h>
#include <Pose.h>

class Clip
{
public:

    Clip();

    // get and set the joint id for that transform track index
    inline unsigned int GetIdAtIndex(unsigned int index) const { return tracks[index].GetId(); }
    inline void SetIdAtIndex(unsigned int index, unsigned int id) {
        tracks[index].SetId(id);
    }

    inline unsigned int GetSize() const { return (unsigned int)tracks.size(); }

    // fills in outPose and returns the adjusted time for that pose
    float Sample(Pose& outPose, float inTime);
    
    // get the transform track at the index; if it doesn't exist, a default is returned
    TransformTrack& operator[](unsigned int index);

    // sets the start/end time of the clip based on the internal tracks
    // finds the min and max start and end time within the tracks and uses those
    void RecalculateDuration();

    inline std::string& GetName()                             { return name;                }
    inline void         SetName(const std::string& inNewName) { name = inNewName;           }
    inline float        GetDuration() const                   { return endTime - startTime; }
    inline float        GetStartTime() const                  { return startTime;           }
    inline float        GetEndTime() const                    { return endTime;             }
    inline bool         GetLooping() const                    { return looping;             }
    inline void         SetLooping(bool inLooping)            { looping = inLooping;        }

protected:

    std::vector<TransformTrack> tracks;
    std::string name;

    float startTime;
    float endTime;
    bool looping; // this would normally be forwarded to a further animation step such as animation component or similar

    float AdjustTimeToFitRange(float inTime);
};

#endif // CLIP_H_INCLUDED

