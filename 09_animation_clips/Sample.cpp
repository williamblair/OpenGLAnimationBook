#include <Sample.h>
#include <vector>
#include <cstdarg>
    

bool Sample::Initialize(const char* title, const int width, const int height)
{
    if (!Application::Initialize(title, width, height)) {
        return false;
    }

    scalarTracks.push_back(MakeScalarTrack(Interpolation::Linear, 2, MakeFrame(0.0f, 0.0f), MakeFrame(1.0f, 1.0f)));
    scalarTracksLooping.push_back(false);

    scalarTracks.push_back(MakeScalarTrack(Interpolation::Linear, 2, MakeFrame(0.0f, 0.0f), MakeFrame(0.5f, 1.0f)));
    scalarTracksLooping.push_back(false);

    scalarTracks.push_back(MakeScalarTrack(Interpolation::Linear, 3, MakeFrame(0.25f, 0.0f), MakeFrame(0.5f, 1.0f), MakeFrame(0.75f, 0.0f)));
    scalarTracksLooping.push_back(true);
    
    scalarTracks.push_back(MakeScalarTrack(Interpolation::Linear, 3, MakeFrame(0.25f, 0.0f), MakeFrame(0.5f, 1.0f), MakeFrame(0.75f, 0.0f)));
    scalarTracksLooping.push_back(false);

    ScalarTrack stepTrack; stepTrack.Resize(11); stepTrack.SetInterpolation(Interpolation::Constant);
    for (int i = 0; i < 11; i++) {
        float time = (float)i / 10.0f * 0.5f + 0.25f;
        stepTrack[i] = MakeFrame(time, (i % 2 == 0.0f) ? 0.0f : 1.0f);
    }
    scalarTracks.push_back(stepTrack);
    scalarTracks.push_back(stepTrack);
    scalarTracksLooping.push_back(true);
    scalarTracksLooping.push_back(false);

    scalarTracks.push_back(MakeScalarTrack(Interpolation::Cubic, 2, MakeFrame(0.25f, 0.676221f, 0.0f, 0.676221f), MakeFrame(0.75f, 4.043837f, 1.0f, 4.043837f)));
    scalarTracksLooping.push_back(false);

    scalarTracks.push_back(MakeScalarTrack(Interpolation::Cubic, 3, MakeFrame(0.25f, 0, 0, 0), MakeFrame(0.5f, 0, 1, 0), MakeFrame(0.75f, 0, 0, 0)));
    scalarTracksLooping.push_back(true);

    scalarTracks.push_back(MakeScalarTrack(Interpolation::Cubic, 5, MakeFrame(0.25f, 0, 0, 0), MakeFrame(0.3833333f, -10.11282f, 0.5499259f, -10.11282f), MakeFrame(0.5f, 25.82528f, 1, 25.82528f), MakeFrame(0.6333333f, 7.925411f, 0.4500741f, 7.925411f), MakeFrame(0.75f, 0, 0, 0)));
    scalarTracksLooping.push_back(true);

    scalarTracks.push_back(MakeScalarTrack(Interpolation::Cubic, 5, MakeFrame(0.25f, 0, 0, 0), MakeFrame(0.3833333f, 13.25147f, 0.5499259f, -10.11282f), MakeFrame(0.5f, 10.2405f, 1, -5.545671f), MakeFrame(0.6333333f, 7.925411f, 0.4500741f, -11.40672f), MakeFrame(0.75f, 0, 0, 0)));
    scalarTracksLooping.push_back(true);

    scalarTrackLines = new DebugDraw();
    handlePoints = new DebugDraw();
    handleLines = new DebugDraw();
    referenceLines = new DebugDraw();

    // range in xy is 0 to 20
    for (unsigned int i = 0; i < 10; i++) {
        float yPosition = (i * 2.0f)  + ((float)i * 0.2f) + 0.1f;
        float height = 1.8f;

        float left = 1.0f;
        float right = 14.0f;

        referenceLines->Push(Vec3(left, yPosition, 0));
        referenceLines->Push(Vec3(left, yPosition + height, 0));

        referenceLines->Push(Vec3(left, yPosition, 0));
        referenceLines->Push(Vec3(right, yPosition, 0));
    }
    referenceLines->UpdateOpenGLBuffers();

    for (unsigned int i = 0; i < (unsigned int)scalarTracks.size(); ++i) {
        float yPosition = ((9 - i) * 2.0f) + ((float)(9 - i) * 0.2f) + 0.1f;
        float height = 1.8f;

        float left = 1.0f;
        float right = 14.0f;
        float xRange = right - left;

        for (unsigned int j = 1; j < 150; ++j) {
            float thisJNorm = (float)(j - 1) / 149.0f;
            float nextJNorm = (float)(j) / 149.0f;

            float thisX = left + thisJNorm * xRange;
            float nextX = left + nextJNorm * xRange;
            
            float thisY = scalarTracks[i].Sample(thisJNorm, scalarTracksLooping[i]);
            float nextY = scalarTracks[i].Sample(nextJNorm, scalarTracksLooping[i]);

            thisY = yPosition + thisY * height;
            nextY = yPosition + nextY * height;

            scalarTrackLines->Push(Vec3(thisX, thisY, 0.1f));
            scalarTrackLines->Push(Vec3(nextX, nextY, 0.1f));
        }

        unsigned int numFrames = scalarTracks[i].GetSize();
        for (unsigned int j = 0; j < numFrames; ++j) {
            float thisTime = scalarTracks[i][j].time;
            float thisY = yPosition + scalarTracks[i].Sample(thisTime, scalarTracksLooping[i]) * height;
            float thisX = left + thisTime * xRange;
            handlePoints->Push(Vec3(thisX, thisY, 0.9f));

            if (j > 0) {
                float prevY = yPosition + scalarTracks[i].Sample(thisTime - 0.0005f, scalarTracksLooping[i]) * height;
                float prevX = left + (thisTime - 0.0005f) * xRange;

                Vec3 thisVec = Vec3(thisX, thisY, 0.6f);
                Vec3 prevVec = Vec3(prevX, prevY, 0.6f);
                Vec3 handleVec = thisVec + normalized(prevVec - thisVec) * 0.75f;

                handleLines->Push(thisVec);
                handleLines->Push(handleVec);
            }

            if (j < numFrames - 1 && scalarTracks[i].GetInterpolation() != Interpolation::Constant) {
                float nextY = yPosition + scalarTracks[i].Sample(thisTime + 0.0005f, scalarTracksLooping[i]) * height;
                float nextX = left + (thisTime + 0.0005f) * xRange;

                Vec3 thisVec = Vec3(thisX, thisY, 0.6f);
                Vec3 nextVec = Vec3(nextX, nextY, 0.6f);
                Vec3 handleVec = thisVec + normalized(nextVec - thisVec) * 0.75f;

                handleLines->Push(thisVec);
                handleLines->Push(handleVec);
            }
        }
    }

    scalarTrackLines->UpdateOpenGLBuffers();
    handleLines->UpdateOpenGLBuffers();
    handlePoints->UpdateOpenGLBuffers();

    return true;
}

void Sample::Update(float inDeltaTime)
{
    Application::Update(inDeltaTime);
}

void Sample::Render(float inAspectRatio)
{
    Mat4 model;
    Mat4 view = lookAt(Vec3(0,0,5), Vec3(0,0,0), Vec3(0,1,0));
    Mat4 projection = ortho(0, inAspectRatio * 22.0f, 0, 22, 0.001f, 10);

    Mat4 mvp = projection * view * model;
    
    referenceLines->Draw(DebugDrawMode::Lines, Vec3(1,1,1), mvp);
    scalarTrackLines->Draw(DebugDrawMode::Lines, Vec3(0,1,0), mvp);
    handleLines->Draw(DebugDrawMode::Lines, Vec3(0,1,0), mvp);
    handlePoints->Draw(DebugDrawMode::Points, Vec3(0,0,1), mvp);
}

void Sample::Shutdown()
{
    scalarTracks.clear();
    scalarTracksLooping.clear();

    delete scalarTrackLines;
    delete handlePoints;
    delete handleLines;
    delete referenceLines;

    Application::Shutdown();
}

ScalarFrame Sample::MakeFrame(float time, float value)
{
    return MakeFrame(time, 0.0f, value, 0.0f);
}
VectorFrame Sample::MakeFrame(float time, const Vec3& value)
{
    return MakeFrame(time, Vec3(), value, Vec3());
}
QuaternionFrame Sample::MakeFrame(float time, const Quat& value)
{
    return MakeFrame(time, Quat(0,0,0,0), value, Quat(0,0,0,0));
}

ScalarFrame Sample::MakeFrame(float time, float in, float value, float out)
{
    ScalarFrame result;
    result.time = time;
    result.in[0] = in;
    result.value[0] = value;
    result.out[0] = out;
    return result;
}
VectorFrame Sample::MakeFrame(float time, const Vec3& in, const Vec3& value, const Vec3& out)
{
    VectorFrame result;
    result.time = time;
    result.in[0] = in.x; result.in[1] = in.y; result.in[2] = in.z;
    result.value[0] = value.x; result.value[1] = value.y; result.value[2] = value.z;
    result.out[0] = out.x; result.out[1] = out.y; result.out[2] = out.z;
    return result;
}
QuaternionFrame Sample::MakeFrame(float time, const Quat& in, const Quat& out, const Quat& value)
{
    QuaternionFrame result;
    result.time = time;
    result.in[0] = in.x; result.in[1] = in.y; result.in[2] = in.z; result.in[3] = in.w;
    result.value[0] = value.x; result.value[1] = value.y; result.value[2] = value.z; result.value[3] = value.w;
    result.out[0] = out.x; result.out[1] = out.y; result.out[2] = out.z; result.out[3] = out.w;
    return result;
}

ScalarTrack Sample::MakeScalarTrack(Interpolation interp, int numFrames, ...)
{
    ScalarTrack result;
    result.SetInterpolation(interp);
    result.Resize(numFrames);

    va_list args;
    va_start(args, numFrames);

    for (int i = 0; i < numFrames; i++) {
        result[i] = va_arg(args, ScalarFrame);
    }

    va_end(args);

    return result;
}

VectorTrack Sample::MakeVectorTrack(Interpolation interp, int numFrames, ...)
{
    VectorTrack result;
    result.SetInterpolation(interp);
    result.Resize(numFrames);

    va_list args;
    va_start(args, numFrames);

    for (int i = 0; i < numFrames; i++) {
        result[i] = va_arg(args, VectorFrame);
    }

    va_end(args);

    return result;
}

QuaternionTrack Sample::MakeQuaternionTrack(Interpolation interp, int numFrames, ...)
{
    QuaternionTrack result;
    result.SetInterpolation(interp);
    result.Resize(numFrames);

    va_list args;
    va_start(args, numFrames);

    for (int i = 0; i < numFrames; i++) {
        result[i] = va_arg(args, QuaternionFrame);
    }

    va_end(args);

    return result;
}

