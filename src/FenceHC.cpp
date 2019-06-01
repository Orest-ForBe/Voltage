/*
 * FenceHC.cpp
 *
 *  Created on: May 31, 2019
 *      Author: tim
 */

#include "FenceHC.h"

FenceHC_t::FenceHC_t(Renderer_t * prend) : Model3D(prend)
{
  // TODO Auto-generated constructor stub
  Angle = 0;
}

FenceHC_t::~FenceHC_t()
{
  // TODO Auto-generated destructor stub
}

void FenceHC_t::Draw(void)
{
  Obj3D::Geom_t * g = Obj->GetMesh("fence_simple");

  glPushMatrix();
  glTranslatef(Pos.X, Pos.Y, Pos.Z);
  glRotated(Angle, 0.0f, 1.0f, 0.0f);
  DrawGeometry(g);
  glPopMatrix();
}
