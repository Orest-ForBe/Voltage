/*
 * FenceHC.h
 *
 *  Created on: May 31, 2019
 *      Author: tim
 */

#ifndef FENCEHC_H_
#define FENCEHC_H_

#include "Model3D.h"

class FenceHC_t: public Model3D
{
public:
  FenceHC_t(Renderer_t * prend);
  virtual ~FenceHC_t();

  virtual void Draw(void);
};

#endif /* FENCEHC_H_ */
