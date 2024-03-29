#ifndef INDEXED_MESH_H
#define INDEXED_MESH_H

#include "Mesh.h"

class IndexedMesh: public Mesh
{
   public:
      /**
        * Draw the grid.
        *
        * Overloaded so that we can draw indices.
        */
      virtual void draw();
   
      virtual float* getCoordinates();

   protected:
      /**
        * Protected constructor so we can't instantiate.
        */
      IndexedMesh();

      /** Amount of indices in the index buffer. */
      GLsizei _indexCount;
};

#endif

