/*
 * Renderer.cpp
 *
 *  Created on: Apr 8, 2019
 *      Author: tim
 */

#include "Renderer.h"
#include "RenderCache.h"
#include <algorithm>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL_image.h>

Renderer_t::Renderer_t()
{
  // TODO Auto-generated constructor stub
  RCache = new RenderCache_t();
  CurTexID = -1;
  gb = false;
}

Renderer_t::~Renderer_t()
{
  // TODO Auto-generated destructor stub
}

int Renderer_t::LoadObject(const std::string ObjectName, const std::string & filename)
{
  RCache->LoadObject(ObjectName, filename);
  return 0;
}

int Renderer_t::AddModel(const std::string & ObjectName, Model3D * Model)
{
  //todo: thread safe
  Model->AssignObj3D(RCache->Find(ObjectName));
  Scene.push_back(Model);
  return 0;
}

void Renderer_t::DelModel(Model3D* Model)
{
  //todo: thread safe
  Scene.erase(std::remove(Scene.begin(), Scene.end(), Model), Scene.end());
}

void Renderer_t::AddCamera(Camera_t* Camera)
{
  //todo: thread safe
  Camera->AssignObj3D(RCache->Find("Camera"));
  for (size_t i = 0; i < Scene.size(); i++)
    if (Scene[i]->IsCamera())
      Scene[i]->SetVisible(false);
  Scene.insert(Scene.begin(), Camera);
}

void Renderer_t::Init(const std::string & winname, int width, int height, bool fullscreen)
{
  //  SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  //  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

  win = SDL_CreateWindow(winname.c_str(),
  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL/* | SDL_WINDOW_FULLSCREEN| SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI*/);
  glContext = SDL_GL_CreateContext(win);
  SDL_GetWindowSize(win, &win_width, &win_height);

  glEnable(GL_MULTISAMPLE);
  glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

  glAlphaFunc(GL_ALWAYS, 0.1);
  glEnable(GL_ALPHA_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA_SATURATE);

  IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP);

#ifdef __WIN32__
  glewExperimental = TRUE;
#endif
  glewInit();

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
//  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.f, 16.0 / 9.0/*1.f*/, 1.f, 10000.f);

  Shd = new Shader("shaders/texture.vs", "shaders/texture.fs");
}


void Renderer_t::DrawAxis(void)
{
  glLineWidth(2);
  glColor3ub(255, 0, 0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(100.0, 0.0, 0.0);
  glVertex3f(85.0, 2.5, 0.0);
  glVertex3f(85.0, -2.5, 0.0);
  glVertex3f(100.0, 0.0, 0.0);
  glVertex3f(85.0, 0.0, 2.5);
  glVertex3f(85.0, 0.0, -2.5);
  glVertex3f(100.0, 0.0, 0.0);
  glEnd();
  glBegin(GL_LINES);
  glVertex3f(105.0, 0.0, -2.5);
  glVertex3f(110.0, 0.0, 2.5);
  glVertex3f(105.0, 0.0, 2.5);
  glVertex3f(110.0, 0.0, -2.5);
  glEnd();
  glColor3ub(0, 255, 0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 100.0, 0.0);
  glVertex3f(0.0, 85.0, 2.5);
  glVertex3f(0.0, 85.0, -2.5);
  glVertex3f(0.0, 100.0, 0.0);
  glVertex3f(2.5, 85.0, 0.0);
  glVertex3f(-2.5, 85.0, 0.0);
  glVertex3f(0.0, 100.0, 0.0);
  glEnd();
  glBegin(GL_LINES);
  glVertex3f(0.0, 107.5, 0.0);
  glVertex3f(0.0, 110.0, 2.5);
  glVertex3f(0.0, 107.5, 0.0);
  glVertex3f(0.0, 110.0, -2.5);
  glVertex3f(0.0, 105.0, 0.0);
  glVertex3f(0.0, 107.5, 0.0);
  glEnd();
  glColor3ub(0, 0, 255);
  glBegin(GL_LINE_STRIP);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 100.0);
  glVertex3f(2.5, 0.0, 85.0);
  glVertex3f(-2.5, 0.0, 85.0);
  glVertex3f(0.0, 0.0, 100.0);
  glVertex3f(0.0, 2.5, 85.0);
  glVertex3f(0.0, -2.5, 85.0);
  glVertex3f(0.0, 0.0, 100.0);
  glEnd();
  glBegin(GL_LINE_STRIP);
  glVertex3f(2.5, 1.0, 110.0);
  glVertex3f(-2.5, 1.0, 110.0);
  glVertex3f(2.5, 1.0, 105.0);
  glVertex3f(-2.5, 1.0, 105.0);
  glEnd();
  glLineWidth(1);
}

void Renderer_t::DrawScene(void)
{
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_TEXTURE_2D);
  // todo: before draw scene event
//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
  DrawAxis();

  glEnable(GL_TEXTURE_2D);
  glColor3f(1.0f, 1.0f, 1.0f);

  CurTexID = -1;
  gb = true;

  for (size_t i = 0; i < Scene.size(); i++)
    Scene[i]->Draw();

  if (gb)
  {
//    glEnd();
//    gb = false;
  }
}

void Renderer_t::UpdateScreen(void)
{
  SDL_GL_SwapWindow(win);
}

void Renderer_t::BindTexture(GLint TexID)
{
  if (TexID !=CurTexID)
  {
//    if (gb)
//    {
//      glEnd();
//      gb = false;
//    }
    CurTexID = TexID;
    glBindTexture(GL_TEXTURE_2D, TexID);
//    glBegin(GL_TRIANGLES);
//    gb = true;
  }
}
