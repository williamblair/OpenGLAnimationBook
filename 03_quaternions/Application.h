#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <iostream>
#include <cstdlib>

#include <GL/glew.h>
#include <SDL2/SDL.h>

class Application
{
public:

    inline Application() :
        window(nullptr),
        quit(false)
    {
    }
    inline virtual ~Application() {
        Shutdown();
    }
    inline virtual bool Initialize(const char* title, const int width, const int height) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << __func__ << ": failed to init SDL: " << SDL_GetError() << std::endl;
            return false;
        }

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
        if (!window) {
            std::cerr << __func__ << ": failed to create window: " << SDL_GetError() << std::endl;
            return false;
        }
    
        // set opengl options
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
        // create a render context
        gc = SDL_GL_CreateContext(window);
    
        // enable vsync
        SDL_GL_SetSwapInterval(1);
    
        // init glew
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            std::cerr << __func__ << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
            return false;
        } else {
            std::cout << "Init GLEW, version: " << glewGetString(GLEW_VERSION) << std::endl;
        }

        return true;
    }
    inline virtual void Update(float inDeltaTime) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_GL_SwapWindow(window);
    }
    inline virtual void Render(float inAspectRatio) {}
    inline virtual void Shutdown() {
        if (window != nullptr) {
           SDL_GL_DeleteContext(gc);
           SDL_DestroyWindow(window);
           SDL_Quit();
           window = nullptr;
        }
    }
    inline virtual bool ShouldQuit() const {
        return quit;
    }
private:

    // disallow copy
    Application(const Application&);
    Application& operator=(const Application&);

    SDL_Window* window;
    SDL_GLContext gc;
    bool quit;
};

#endif // APPLICATION_H_INCLUDED

