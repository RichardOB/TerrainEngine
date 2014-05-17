#include <cmath>
#include <cstdlib>

#include "Quad.h"

Quad::
Quad():
   Mesh()
{
   _vertexCount = 6;
   
   GLfloat* positions = new float[_vertexCount * 3];
   GLfloat* normals = new float[_vertexCount * 3];

   {
      unsigned index = 0;

      positions[index + X] = -.5f;
      positions[index + Y] = .0f;
      positions[index + Z] = .5f;
      index += W ;

      positions[index + X] = .5f;
      positions[index + Y] = .0f;
      positions[index + Z] = .5f;
      index += W;

      positions[index + X] = -.5f;
      positions[index + Y] = .0f;
      positions[index + Z] = -.5f;
      index += W;

      positions[index + X] = -.5f;
      positions[index + Y] = .0f;
      positions[index + Z] = -.5f;
      index += W;

      positions[index + X] = .5f;
      positions[index + Y] = .0f;
      positions[index + Z] = .5f;
      index += W;

      positions[index + X] = .5f;
      positions[index + Y] = .0f;
      positions[index + Z] = -.5f;
   }

   for (int i = 0; i < _vertexCount; i++)
   {
      int index = i * W;
      normals[index + X] = 0.0f;
      normals[index + Y] = 1.0f;
      normals[index + Z] = 0.0f;
   }

   const GLsizei VERTEX_ARRAY_SIZE = _vertexCount * 3 * (GLsizei)sizeof(float);

   bind();
   GLuint buffers[2];
   glGenBuffers(2, buffers);

   glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, VERTEX_ARRAY_SIZE, positions, GL_STATIC_DRAW);

   GLuint positionLocation = findAttribute("position");
   glEnableVertexAttribArray(positionLocation);
   glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

   glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
   glBufferData(GL_ARRAY_BUFFER, VERTEX_ARRAY_SIZE, normals, GL_STATIC_DRAW);

   GLuint normalLocation = findAttribute("normal");
   glEnableVertexAttribArray(normalLocation);
   glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

   delete[] positions;
}

