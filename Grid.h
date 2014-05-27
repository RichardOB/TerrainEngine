#ifndef GRID_H
#define GRID_H

#include <cmath>

#include "Mesh.h"

/**
  * Encapsulates a grid-shaped mesh on the XZ plane.
  *
  * The mesh will stretch from the origin (0, 0, 0) to a point in the positive X
  * and Z sub spaces (length, 0, length).
  */
class Grid: public Mesh
{
   public:
      /**
        * Constructor.
        *
        * @param length Side length of the grid.
        * @param samples Amount of squares on each axis.
        */
      Grid(float length, unsigned resolution);

      /**
        * Draw the grid.
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

