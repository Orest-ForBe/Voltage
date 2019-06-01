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
#include "FenceCorner.h"
#include "PCB.h"
#include "TransformerBox.h"

Renderer_t * Renderer;
Camera_t * Cam1;

Fence_t * f1;
FenceHC_t * f1g;
FenceCorner_t * fc;
PCB_t * pcb[100][100];
TransformerBox_t * trn;

#if defined(_WIN32)
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
  SDL_Event event;
  bool IsCloseWindow = false;

  Renderer = new Renderer_t();

  Renderer->Init("Voltage", 1366, 768, true);
  SDL_GL_SetSwapInterval(0);

  Renderer->LoadObject("fence", "models/fence_linear.ASE");
  Renderer->LoadObject("fence_s", "models/fence_simple.ASE");
  Renderer->LoadObject("fence_corner", "models/fence_corner.ASE");
  Renderer->LoadObject("pcb", "models/pcb.ASE");
  Renderer->LoadObject("Transformer", "models/Transformer_Box.ASE");

//  GLint ts = Renderer->getRCache().LoadTexture("TopScreen.png");
  GLint ts = Renderer->getRCache().LoadTexture("hud.png");

  Cam1 = new Camera_t(Renderer);
  Renderer->AddCamera(Cam1);

  Cam1->Pos.X = 30 * 7;
  Cam1->Pos.Y = 50 * 7;
  Cam1->Pos.Z = 50 * 7;

  Cam1->ViewTo.X = 5;
  Cam1->ViewTo.Y = 15;
  Cam1->ViewTo.Z = 0;

  trn = new TransformerBox_t(Renderer);
  trn->Pos.X = 60;
  trn->Pos.Z = 60;
  Renderer->AddModel("Transformer", trn);

  for (int i = 0; i < 50; i++)
    for (int j = 0; j < 50; j++)
    {
      pcb[i][j] = new PCB_t(Renderer);
      pcb[i][j]->Pos.X = 30 * i - 25 * 30;
      pcb[i][j]->Pos.Z = 30 * j - 25 * 30;
      Renderer->AddModel("pcb", pcb[i][j]);
    }

  for (int i = 1; i < 10; i++)
  {
    f1g = new FenceHC_t(Renderer);
    f1g->Pos.X = 60 * i - 30;
    Renderer->AddModel("fence_s", f1g);
    f1 = new Fence_t(Renderer);
    f1->Pos.X = 60 * i;
    Renderer->AddModel("fence", f1);
  }

  for (int i = 1; i < 10; i++)
  {
    f1g = new FenceHC_t(Renderer);
    f1g->Pos.Z = 60 * i - 30;
    f1g->Angle = 90.0f;
    Renderer->AddModel("fence_s", f1g);
    f1 = new Fence_t(Renderer);
    f1->Pos.Z = 60 * i;
    f1->Angle = 90;
    Renderer->AddModel("fence", f1);
  }

  fc = new FenceCorner_t(Renderer);
  fc->Angle = 0.0f;
  Renderer->AddModel("fence_corner", fc);



  float a = 0, b = 0;

  int state = 0;


  while (!IsCloseWindow)
  {
    Cam1->Pos.X = 0 + 30 * 7 * sin(a);
    Cam1->Pos.Y = 50 * 10 + 250 * cos(b);
    Cam1->Pos.Z = 0 + 50 * 7 * cos(a);

    Cam1->ViewTo.X = Cam1->Pos.X + 0.01;
    Cam1->ViewTo.Y = 0;
    Cam1->ViewTo.Z = Cam1->Pos.Z + 0.0;

    if (state == 0)
    {
      a = a + 0.01;
      if ((rand() % 100) == 50)
        state = 1;
    }
    else
    {
      b = b + 0.03;
      if (b > (2 * M_PI))
      {
        state = 0;
        b = 0;
      }
    }

    GLint viewport[4];

    Renderer->DrawScene();

    glDisable(GL_DEPTH_TEST);
    glGetIntegerv(GL_VIEWPORT, viewport);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(viewport[0], viewport[2], viewport[3], viewport[0], -1, 1);
    glMatrixMode(GL_MODELVIEW);
  //  glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, ts);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -0.1f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1366.0f, 0.0f, -0.1f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1366.0f, 768.0f, -0.1f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.0f, 768.0f, -0.1f);
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  //  glPopMatrix();
    glEnable(GL_DEPTH_TEST);



    Renderer->UpdateScreen();
    SDL_Delay(10);
    SDL_PumpEvents();


    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        IsCloseWindow = true;

      if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
        IsCloseWindow = true;

    }
  }
}
