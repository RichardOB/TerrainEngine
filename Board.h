#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>

#include <cmath>

#include "Mesh.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;

class Board: public Mesh
{
   public:
      /**
        * Constructor.
        *
        * @param length Side length of the Cube.
        */
      Board(float lengthX, float lengthY, float lengthZ);

      /**
        * Draw the Cube.
        *
        * Overloaded so that we can draw indices.
        */
      virtual void draw();

   private:
      /** Amount of indices in the mesh. */
      GLsizei _indexCount;
};

#endif

