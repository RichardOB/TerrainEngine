#include "Texture.h"

Texture::Texture(const string& directory,
		const string& texName, bool selector):
   fileName(),
   textureHandle(0),
   rgba(selector)
{
	fileName = directory + "/" +  texName;
	
	//textureUnit = TextureUnit::getInstance()->getNextUnitEnum();
	//textureUnit = GL_TEXTURE1;
	//cout << "Texture Unit for Texture: "<< textureUnit <<endl;
	
	
	//use Texture Unit instantiated from Singleton Class
	//glActiveTexture(textureUnit);
	
	//Textures are also objects
	//This means, we need to generate one and bind it to the context.

	glGenTextures(1, & textureHandle);
	//bind the object to a target named GL_TEXTURE_CUBE_MAP.
	//GL_TEXTURE_CUBE_MAP: because our texture is a cube map texture. This means openGL will treat it as 6 different sub-textures instead of a single one.
	//glBindTexture(GL_TEXTURE_2D,  textureHandle);
	glBindTexture(GL_TEXTURE_2D,  textureHandle);
	//Tell openGL to enable texturing parts
	glEnable(GL_TEXTURE_2D);
	
	/*Select filters to use for Magnifying and Minifying. Possible filters:
	 * 1. point filter
	 * 2. bi-linear filter
	 * 3. tri-linear filter
	 * 4. anisotropic filter
	 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	//Decide what to do when our texture coordinates fall off the edge of the texture. We will just set this to wrap (repeat in openGL) so we are safe.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	/*Now we are going to start loading each of the textures*/
	
	//We need to create integers to store the width and height of a texture
	int width = 0;
	int height = 0;
	
	unsigned char* data;
	
	if (!rgba)
	{
		data = loadTextureFromPNG(fileName, width, height);
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		data = loadTextureRGBAFromPNG(fileName, width, height);
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	
	
	delete [] data;
}

Texture::~Texture()
{
   
}

void Texture::bind()
{
	//glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D,  textureHandle);
}

void Texture::load()
{
	glBindTexture(GL_TEXTURE_2D,  textureHandle);
	//Tell openGL to enable texturing parts

	//use Texture Unit instantiated from Singleton Class
	//glActiveTexture(textureUnit);
	
	//Textures are also objects
	//This means, we need to generate one and bind it to the context.

	glGenTextures(1, & textureHandle);
	//bind the object to a target named GL_TEXTURE_CUBE_MAP.
	//GL_TEXTURE_CUBE_MAP: because our texture is a cube map texture. This means openGL will treat it as 6 different sub-textures instead of a single one.
	//glBindTexture(GL_TEXTURE_2D,  textureHandle);
	
	/*Select filters to use for Magnifying and Minifying. Possible filters:
	 * 1. point filter
	 * 2. bi-linear filter
	 * 3. tri-linear filter
	 * 4. anisotropic filter
	 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	//Decide what to do when our texture coordinates fall off the edge of the texture. We will just set this to wrap (repeat in openGL) so we are safe.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	/*Now we are going to start loading each of the textures*/
	
	//We need to create integers to store the width and height of a texture
	int width = 0;
	int height = 0;
	
	unsigned char* data;
	
	if (!rgba)
	{
		data = loadTextureFromPNG(fileName, width, height);
	}
	else
	{
		data = loadTextureRGBAFromPNG(fileName, width, height);
	}
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	
	delete [] data;
}

unsigned char* Texture::loadTextureFromPNG(const string fileName, int& w, int& h)
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

GLubyte*  Texture::loadTextureRGBAFromPNG(const string filename, int& width, int& height)
{
	 FILE* file = fopen(filename.c_str(), "r");

   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, 
         NULL, NULL);
   if (png_ptr == NULL)
   {
      printf("Could not initialize libPNG's read struct.\n");
      exit(-1);
   }

   png_infop png_info_ptr = png_create_info_struct(png_ptr);
   if (png_info_ptr == NULL)
   {
      printf("Could not initialize libPNG's info pointer.\n");
      exit(-1);
   }

   /* We jump back here if an error is encountered. */
   if (setjmp(png_jmpbuf(png_ptr)))
   {
      printf("LibPNG encountered an error.\n");

      png_destroy_read_struct(&png_ptr, &png_info_ptr, NULL);
      exit(-1);
   }

   png_init_io(png_ptr, file);

   png_read_png(png_ptr, png_info_ptr, 0, NULL);

   png_uint_32 png_width = 0;
   png_uint_32 png_height = 0;
   int bits = 0;
   int colour_type = 0;
   png_get_IHDR(png_ptr, png_info_ptr, &png_width, &png_height, &bits, &colour_type, 
         NULL, NULL, NULL);

   const unsigned BITS_PER_BYTE = 8;
   unsigned bytes_per_colour = (unsigned)bits / BITS_PER_BYTE;
   unsigned colours_per_pixel;

   if (colour_type == PNG_COLOR_TYPE_RGBA)
   {
      colours_per_pixel = 4;
   }
   else
   {
      cout << "[ERROR] Cannot load " << filename << ", since it is not in "
         << "RGBA format." << endl;
      exit(-1);
   }

   cout << "[INFO] Loaded " << filename << " as PNG. Width = " << png_width
      << ", height = " << png_height << ", bit depth = " << bits
      << ", colour type = " << colour_type << "." << endl;

   unsigned char* data = new unsigned char[png_width * png_height * colours_per_pixel * 
      bytes_per_colour];

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
         data[index++] = row_pointers[y][x++];
      }
   }

   width = (int)png_width;
   height = (int)png_height;

   return data;
}



