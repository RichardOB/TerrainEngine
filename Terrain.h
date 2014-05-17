#ifndef TERRAIN_H
#define TERRAIN_H

#include <iostream>
using std::cout;
using std::endl;
#include <string>


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-pedantic"
#include "png++/png.hpp"
#include "libs/glm/glm.hpp"
#pragma GCC diagnostic pop
using namespace png;

#include "Constants.h"
#include "Mesh.h"
#include "RuntimeError.h"

using glm::cross;
using glm::normalize;
using glm::vec3;
//using png::image;
//using png::rgb_pixel;
using std::max;
using std::min;
using std::string;

/**
  * Encapsulates terrain.
  */
class Terrain: public Mesh
{
   public:
      /**
        * Constructor, creates the mesh.
        *
        * @param heightmap The filename of a heightmap to use when generating this
        * terrain.
        */
      Terrain(string heightmap);
   
	void initVariables();
   
	void createGeometry();
   
    /**
        * Draw the grid.
        *
        * Overloaded so that we can draw indices.
        */
      virtual void draw();
   
   float* getCoord(unsigned x, unsigned z);
   
   void smoothVertices();
   
   void smoothPNG();
   
   void readPNG();
	
	
float* getRGB(int x, int z);

   private:
	
   const unsigned COMPONENTS;

      unsigned width;
      /** Depth of the heightmap. */
      unsigned height;
      /** Heightmap. */
       image<rgb_pixel> heightMap;
       
       /** Amount of indices in the mesh. */
      GLsizei _indexCount;
   
   GLfloat* vertices;
   
   float** heights;

};

#endif

