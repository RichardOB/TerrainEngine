#ifndef QUAD_H
#define QUAD_H

#include <GL/glew.h>

#include "Constants.h"
#include "Mesh.h"

/**
  * This class encapsulates a vertex array that can be used to draw a quad.
  *
  * This quad will be a unit quad on the XZ plane. It will be centred around
  * the origin.
  */
class Quad: public Mesh
{
   public:
      /**
        * Default constructor. Allocates memory and initializes buffers.
        */
      Quad();
};

#endif

