/*
 * Renderer.h
 *
 *  Created on: Apr 8, 2019
 *      Author: tim
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#ifdef __WIN32__
#include <windows.h>
#endif
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "Model3D.h"
#include "shader.h"

class RenderCache_t;

class Renderer_t
{
private:
  SDL_Window *win;
  SDL_GLContext glContext;

  RenderCache_t * RCache;
  std::vector<Model3D *> Scene;
  GLint CurTexID;
  bool gb;

public:
  Renderer_t();
  virtual ~Renderer_t();

  void Init(const std::string & winname, int width, int height, bool fullscreen);

  virtual int LoadObject(const std::string ObjectName, const std::string & filename);

  virtual int AddModel(const std::string & ObjectName, Model3D * Model);
  virtual void DelModel(Model3D * Model);

  virtual void AddCamera(Camera_t * Camera);

  virtual void DrawScene(void);
  virtual void UpdateScreen(void);

  virtual void BindTexture(GLint TexID);

  virtual void DrawAxis(void);

  virtual RenderCache_t & getRCache(void) { return *RCache;}

  int win_width;
  int win_height;

  struct nk_context *ctx;

  Shader * Shd;



};

#endif /* RENDERER_H_ */
