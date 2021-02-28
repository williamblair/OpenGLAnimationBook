#ifndef SAMPLE_H_INCLUDED
#define SAMPLE_H_INCLUDED

#include <Application.h>

#include <Mat4.h>
#include <Quat.h>
#include <Vec2.h>
#include <Vec3.h>
#include <Vec4.h>

#include <Shader.h>
#include <Attribute.h>
#include <Uniform.h>
#include <IndexBuffer.h>
#include <Texture.h>
#include <Draw.h>
#include <DebugDraw.h>
#include <Track.h>
#include <Interpolation.h>
#include <Frame.h>

class Sample : public Application
{
public:
    
    bool Initialize(const char* title, const int width, const int height);
    void Update(float inDeltaTime);
    void Render(float inAspectRatio);
    void Shutdown();

private:

    std::vector<ScalarTrack> scalarTracks;
    std::vector<bool> scalarTracksLooping;

    DebugDraw* scalarTrackLines;
    DebugDraw* handleLines;
    DebugDraw* handlePoints;
    DebugDraw* referenceLines;

    ScalarFrame MakeFrame(float time, float value);
    ScalarFrame MakeFrame(float time, float in, float value, float out);
    VectorFrame MakeFrame(float time, const Vec3& value);
    VectorFrame MakeFrame(float time, const Vec3& in, const Vec3& value, const Vec3& out);
    QuaternionFrame MakeFrame(float time, const Quat& value);
    QuaternionFrame MakeFrame(float time, const Quat& in, const Quat& out, const Quat& value);
    ScalarTrack MakeScalarTrack(Interpolation interp, int numFrames, ...);
    VectorTrack MakeVectorTrack(Interpolation interp, int numFrames, ...);
    QuaternionTrack MakeQuaternionTrack(Interpolation interp, int numFrames, ...);
};

#endif // SAMPLE_H_INCLUDED

