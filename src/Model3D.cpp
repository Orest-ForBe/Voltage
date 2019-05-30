/*
 * Model3D.cpp
 *
 *  Created on: Apr 8, 2019
 *      Author: tim
 */

#include "Model3D.h"
#include <GL/glew.h>
#include "GL/gl.h"
#include "GL/glut.h"
#include "math.h"

Model3D::Model3D(Renderer_t * prend)
{
  Pos.X = 0;
  Pos.Y = 0;
  Pos.Z = 0;
  Obj = NULL;
  Visible = true;
  CamMode = false;
  Rend = prend;
}

Model3D::~Model3D()
{
}

void Model3D::AssignObj3D(Obj3D* object)
{
  Obj = object;
}

void Model3D::DrawGeometry(Obj3D::Geom_t* g)
{
  glBindTexture(GL_TEXTURE_2D, g->TextureID);
  glBegin(GL_TRIANGLES);
  for (size_t i = 0; i < g->mesh.size(); i++)
  {
//    glTexCoord2f(fp1.X, 1.0 - fp1.Y);
//    glNormal3f(N.X, N.Y, N.Z);
//    glVertex3f(lp1.X, lp1.Y, lp1.Z);
//    glTexCoord2f(fp2.X, 1.0 - fp2.Y);
//    glVertex3f(lp2.X, lp2.Y, lp2.Z);
//    glTexCoord2f(fp3.X, 1.0 - fp3.Y);
//    glVertex3f(lp3.X, lp3.Y, lp3.Z);

    for (size_t j = 0; j < 3; j++)
    {
      glTexCoord2f(g->mesh[i]->T[j].X, g->mesh[i]->T[j].Y);
      glVertex3f(g->mesh[i]->P[j].X, g->mesh[i]->P[j].Y, g->mesh[i]->P[j].Z);
    }
  }
  glEnd();
}

bool Model3D::GetVisible(void)
{
  return Visible;
}

void Model3D::SetVisible(bool NewState)
{
  Visible = NewState;
}

bool Model3D::IsCamera(void)
{
  return CamMode;
}

Camera_t::Camera_t(Renderer_t * prend):Model3D(prend)
{
  Pos.X = 0;
  Pos.Y = 130;
  Pos.Z = 200;
  ViewTo.X = 0;
  ViewTo.Y = 10;
  ViewTo.Z = 0;
  CamMode = true;
}

Camera_t::~Camera_t()
{
}

void Camera_t::Draw(void)
{
  Obj3D::Geom_t * g = Obj ? Obj->GetMesh("Camera") : NULL;
  float pan, tilt;

  if (!GetVisible() && g)
  {
    glPushMatrix();
    glTranslatef(Pos.X, Pos.Y, Pos.Z);
    tilt = atan2f((Pos.Z - ViewTo.Z) , (Pos.X - ViewTo.X)) * 180.0f / M_PI;
    pan = asinf((Pos.Y - ViewTo.Y) / sqrtf((Pos.X - ViewTo.X) * (Pos.X - ViewTo.X) + (Pos.Y - ViewTo.Y) * (Pos.Y - ViewTo.Y) + (Pos.Z - ViewTo.Z) * (Pos.Z - ViewTo.Z))) * 180.0f / M_PI;
    glRotatef(-pan, 1.0f, 0.0f, 0.0f);
    glRotatef(-tilt, 0.0f, 1.0f, 0.0f);
    DrawGeometry(g);
    glPopMatrix();
    glBegin(GL_LINES);
    glVertex3f(Pos.X, Pos.Y, Pos.Z);
    glVertex3f(ViewTo.X, ViewTo.Y, ViewTo.Z);
    glEnd();
  }
  else if (GetVisible())
  {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(Pos.X, Pos.Y, Pos.Z, ViewTo.X, ViewTo.Y, ViewTo.Z, 0, 1, 0);
  }
}

