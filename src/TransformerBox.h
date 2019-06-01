/*
 * TransformerBox.h
 *
 *  Created on: Jun 1, 2019
 *      Author: tim
 */

#ifndef TRANSFORMERBOX_H_
#define TRANSFORMERBOX_H_

#include "Model3D.h"

class TransformerBox_t: public Model3D
{
public:
  TransformerBox_t(Renderer_t * prend);
  virtual ~TransformerBox_t();


  virtual void Draw(void);
};

#endif /* TRANSFORMERBOX_H_ */
