#include <Application.h>
#include <Vec2.h>
#include <Vec3.h>
#include <Vec4.h>
#include <Mat4.h>
#include <Quat.h>
#include <Transform.h>
#include <Shader.h>
#include <Attribute.h>
#include <Uniform.h>
#include <IndexBuffer.h>
#include <Draw.h>
#include <Texture.h>
#include <Sample.h>
#include <GLTFLoader.h>
#include <Bezier.h>
#include <Hermite.h>
#include <Interpolation.h>
#include <Frame.h>
#include <Track.h>
#include <TransformTrack.h>
#include <DebugDraw.h>
#include <Pose.h>
#include <Clip.h>
#include <Skeleton.h>
#include <Mesh.h>

#ifdef __WIN32
    #undef main
#endif

Application* gApplication       = nullptr;
GLuint       gVertexArrayObject = 0;

static void quit() {
    if (gApplication != nullptr) delete gApplication;
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
    const int appWidth = 640;
    const int appHeight = 480;
    const float appAspect = (float)appWidth / (float)appHeight;
    const bool vsyncEnabled = true;
    
    gApplication = new Sample();
    if (!gApplication->Initialize("Hello World", appWidth, appHeight)) quit();

    // OpenGL 3.3 core requires a VAO be bound for all draw calls
    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    // main loop
    Uint32 lastTicks = SDL_GetTicks();
    do
    {
        glViewport(0, 0, appWidth, appHeight);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glPointSize(5.0f);
        glBindVertexArray(gVertexArrayObject);
        glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        gApplication->Render(appAspect);

        if (vsyncEnabled) {
            glFinish(); // wait until queued GL commands have finished
        }

        // update time passed
        Uint32 newTicks = SDL_GetTicks();
        Uint32 ticksPassed = newTicks - lastTicks;
        float dtSeconds = ((float)ticksPassed) * 0.001f; // milliseconds to seconds
        lastTicks = newTicks;

        gApplication->Update(dtSeconds);
    } while (!gApplication->ShouldQuit());

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &gVertexArrayObject);
    gVertexArrayObject = 0;

    gApplication->Shutdown();
    quit();
    return 0;
}

