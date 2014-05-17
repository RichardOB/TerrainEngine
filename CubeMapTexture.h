#ifndef CUBEMAPTEXTURE_H
#define CUBEMAPTEXTURE_H

#include <iostream>
#include <string>

#include <GL/glew.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-pedantic"
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#pragma GCC diagnostic pop

#include "png.h"

#include "TextureUnit.h"

using glm::mat4;
using glm::value_ptr;
using std::cerr;
using std::cout;
using std::endl;
using std::string;

class CubeMapTexture
{
   public:
      /**
        * Constructor.
        *
        * Obtains directory name containing desired textures, aswell as the texture filenames themselves.
        */
	CubeMapTexture(const string& directory, 
		const string& posXName, const string& negXName, 
		const string& posYName, const string& negYName, 
		const string& posZName, const string& negZName);

      /**
        * Destructor.
        */
      ~CubeMapTexture();

      /**
        * Load the texture.
        */
	void load();
   
	void bind();

   private:
	   
	unsigned char* loadTextureFromPNG(const string fileName, int& w, int& h);
      
	string fileNames[6];
   
	GLuint textureHandle;
   
	GLenum textureUnit;
   
};

#endif

