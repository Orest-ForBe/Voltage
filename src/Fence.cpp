/*
 * Fence.cpp
 *
 *  Created on: May 31, 2019
 *      Author: tim
 */

#include "Fence.h"

Fence_t::Fence_t(Renderer_t * prend) : Model3D(prend)
{
  // TODO Auto-generated constructor stub
  Angle = 0;
}

Fence_t::~Fence_t()
{
  // TODO Auto-generated destructor stub
}

void Fence_t::Draw(void)
{
  Obj3D::Geom_t * g = Obj->GetMesh("fence_linear");

  glPushMatrix();
  glTranslatef(Pos.X, Pos.Y, Pos.Z);
  glRotatef(Angle, 0.0f, 1.0f, 0.0f);
  DrawGeometry(g);
  glPopMatrix();
}
