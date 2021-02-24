#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <iostream>
#include <cstdlib>

#include <GL/glew.h>
#include <SDL2/SDL.h>

class Application
{
public:

    Application();
    virtual ~Application();
    
    virtual bool Initialize(const char* title, const int width, const int height);
    virtual void Update(float inDeltaTime);
    virtual void Render(float inAspectRatio);
    virtual void Shutdown();
    
    inline virtual bool ShouldQuit() const { return quit; }
    
private:

    // disallow copy
    Application(const Application&);
    Application& operator=(const Application&);

    SDL_Window* window;
    SDL_GLContext gc;
    bool quit;
};

#endif // APPLICATION_H_INCLUDED

