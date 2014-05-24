#include "CubeMap.h"

CubeMap::
CubeMap(GLsizei w, GLsizei h):
	width(w),
	height(h),
	handle(0),
	textureHandle(0)
{
	//Create buffers
	glGenFramebuffers(1, &handle);
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
	
	//Make texture for our frame buffers to store stuff in
	glActiveTexture(GL_TEXTURE10);
	
	
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandle);//GL_TEXTURE_CUBE_MAP
	
	//Set some texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width,height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	/*
	 * 1. The texture target. We are actually saying here what part of the cube map we are loading.
	 * 2. Level of detail - used for mipmapping. SIcne we are not mipmapping anything, we set to 0.
	 * 3. Format to store image on graphics card
	 * 4. width
	 * 5. height
	 * 6. border?
	 * 7. Format in which image is stored in RAM: We pick GL_RGB because that is the format our PNG were stored in.
	 * 8. Data type our image is stored in - We used unsigneds.
	 * 9. Pointer to actual data.
	 */
	
	unsigned char* data;
	
	data = loadTextureFromPNG("textures/skybox/terrain_positive_x.png", width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	data = loadTextureFromPNG("textures/skybox/terrain_positive_y.png", width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	
	data = loadTextureFromPNG("textures/skybox/terrain_positive_z.png", width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	
	data = loadTextureFromPNG("textures/skybox/terrain_negative_x.png", width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	
	data = loadTextureFromPNG("textures/skybox/terrain_negative_y.png", width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	
	data = loadTextureFromPNG("textures/skybox/terrain_negative_z.png", width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	
	GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		switch (status)
		{
		 case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		    //throw RuntimeError("Incomplete framebuffer attachment.");
			cout << "[ERROR]: Incomplete framebuffer attachment." << endl;
		 case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		    //throw RuntimeError("No color attachment in draw buffer.");
			cout << "[ERROR]: No color attachment in draw buffer." << endl;
		 case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		   // throw RuntimeError("No color attachment in read buffer.");
			cout << "[ERROR]: No color attachment in read buffer." << endl;
		 case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		    //throw RuntimeError("Framebuffer is missing an attachment.");
			cout << "[ERROR]: Framebuffer is missing an attachment." << endl;
		 case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		    //throw RuntimeError("The framebuffer different size multisamples.");
			cout << "[ERROR]: The framebuffer different size multisamples." << endl;
		 case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		    //throw RuntimeError("Not all framebuffer attachments are layered.");
			cout << "[ERROR]: Not all framebuffer attachments are layered." << endl;
		 case GL_FRAMEBUFFER_UNSUPPORTED:
		    //throw RuntimeError("The framebuffer configuration is unsupported.");
			cout << "[ERROR]: The framebuffer configuration is unsupported." << endl;
		 default:
		    
		    //throw RuntimeError("Unknown framebuffer error.");
			cout << "[ERROR]: Unknown framebuffer errort." << endl;
		}
		
		exit(-1);
	}
	
	unbind();
}

CubeMap::
~CubeMap()
{
  
}

void  CubeMap::attachToNewTexture(int face)
{
	if (face < 0 || face > 5)
	{
		cout << "[ERROR] Cannot attach a texture face of value '" << face << "' in CubeMap." << endl;
		exit(-1);
	}
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + (GLuint)face, textureHandle, 0);
}

void CubeMap::
write()
{
	bind();
	
  const GLsizei DATA_SIZE = width * height * 
      (GLsizei)sizeof(GL_UNSIGNED_BYTE) * (GLsizei)4;
   GLubyte* data = new GLubyte[(unsigned)DATA_SIZE];

   glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

   png::image< png::rgb_pixel > image((long unsigned)width, (long unsigned)height);
   GLsizei index = 0;
   for (GLsizei y = 0; y < height; y++)
   {
      for (GLsizei x = 0; x < width; x++)
      {
         image[(size_t)(height - y - 1)][(size_t)x] =
            png::rgb_pixel(data[index], data[index+1], data[index+2]);
         index += 4;
      }
   }
   image.write("rgb.png");
   
   unbind();

}

void CubeMap::
write(int id)
{
	//bind();
	
  const GLsizei DATA_SIZE = width * height * 
      (GLsizei)sizeof(GL_UNSIGNED_BYTE) * (GLsizei)4;
   GLubyte* data = new GLubyte[(unsigned)DATA_SIZE];

   glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

   png::image< png::rgb_pixel > image((long unsigned)width, (long unsigned)height);
   GLsizei index = 0;
   for (GLsizei y = 0; y < height; y++)
   {
      for (GLsizei x = 0; x < width; x++)
      {
         image[(size_t)(height - y - 1)][(size_t)x] =
            png::rgb_pixel(data[index], data[index+1], data[index+2]);
         index += 4;
      }
   }
   
   switch (id)
   {
      case 1:
	image.write("rgb1.png");
      break;
      
      case 2:
	image.write("rgb2.png");
      break;
      
      case 3:
	image.write("rgb3.png");
      break;
      
      case 4:
	image.write("rgb4.png");
      break;
      
      case 5:
	image.write("rgb5.png");
      break;
      
      case 6:
	image.write("rgb6.png");
      break;
      
      default:
	 image.write("rgb.png");  
	break;
      }
   
  // unbind();

}

void CubeMap::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
}

void CubeMap::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

 void CubeMap::apply()
{
	//Tell openGL to enable texturing parts
	glEnable(GL_TEXTURE_CUBE_MAP);
	//use Texture Unit instantiated from Singleton Class
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_CUBE_MAP,  textureHandle);
	glUniform1i (1, 10);
}

unsigned char* CubeMap::loadTextureFromPNG(const string fileName, int& w, int& h)
{
	FILE* file = fopen(fileName.c_str(), "r");
	 
	png_structp png_ptr = 
		png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	
	if (png_ptr == NULL)
	{
		printf("Could not initialise libPNG's read struct.\n");
		exit (-1);
	}
	
	png_infop png_info_ptr = png_create_info_struct(png_ptr);

	if (png_info_ptr == NULL)
	{
		printf ("Could not initialise libPNG's info pointer.\n");
		exit(-1);
	}
	
	/*We jump back here if an error is encountered. */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf ("LibPNG encountered an error.\n");
		
		png_destroy_read_struct(&png_ptr, &png_info_ptr, NULL);
		exit(-1);
	}
	
	png_init_io(png_ptr, file);
		
	/**/png_read_png(png_ptr, png_info_ptr, 0 , NULL);
	printf("test\n");
	png_uint_32 png_width = 0;
	png_uint_32 png_height = 0;
	int bits = 0;
	int colour_type = 0;
	png_get_IHDR(png_ptr, png_info_ptr, &png_width, &png_height, &bits, &colour_type, NULL, NULL, NULL);
	
	const unsigned BITS_PER_BYTE = 8;
	unsigned bytes_per_colour = (unsigned)bits / BITS_PER_BYTE;
	unsigned colours_per_pixel;
	
	if (colour_type == PNG_COLOR_TYPE_RGB)
	{
		colours_per_pixel = 3;
	}
	else 
	{
		printf ("Colour types other that RGB are not supported.");
		exit (-1);
	}
	
	printf ("png_width = %d, png_height = %d, bits = %d, colour type = %d.\n", (int)png_width, (int)png_height, bits, colour_type);
	
	unsigned char* data = new unsigned char[png_width * png_height * colours_per_pixel * bytes_per_colour];
	
	png_bytepp row_pointers = png_get_rows(png_ptr, png_info_ptr);
	
	unsigned index = 0;
	
	for (unsigned y = 0; y < png_height; y++)
	{
		unsigned x = 0;
		
		while (x < png_width * colours_per_pixel * bytes_per_colour)
		{
			data[index++] = row_pointers[y][x++];
			data[index++] = row_pointers[y][x++];
			data[index++] = row_pointers[y][x++];
		}
		
	}
	
	w = (int)png_width;
	h = (int) png_height;
	
	return data;
	
}



