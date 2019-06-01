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
//  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
//  glTranslatef(0.0f, 50.0f, 150.0f);
  glTranslatef(Pos.X, Pos.Y, Pos.Z);
  DrawGeometry(g);
  glPopMatrix();
}
