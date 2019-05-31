/*
 * Model3D.h
 *
 *  Created on: Apr 8, 2019
 *      Author: tim
 */

#ifndef MODEL3D_H_
#define MODEL3D_H_

#include "RenderCache.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Renderer_t;

// todo: Move to global deftypes:
typedef struct
{
  float X;
  float Y;
  float Z;
} V3f;

class Model3D
{
private:
protected:
  Obj3D * Obj;
  Renderer_t * Rend;
  bool Visible;
  bool CamMode;
public:
  V3f Pos;
  Model3D(Renderer_t * prend);
  virtual ~Model3D();

  virtual void AssignObj3D(Obj3D * object);

  virtual void Draw(void) = 0;
  virtual void DrawGeometry(Obj3D::Geom_t * g);

  virtual bool GetVisible(void);
  virtual void SetVisible(bool NewState);
  virtual bool IsCamera(void);
};

class Camera_t: public Model3D
{
private:
protected:
public:
  V3f ViewTo;
  Camera_t(Renderer_t * prend);
  virtual ~Camera_t();

  virtual void Draw(void);
};

#endif /* MODEL3D_H_ */
