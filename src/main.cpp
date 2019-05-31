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
#include "Fence.h"
#include "FenceHC.h"

Renderer_t * Renderer;
Camera_t * Cam1;

Fence_t * f1;
FenceHC_t * f1g;

#if defined(_WIN32)
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
  Renderer = new Renderer_t();

  Renderer->Init("Voltage", 800, 600, false);
  SDL_GL_SetSwapInterval(1);

  Renderer->LoadObject("fence", "models/fence.ASE");
  Renderer->LoadObject("fence_hc", "models/fence_h1.ASE");

  Cam1 = new Camera_t(Renderer);
  Renderer->AddCamera(Cam1);

  Cam1->Pos.X = 150;
  Cam1->Pos.Y = 250;
  Cam1->Pos.Z = 450;

  Cam1->ViewTo.X = 50;
  Cam1->ViewTo.Y = 150;
  Cam1->ViewTo.Z = 0;


  f1 = new Fence_t(Renderer);
  Renderer->AddModel("fence", f1);

  f1g = new FenceHC_t(Renderer);
  Renderer->AddModel("fence_hc", f1g);

  while (1)
  {
    Renderer->DrawScene();
    Renderer->UpdateScreen();
    SDL_PumpEvents();
  }
}
