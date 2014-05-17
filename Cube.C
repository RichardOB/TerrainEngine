#include "Cube.h"


Cube::
Cube(float length):
   _indexCount(0)
{
   _vertexCount = 8;
	
	length = length/2;

   const unsigned VERTEX_COMPONENT_COUNT = 3;
   const unsigned VERTEX_ARRAY_SIZE = (unsigned)_vertexCount *
      VERTEX_COMPONENT_COUNT;
   float* vertices = new float[VERTEX_ARRAY_SIZE];

   const unsigned SQUARES = 6;
   const unsigned TRIANGLES_PER_SQUARE = 2;
   const unsigned VERTICES_PER_TRIANGLE = 3;
   const unsigned VERTICES_PER_SQUARE = TRIANGLES_PER_SQUARE * 
      VERTICES_PER_TRIANGLE;
   _indexCount = (GLsizei)VERTICES_PER_SQUARE * (GLsizei)SQUARES;

   const unsigned INDEX_ARRAY_SIZE = (unsigned)_indexCount;
   GLuint* indices = new GLuint[INDEX_ARRAY_SIZE];

   int i = 0;
   
   vertices[i++] = -length;
   vertices[i++] = -length;
   vertices[i++] = -length;

   vertices[i++] = -length;
   vertices[i++] = -length;
   vertices[i++] = length;

   vertices[i++] = length;
   vertices[i++] = -length;
   vertices[i++] = length;

   vertices[i++] = length;
   vertices[i++] = -length;
   vertices[i++] = -length;

   vertices[i++] = -length;
   vertices[i++] = length;
   vertices[i++] = -length;

   vertices[i++] = -length;
   vertices[i++] = length;
   vertices[i++] = length;

   vertices[i++] = length;
   vertices[i++] = length;
   vertices[i++] = length;

   vertices[i++] = length;
   vertices[i++] = length;
   vertices[i++] = -length;

  i = 0;

   // Back. 
   indices[i++] = 3;
   indices[i++] = 0;
   indices[i++] = 4;
   indices[i++] = 3;
   indices[i++] = 4;
   indices[i++] = 7;

   //Front. 
   indices[i++] = 1;
   indices[i++] = 6;
   indices[i++] = 5;
   indices[i++] = 1;
   indices[i++] = 2;
   indices[i++] = 6;

   // Bottom. 
   indices[i++] = 2;
   indices[i++] = 1;
   indices[i++] = 0;
   indices[i++] = 2;
   indices[i++] = 0;
   indices[i++] = 3;

   //Left. 
   indices[i++] = 0;
   indices[i++] = 1;
   indices[i++] = 5;
   indices[i++] = 0;
   indices[i++] = 5;
   indices[i++] = 4;

   // Top. 
   indices[i++] = 5;
   indices[i++] = 7;
   indices[i++] = 4;
   indices[i++] = 5;
   indices[i++] = 6;
   indices[i++] = 7;

   // Right.
   indices[i++] = 2;
   indices[i++] = 7;
   indices[i++] = 6;
   indices[i++] = 2;
   indices[i++] = 3;
   indices[i++] = 7;

   bind();

   GLuint buffers[2];
   glGenBuffers(2, buffers);

   glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, VERTEX_ARRAY_SIZE * (GLsizei)sizeof(float),
         vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_ARRAY_SIZE * (GLsizei)sizeof(GLuint),
         indices, GL_STATIC_DRAW);

   GLuint positionLocation = findAttribute("position");
   glEnableVertexAttribArray(positionLocation);
   glVertexAttribPointer(positionLocation, VERTEX_COMPONENT_COUNT, GL_FLOAT,
         GL_FALSE, 0, 0);
}

void Cube::draw()
{
	bind();
	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, NULL);
}
