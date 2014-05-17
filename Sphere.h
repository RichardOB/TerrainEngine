#ifndef SPHERE_H
#define SPHERE_H

#include <GL/glew.h>

#include "Mesh.h"

/**
  * This class encapsulates a vertex array that can be used to draw a sphere.
  */
class Sphere: public Mesh
{
   public:
      /**
        * Default constructor. Allocates memory and initializes buffers.
        *
        * @param resolution The amount of times to subdivide the sphere.
        */
      Sphere(int resolution = 72);

   private:
      /** Degrees in a revolution. */
      const static int REVOLUTION = 360;
      /** Where to start the X angle from. */
      const static int X_ANGLE_START = 0;
      /** Where to stop the X angle. */
      const static int X_ANGLE_STOP = 360;
      /** Where to start the Y angle from. */
      const static int Y_ANGLE_START = -90;
      /** Where to stop the Y angle. */
      const static int Y_ANGLE_STOP = 90;

      /** Resolution of the sphere. */
      int _resolution;
};

#endif

