/*
 * FenceCorner.cpp
 *
 *  Created on: Jun 1, 2019
 *      Author: tim
 */

#include "FenceCorner.h"

FenceCorner_t::FenceCorner_t(Renderer_t * prend) : Model3D(prend)
{
  // TODO Auto-generated constructor stub
  Angle = 0;
}

FenceCorner_t::~FenceCorner_t()
{
  // TODO Auto-generated destructor stub
}

void FenceCorner_t::Draw(void)
{
  Obj3D::Geom_t * g = Obj->GetMesh("fence_corner");

  glPushMatrix();
  glTranslatef(Pos.X, Pos.Y, Pos.Z);
  glRotatef(Angle, 0.0f, 1.0f, 0.0f);
  DrawGeometry(g);
  glPopMatrix();
}
