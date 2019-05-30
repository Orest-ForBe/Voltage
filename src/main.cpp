//============================================================================
// Voltage 2019
//============================================================================

#include <iostream>
#include "Renderer.h"
#include "LoaderASE.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL_image.h>
#include <math.h>

Renderer_t * Renderer;
Camera_t * Cam1;

#if defined(_WIN32)
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
  Renderer = new Renderer_t();

  Renderer->Init("Voltage", 800, 600, false);
  SDL_GL_SetSwapInterval(1);

  Cam1 = new Camera_t(Renderer);
  Renderer->AddCamera(Cam1);

  while (1)
  {
    Renderer->DrawScene();
    Renderer->UpdateScreen();
    SDL_PumpEvents();
  }
}
