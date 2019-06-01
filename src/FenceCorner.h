/*
 * FenceCorner.h
 *
 *  Created on: Jun 1, 2019
 *      Author: tim
 */

#ifndef FENCECORNER_H_
#define FENCECORNER_H_

#include "Model3D.h"

class FenceCorner_t: public Model3D
{
public:
  FenceCorner_t(Renderer_t * prend);
  virtual ~FenceCorner_t();

  virtual void Draw(void);

  float Angle;
};

#endif /* FENCECORNER_H_ */
