#ifndef MODELLOADEDMESH_H
#define MODELLOADEDMESH_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

#include "Mesh.h"

#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stringstream; 

using glm::vec3;

class ModelLoadedMesh: public Mesh
{
   public:
      /**
        * Constructor.
        *
        * @param length Side length of the Cube.
        */
      ModelLoadedMesh(string fileName);
   
	

      /**
        * Draw the Cube.
        *
        * Overloaded so that we can draw indices.
        */
      virtual void draw();

   private:
	   
   string charArrayToString(char* characters);
   
   float stringToFloat(string str);
   
      /** Amount of indices in the mesh. */
      GLsizei _indexCount;
};

#endif

