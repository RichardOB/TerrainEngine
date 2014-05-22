#ifndef COLOURBUFFER_H
#define COLOURBUFFER_H

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

class ColourBuffer
{
	public:
     
		ColourBuffer(GLsizei w, GLsizei h);

		~ColourBuffer();
	
		void write();
	
		void bind();
	
		void unbind();

     
	private:
	   
		GLsizei width;
		GLsizei height;	
	
		GLuint handle;
	   
	
      
};

#endif

