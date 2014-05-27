#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <string>

#include <GL/glew.h>

#include "LinkError.h"

using std::cerr;
using std::endl;
using std::string;


class Mesh
{
   public:
      /**
        * Constructor.
        */
      Mesh();

      /**
        * Destructor. Deallocates memory.
        */
      virtual ~Mesh();
   
	//virtual float* getCoordinates;

      /**
        * Draw the mesh.
        */
      virtual void draw();
   
      void toggleWireFrame();
   
	virtual float * getCoordinates();
   
   float* coordinates;

   protected:
      /**
        * Bind to the context.
        */
      void bind();

      /**
        * Helper function to find vertex attribute locations.
        *
        * @param name The attribute's name in the shader source.
        *
        * @return The attribute's location as a GLuint.
        *
        * @throws LinkError If the attribute could not be found.
        */
      GLuint findAttribute(const char* name);

      /**
        * Amount of vertices in this mesh.
        */
      GLsizei _vertexCount;

   private:
      /**
        * Handle for the VAO.
        */
      GLuint _vao;
	
      bool wireFrame;
};

#endif
