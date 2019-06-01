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

}

Fence_t::~Fence_t()
{
  // TODO Auto-generated destructor stub
}

void Fence_t::Draw(void)
{
  Obj3D::Geom_t * g = Obj->GetMesh("fence");

  glPushMatrix();
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  glTranslatef(0.0f, 50.0f, 150.0f);
  glTranslatef(Pos.X, Pos.Y, Pos.Z);
  DrawGeometry(g);
  glPopMatrix();
}
