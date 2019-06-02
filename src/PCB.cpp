/*
 * PCB.cpp
 *
 *  Created on: Jun 1, 2019
 *      Author: tim
 */

#include "PCB.h"

PCB_t::PCB_t(Renderer_t * prend) : Model3D(prend)
{
  // TODO Auto-generated constructor stub
}

PCB_t::~PCB_t()
{
  // TODO Auto-generated destructor stub
}

void PCB_t::Draw(void)
{
  Obj3D::Geom_t * g = Obj->GetMesh("pcb");

  glPushMatrix();
  glTranslatef(Pos.X, Pos.Y, Pos.Z);
  DrawGeometry(g);
  glPopMatrix();
}
