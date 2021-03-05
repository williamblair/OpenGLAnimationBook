#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

// used to store keyframes in an animation track
template<unsigned int N>
class Frame
{
public:
    float value[N];
    float in[N];    // in and out tangents
    float out[N];
    float time;
};

typedef Frame<1> ScalarFrame;
typedef Frame<3> VectorFrame;
typedef Frame<4> QuaternionFrame;

#endif // FRAME_H_INCLUDED

