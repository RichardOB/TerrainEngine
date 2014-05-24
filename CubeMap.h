#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <iostream>

#include <GL/glew.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-pedantic"
#include "png++/png.hpp"
#pragma GCC diagnostic pop

using std::cerr;
using std::cout;
using std::endl;
using std::string;

class CubeMap
{
	public:
     
		CubeMap(GLsizei w, GLsizei h);

		~CubeMap();
	
		void write();
	
		void write(int);
	
		void bind();
	
		void unbind();
	
		void attachToNewTexture(int face);
	
		unsigned char* loadTextureFromPNG(const string fileName, int& w, int& h);

     
	private:
	   
		GLsizei width;
		GLsizei height;	
	
		GLuint handle;
	
		GLuint textureHandle;
	   
	
      
};

#endif

