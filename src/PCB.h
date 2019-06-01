/*
 * PCB.h
 *
 *  Created on: Jun 1, 2019
 *      Author: tim
 */

#ifndef PCB_H_
#define PCB_H_

#include "Model3D.h"

class PCB_t: public Model3D
{
public:
  PCB_t(Renderer_t * prend);
  virtual ~PCB_t();

  virtual void Draw(void);
};

#endif /* PCB_H_ */
