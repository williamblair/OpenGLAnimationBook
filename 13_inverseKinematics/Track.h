#ifndef TRACK_H_INCLUDED
#define TRACK_H_INCLUDED

#include <vector>

#include <Vec3.h>
#include <Quat.h>
#include <Frame.h>
#include <Interpolation.h>

// a track is a collection of frames which can be interpolated between
template<typename T, int N>
class Track
{
public:
    Track();

    void Resize(unsigned int size);
    unsigned int GetSize();
    Interpolation GetInterpolation();
    void SetInterpolation(Interpolation interp);
    float GetStartTime();
    float GetEndTime();

    T Sample(float time, bool looping);
    Frame<N>& operator[](unsigned int index);

    T Hermite(float time, const T& p1, const T& s1, const T& p2, const T& s2);
    
    // returns the last frame before the requested time
    virtual int FrameIndex(float time, bool looping);

    // maps time to within input range
    float AdjustTimeToFitTrack(float t, bool loop);

    // will be specialized based on template implementation
    T Cast(float* value);

protected:
    std::vector<Frame<N>> frames;
    Interpolation interpolation;

    T SampleConstant(float time, bool looping);
    T SampleLinear(float time, bool looping);
    T SampleCubic(float time, bool looping);

};

// optimized frame index lookup version
template<typename T, int N>
class FastTrack : public Track<T,N>
{
protected:
    std::vector<unsigned int> sampledFrames;
public:
    virtual int FrameIndex(float time, bool looping);
    void UpdateIndexLookupTable();
};

// convert a track to a fast track
template<typename T, int N>
FastTrack<T,N> OptimizeTrack(Track<T,N>& input);

// use optimized version instead
typedef Track<float, 1> ScalarTrack;
typedef Track<Vec3, 3> VectorTrack;
typedef Track<Quat, 4> QuaternionTrack;
typedef FastTrack<float, 1> FastScalarTrack;
typedef FastTrack<Vec3, 3> FastVectorTrack;
typedef FastTrack<Quat, 4> FastQuaternionTrack;

#endif // TRACK_H_INCLUDED

