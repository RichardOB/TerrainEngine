#ifndef CUBE_H
#define CUBE_H

#include <iostream>
#include <string>

#include <cmath>

#include "Mesh.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;

class Cube: public Mesh
{
   public:
      /**
        * Constructor.
        *
        * @param length Side length of the Cube.
        */
      Cube(float length);

      /**
        * Draw the Cube.
        *
        * Overloaded so that we can draw indices.
        */
      virtual void draw();
   
      virtual float* getCoordinates();

   private:
      /** Amount of indices in the mesh. */
      GLsizei _indexCount;
};

#endif

