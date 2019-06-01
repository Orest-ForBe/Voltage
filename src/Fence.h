/*
 * Fence.h
 *
 *  Created on: May 31, 2019
 *      Author: tim
 */

#ifndef FENCE_H_
#define FENCE_H_

#include "Model3D.h"

class Fence_t: public Model3D
{
public:
  Fence_t(Renderer_t * prend);
  virtual ~Fence_t();

  virtual void Draw(void);

  float Angle;
};

#endif /* FENCE_H_ */
