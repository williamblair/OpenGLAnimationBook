#include <Track.h>
#include <cstring>
#include <iostream>

template class Track<float, 1>;
template class Track<Vec3, 3>;
template class Track<Quat, 4>;
template class FastTrack<float, 1>;
template class FastTrack<Vec3, 3>;
template class FastTrack<Quat, 4>;

namespace TrackHelpers
{
    // linear interpolation
    inline float Interpolate(float a, float b, float t) {
        return a + (b - a) * t;
    }
    inline Vec3 Interpolate(const Vec3& a, const Vec3& b, float t) {
        return lerp(a, b, t);
    }
    inline Quat Interpolate(const Quat& a, const Quat& b, float t) {
        Quat result = Mix(a, b, t);
        // perform neighborhood check
        if (dot(a, b) < 0) {
            result = Mix(a, -b, t);
        }
        return normalized(result); // using nlerp
    }

    // normalization if necessary for each type specification
    inline float AdjustHermiteResult(float f) {
        return f;
    }
    inline Vec3 AdjustHermiteResult(const Vec3& v) {
        return v;
    }
    inline Quat AdjustHermiteResult(const Quat& q) {
        return normalized(q);
    }

    // neighborhooding if necessary for each type
    inline void Neighborhood(const float& a, const float& b) {} // does nothing
    inline void Neighborhood(const Vec3& a, const Vec3& b) {} // does nothing
    inline void Neighborhood(const Quat& a, Quat& b) {
        if (dot(a, b) < 0) {
            b = -b;
        }
    }
} // end TrackHelpers namespace

template<typename T, int N>
Track<T, N>::Track() {
    interpolation = Interpolation::Linear;
}

template<typename T, int N>
inline float Track<T, N>::GetStartTime() {
    return frames[0].time;
}
template<typename T, int N>
inline float Track<T, N>::GetEndTime() {
    return frames[frames.size()-1].time;
}

template<typename T, int N>
T Track<T, N>::Sample(float time, bool looping)
{
    if (interpolation == Interpolation::Constant) {
        return SampleConstant(time, looping);
    } else if (interpolation == Interpolation::Linear) {
        return SampleLinear(time, looping);
    }
    // else
    //std::cout << "Sampling cubic" << std::endl;
    return SampleCubic(time, looping);
}
template<typename T, int N>
Frame<N>& Track<T,N>::operator[](unsigned int index) {
    return frames[index];
}

template<typename T, int N>
inline void Track<T,N>::Resize(unsigned int size) {
    frames.resize(size);
}
template<typename T, int N>
inline unsigned int Track<T,N>::GetSize() {
    return frames.size();
}

template<typename T, int N>
inline Interpolation Track<T,N>::GetInterpolation() {
    return interpolation;
}
template<typename T, int N>
inline void Track<T,N>::SetInterpolation(Interpolation interp) {
    interpolation = interp;
}


template<typename T, int N>
T Track<T,N>::Hermite(float t, const T& p1, const T& s1, const T& p2, const T& s2)
{
    float tt = t * t;
    float ttt = tt * t;
    T tmpP2 = p2;
    TrackHelpers::Neighborhood(p1, tmpP2);

    float h1 = 2.0f * ttt - 3.0f * tt + 1.0f;
    float h2 = -2.0f * ttt + 3.0f * tt;
    float h3 = ttt - 2.0f * tt + t;
    float h4 = ttt - tt;

    T result = p1 * h1 + tmpP2 * h2 + s1 * h3 + s2 * h4;
    return TrackHelpers::AdjustHermiteResult(result);
}

template<typename T, int N>
int Track<T,N>::FrameIndex(float time, bool looping)
{
    // if the track has one frame or less, it is invalid
    unsigned int size = (unsigned int)frames.size();
    if (size <= 1) {
        return -1;
    }

    // if looping, we need to map between the start and end time (modulo)
    if (looping) {
        float startTime = frames[0].time;
        float endTime = frames[size - 1].time;
        float duration = endTime - startTime;

        time = fmodf(time - startTime, endTime - startTime);
        if (time < 0.0f) {
            time += endTime - startTime;
        }
        time = time + startTime;

    // otherwise clamp between start and end time
    } else {
        if (time <= frames[0].time) {
            return 0;
        }
        if (time >= frames[size - 2].time) {
            return (int)size - 2;
        }
    }

    // find the frame less than and closest to the desired time
    for (int i = (int)size - 1; i >= 0; --i) {
        if (time >= frames[i].time) {
            return i;
        }
    }

    // should theoretically never reach here
    return -1;
}

template<typename T, int N>
float Track<T,N>::AdjustTimeToFitTrack(float t, bool loop) {
    unsigned int size = (unsigned int)frames.size();
    if (size <= 1) {
        return 0.0f;
    }

    float startTime = frames[0].time;
    float endTime = frames[size - 1].time;
    float duration = endTime - startTime;
    if (duration <= 0.0f) {
        return 0.0f;
    }

    // wrap t if looping
    if (loop) {
        t = fmodf(t - startTime, endTime - startTime);
        if (t < 0.0f) {
            t += endTime - startTime;
        }
        t = t + startTime;

    // otherwise clamp between start and end
    } else {
        if (t <= frames[0].time) {
            t = startTime;
        }
        if (t >= frames[size - 1].time) {
            t = endTime;
        }
    }

    return t;
}

template<>
inline float Track<float, 1>::Cast(float* value) {
    return value[0];
}
template<> 
inline Vec3 Track<Vec3, 3>::Cast(float* value) {
    return Vec3(value[0], value[1], value[2]);
}
template<> 
inline Quat Track<Quat, 4>::Cast(float* value) {
    Quat q = Quat(value[0], value[1], value[2], value[3]);
    return normalized(q);
}

// no interpolation; results in immediate change in value (e.g. turn on/off)
template<typename T, int N>
T Track<T,N>::SampleConstant(float t, bool looping)
{
    int frame = FrameIndex(t, looping);
    if (frame < 0 || frame >= (int)frames.size()) {
        // TODO ERROR
        return T();
    }

    return Cast(&frames[frame].value[0]);
}

// linearly interpolates between the current and next frame
template<typename T, int N>
T Track<T, N>::SampleLinear(float time, bool looping) 
{
    int thisFrame = FrameIndex(time, looping);
    if (thisFrame < 0 || thisFrame >= frames.size() - 1) {
        // TODO error
        std::cout << "Sample linear this frame error, returning default" << std::endl;
        return T();
    }

    int nextFrame = thisFrame + 1;
    float trackTime = AdjustTimeToFitTrack(time, looping);
    float thisTime = frames[thisFrame].time;
    float frameDelta = frames[nextFrame].time - thisTime;
    if (frameDelta <= 0.0f) {
        std::cout << "Frame default < 0, returning default" << std::endl;
        return T();
    }

    float t = (trackTime - thisTime) / frameDelta;
    T start = Cast(&frames[thisFrame].value[0]);
    T end = Cast(&frames[nextFrame].value[0]);
    return TrackHelpers::Interpolate(start, end, t);
}

// Cubic track sampling using Hermite
template<typename T, int N>
T Track<T,N>::SampleCubic(float time, bool looping)
{
    int thisFrame = FrameIndex(time, looping);
    if (thisFrame < 0 || thisFrame >= frames.size() - 1) {
        // TODO error
        return T();
    }

    int nextFrame = thisFrame + 1;
    float trackTime = AdjustTimeToFitTrack(time, looping);
    float thisTime = frames[thisFrame].time;
    float frameDelta = frames[nextFrame].time - thisTime;
    if (frameDelta <= 0.0f) {
        return T();
    }

    float t = (trackTime - thisTime) / frameDelta;
    size_t floatSize = sizeof(float);

    // memcpy so we don't normalize if we were to use cast
    T point1 = Cast(&frames[thisFrame].value[0]);
    T slope1; // = frames[thisFrame].out * frameDelta
    memcpy(&slope1, frames[thisFrame].out, N * floatSize);
    slope1 = slope1 * frameDelta;

    T point2 = Cast(&frames[nextFrame].value[0]);
    T slope2; // = frames[nextFrame].in * frameDelta
    memcpy(&slope2, frames[nextFrame].in, N * floatSize);
    slope2 = slope2 * frameDelta;

    return Hermite(t, point1, slope1, point2, slope2);
}

template<typename T, int N>
void FastTrack<T,N>::UpdateIndexLookupTable()
{
    int numFrames = (int)this->frames.size();
    if (numFrames <= 1) {
        return;
    }
    
    // assumes 60 frames per second is enough
    float duration = this->GetEndTime() - this->GetStartTime();
    unsigned int numSamples = duration * 60.0f;
    sampledFrames.resize(numSamples);
    
    // pre compute sample along the track
    for (unsigned int i = 0; i < numSamples; ++i) {
        
        float t = (float)i / (float)(numSamples - 1);
        float time = t * duration + this->GetStartTime();
        
        // find the closest matching frame
        unsigned int frameIndex = 0;
        for (int j = numFrames - 1; j >= 0; --j) {
            if (time >= this->frames[j].time) {
                frameIndex = (unsigned int)j;
                if ((int)frameIndex >= numFrames - 2) {
                    frameIndex = numFrames - 2;
                }
                break;
            }
        }
        sampledFrames[i] = frameIndex;
    }
}

template<typename T, int N>
int FastTrack<T,N>::FrameIndex(float time, bool loop)
{
    unsigned int size = this->frames.size();
    if (size <= 1) {
        // TODO error
        return -1;
    }
    
    // wrap within looping duration
    if (loop) {
        float startTime = this->frames[0].time;
        float endTime = this->frames[this->frames.size() - 1].time;
        float duration = endTime - startTime;
        
        time = fmodf(time - startTime, endTime - startTime);
        if (time < 0.0f) {
            time += duration;
        }
        time = time + startTime;
        
    // clamp between start and end
    } else {
        if (time <= this->frames[0].time) {
            return 0;
        }
        if (time >= this->frames[this->frames.size() - 2].time) {
            return (int)this->frames.size() - 2;
        }
    }
    
    float duration = this->GetEndTime() - this->GetStartTime();
    float t = time / duration;
    unsigned int numSamples = duration * 60.0f; // assumes 60 frames per second max
    unsigned int index = t * (float)numSamples;
    if (index >= sampledFrames.size()) {
        // TODO error
        return -1;
    }
    return (int)sampledFrames[index];
}

// specializations declaration
template FastTrack<float,1> OptimizeTrack(Track<float,1>& input);
template FastTrack<Vec3,3> OptimizeTrack(Track<Vec3,3>& input);
template FastTrack<Quat,4> OptimizeTrack(Track<Quat,4>& input);

template<typename T, int N>
FastTrack<T,N> OptimizeTrack(Track<T,N>& input)
{
    FastTrack<T,N> result;
    result.SetInterpolation(input.GetInterpolation());
    unsigned int size = input.GetSize();
    result.Resize(size);
    for (unsigned int i = 0; i < size; ++i) {
        result[i] = input[i];
    }
    result.UpdateIndexLookupTable();
    return result;
}
