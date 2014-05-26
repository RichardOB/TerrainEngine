#include "Board.h"


Board::
Board(float lengthX, float lengthY, float lengthZ):
   _indexCount(0)
{
   _vertexCount = 8;

   const unsigned VERTEX_COMPONENT_COUNT = 3;
   const unsigned VERTEX_ARRAY_SIZE = (unsigned)_vertexCount *
      VERTEX_COMPONENT_COUNT;
   float* vertices = new float[VERTEX_ARRAY_SIZE];
   float* uvVertices = new float [_vertexCount * 2];

   const unsigned SQUARES = 2;
   const unsigned TRIANGLES_PER_SQUARE = 2;
   const unsigned VERTICES_PER_TRIANGLE = 3;
   const unsigned VERTICES_PER_SQUARE = TRIANGLES_PER_SQUARE * 
      VERTICES_PER_TRIANGLE;
   _indexCount = (GLsizei)VERTICES_PER_SQUARE * (GLsizei)SQUARES;

   const unsigned INDEX_ARRAY_SIZE = (unsigned)_indexCount;
   GLuint* indices = new GLuint[INDEX_ARRAY_SIZE];
   GLuint* uvIndices = new GLuint[INDEX_ARRAY_SIZE];

   int i = 0;
   
   vertices[i++] = 0.0f;
   vertices[i++] = 0.0f;
   vertices[i++] = lengthZ/2;

   vertices[i++] = lengthX;
   vertices[i++] = 0.0f;
   vertices[i++] = lengthZ/2;

   vertices[i++] = 0.0f;
   vertices[i++] = lengthY;
   vertices[i++] = lengthZ/2;

   vertices[i++] = lengthX;
   vertices[i++] = lengthY;
   vertices[i++] = lengthZ/2;
   
   //Face 2
   vertices[i++] = lengthX/2;
   vertices[i++] = 0.0f;
   vertices[i++] = 0.0f;

   vertices[i++] = lengthX/2;
   vertices[i++] = 0.0f;
   vertices[i++] = lengthZ;

   vertices[i++] = lengthX/2;
   vertices[i++] = lengthY;
   vertices[i++] = 0.0f;

   vertices[i++] = lengthX/2;
   vertices[i++] = lengthY;
   vertices[i++] = lengthZ;
   
   i = 0;
   
   uvVertices[i++] = 0.0f;
   uvVertices[i++] = 1.0f;

   uvVertices[i++] = 1.0f;
   uvVertices[i++] = 1.0f;

   uvVertices[i++] = 0.0f;
   uvVertices[i++] = 0.0f;

   uvVertices[i++] = 1.0f;
   uvVertices[i++] = 0.0f;
   
   //Face 2
   uvVertices[i++] = 0.0f;
   uvVertices[i++] = 1.0f;

   uvVertices[i++] = 1.0f;
   uvVertices[i++] = 1.0f;

   uvVertices[i++] = 0.0f;
   uvVertices[i++] = 0.0f;

   uvVertices[i++] = 1.0f;
   uvVertices[i++] = 0.0f;


  i = 0;

   // Bottom. 
   indices[i++] = 0;
   indices[i++] = 1;
   indices[i++] = 2;
   indices[i++] = 1;
   indices[i++] = 3;
   indices[i++] = 2;
   
    // Bottom. 
   indices[i++] = 4;
   indices[i++] = 5;
   indices[i++] = 6;
   indices[i++] = 5;
   indices[i++] = 7;
   indices[i++] = 6;
   
     i = 0;

   // Bottom. 
   uvIndices[i++] = 0;
   uvIndices[i++] = 1;
   uvIndices[i++] = 2;
   uvIndices[i++] = 1;
   uvIndices[i++] = 3;
   uvIndices[i++] = 2;
   
   uvIndices[i++] = 4;
   uvIndices[i++] = 5;
   uvIndices[i++] = 6;
   uvIndices[i++] = 5;
   uvIndices[i++] = 7;
   uvIndices[i++] = 6;


   bind();

   GLuint buffers[4];
   glGenBuffers(4, buffers);

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
	 
	 
   glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
   glBufferData(GL_ARRAY_BUFFER, _vertexCount * 2 * (GLsizei)sizeof(float),
         uvVertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_ARRAY_SIZE * (GLsizei)sizeof(GLuint),
         uvIndices, GL_STATIC_DRAW);

   GLuint uvLocation = findAttribute("uv");
   glEnableVertexAttribArray(uvLocation);
   glVertexAttribPointer(uvLocation, 2, GL_FLOAT,
         GL_FALSE, 0, 0);
}

void Board::draw()
{
	bind();
	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, NULL);
}
