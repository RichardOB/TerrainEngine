#include <GL/glew.h>

#include "Constants.h"
#include "Mesh.h"

Mesh::
Mesh():
   _vertexCount(0),
   _vao(0),
   wireFrame(false)
{
   glGenVertexArrays(1, &_vao);
   bind();
}

Mesh::
~Mesh()
{
   glDeleteVertexArrays(1, &_vao);
}

void Mesh::
draw()
{
   bind();
   glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
}

void Mesh::
bind()
{
   glBindVertexArray(_vao);
}

GLuint Mesh::
findAttribute(const char* name)
{
   GLint currentProgram;
   glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
   
   GLint location = glGetAttribLocation((GLuint)currentProgram, name);

   if (location == -1)
   {
      string sName(name);
      cerr << "[WARNING] Could not find attribute '" + sName + "'." << endl;
   }

   return (GLuint)location;
}

void Mesh::toggleWireFrame()
{
	if (!wireFrame)
	{
		//Switch openGL into wireframe drawing mode
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
		
		wireFrame = true;
	}
	else
	{
		//Switch openGL into wireframe drawing mode
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		
		wireFrame = false;
	}
	
}

