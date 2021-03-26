#include <Sample.h>
#include <vector>
#include <cstdarg>

#define GIZMO_SIZE 0.25f
    
static inline float deg2rad(float inDeg) {
    return inDeg * (M_PI/180.0f);
}

bool Sample::Initialize(const char* title, const int width, const int height)
{
    if (!Application::Initialize(title, width, height)) {
        return false;
    }

    solver.Resize(6);
    solver[0] = Transform(Vec3(), angleAxis(deg2rad(90.0f), Vec3(1,0,0)), Vec3(1,1,1));
    solver[1] = Transform(Vec3(0, 0, 1.0f), Quat(), Vec3(1,1,1));
    solver[2] = Transform(Vec3(0, 0, 1.5f), Quat(), Vec3(1,1,1));
    solver[3] = Transform(Vec3(0, 0, 0.5f), angleAxis(deg2rad(90.0f), Vec3(0,1,0)), Vec3(1,1,1));
    solver[4] = Transform(Vec3(0, 0, 0.5f), Quat(), Vec3(1,1,1));
    solver[5] = Transform(Vec3(0, 0, 0.5f), Quat(), Vec3(1,1,1));

    target.position = Vec3(1, -2, 0);

    VectorTrack& path = targetPath.GetPositionTrack();
    path.Resize(14);
    SetFrame(path, 0, 0.0f, Vec3(1, -2, 0) * 0.5f);
    SetFrame(path, 1, 1.0f, Vec3(1,  2, 0) * 0.5f);
    SetFrame(path, 2, 2.0f, Vec3(1, 4, 0) * 0.5f);
    SetFrame(path, 3, 3.0f, Vec3(3, 4, 0) * 0.5f);
    SetFrame(path, 4, 4.0f, Vec3(5, 4, 0) * 0.5f);
    SetFrame(path, 5, 5.0f, Vec3(5, 4, 2) * 0.5f);
    SetFrame(path, 6, 6.0f, Vec3(5, 4, 4) * 0.5f);
    SetFrame(path, 7, 7.0f, Vec3(3, 4, 4) * 0.5f);
    SetFrame(path, 8, 8.0f, Vec3(3, 2, 4) * 0.5f);
    SetFrame(path, 9, 9.0f, Vec3(3, 2, 2) * 0.5f);
    SetFrame(path, 10, 10.0f, Vec3(1, 2, 2) * 0.5f);
    SetFrame(path, 11, 11.0f, Vec3(1, 0, 2) * 0.5f);
    SetFrame(path, 12, 12.0f, Vec3(1, -2, 2) * 0.5f);
    SetFrame(path, 13, 13.0f, Vec3(1, -2, 0) * 0.5f);

    solverLines = new DebugDraw();
    solverPoints = new DebugDraw();
    targetVisual[0] = new DebugDraw(2);
    targetVisual[1] = new DebugDraw(2);
    targetVisual[2] = new DebugDraw(2);

    camPitch = 45.0f;
    camYaw = 60.0f;
    camDist = 7.0f;
    playTime = 0.0f;

    return true;
}

void Sample::SetFrame(VectorTrack& track, int index, float time, const Vec3& value)
{
    track[index].value[0] = value.x;
    track[index].value[1] = value.y;
    track[index].value[2] = value.z;
    track[index].time = time;
}

void Sample::Update(float inDeltaTime)
{
    Application::Update(inDeltaTime);

    playTime += inDeltaTime;
    if (playTime > targetPath.GetEndTime()) {
        playTime -= targetPath.GetEndTime();
    }
    target = targetPath.Sample(target, playTime, true);
    solver.Solve(target);
}

void Sample::Render(float inAspectRatio)
{
    Vec3 cameraPose(
        camDist * cosf(deg2rad(camYaw)) * sinf(deg2rad(camPitch)),
        camDist * cosf(deg2rad(camPitch)),
        camDist * sinf(deg2rad(camYaw)) * sinf(deg2rad(camPitch))
    );

    Mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
    Mat4 view = lookAt(cameraPose, Vec3(0,0,0), Vec3(0,1,0));
    Mat4 mvp = projection * view; // no model

    solverLines->LinesFromIKSolver(solver);
    solverPoints->PointsFromIKSolver(solver);

    solverLines->UpdateOpenGLBuffers();
    solverPoints->UpdateOpenGLBuffers();

    solverLines->Draw(DebugDrawMode::Lines, Vec3(1,0,1), mvp);
    solverPoints->Draw(DebugDrawMode::Points, Vec3(1,0,1), mvp);

    (*targetVisual[0])[0] = target.position + Vec3(GIZMO_SIZE, 0, 0);
    (*targetVisual[1])[0] = target.position + Vec3(0, GIZMO_SIZE, 0);
    (*targetVisual[2])[0] = target.position + Vec3(0, 0, GIZMO_SIZE);
    (*targetVisual[0])[1] = target.position - Vec3(GIZMO_SIZE, 0, 0);
    (*targetVisual[1])[1] = target.position - Vec3(0, GIZMO_SIZE, 0);
    (*targetVisual[2])[1] = target.position - Vec3(0, 0, GIZMO_SIZE);
    
    targetVisual[0]->UpdateOpenGLBuffers();
    targetVisual[1]->UpdateOpenGLBuffers();
    targetVisual[2]->UpdateOpenGLBuffers();

    targetVisual[0]->Draw(DebugDrawMode::Lines, Vec3(1,0,0), mvp);
    targetVisual[1]->Draw(DebugDrawMode::Lines, Vec3(0,1,0), mvp);
    targetVisual[2]->Draw(DebugDrawMode::Lines, Vec3(0,0,1), mvp);
}

void Sample::Shutdown()
{
    Application::Shutdown();
    
    delete solverLines;
    delete solverPoints;
    delete targetVisual[0];
    delete targetVisual[1];
    delete targetVisual[2];
}

