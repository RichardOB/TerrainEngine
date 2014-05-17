#include "CubeMapTexture.h"

CubeMapTexture::
CubeMapTexture(const string& directory,
		const string& posXName, const string& negXName, 
		const string& posYName, const string& negYName, 
		const string& posZName, const string& negZName):
   fileNames(),
   textureHandle(0),
   textureUnit(0)
{
	fileNames[0] = directory + "/" +  posXName;
	fileNames[1] = directory + "/" +  negXName;
	fileNames[2] = directory + "/" +  posYName;
	fileNames[3] = directory + "/" +  negYName;
	fileNames[4] = directory + "/" +  posZName;
	fileNames[5] = directory + "/" +  negZName;
	
	textureUnit = TextureUnit::getInstance()->getNextUnitEnum();
	cout << "Texture Unit for Cube Map Texture: "<< textureUnit <<endl;
}

CubeMapTexture::
~CubeMapTexture()
{
   
}

void CubeMapTexture::bind()
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP,  textureHandle);
}

void CubeMapTexture::load()
{
	//Tell openGL to enable texturing parts
	glEnable(GL_TEXTURE_CUBE_MAP);
	//use Texture Unit instantiated from Singleton Class
	glActiveTexture(textureUnit);
	
	//Textures are also objects
	//This means, we need to generate one and bind it to the context.

	glGenTextures(1, & textureHandle);
	//bind the object to a target named GL_TEXTURE_CUBE_MAP.
	//GL_TEXTURE_CUBE_MAP: because our texture is a cube map texture. This means openGL will treat it as 6 different sub-textures instead of a single one.
	glBindTexture(GL_TEXTURE_CUBE_MAP,  textureHandle);
	
	/*Select filters to use for Magnifying and Minifying. Possible filters:
	 * 1. point filter
	 * 2. bi-linear filter
	 * 3. tri-linear filter
	 * 4. anisotropic filter
	 */
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	//Decide what to do when our texture coordinates fall off the edge of the texture. We will just set this to wrap (repeat in openGL) so we are safe.
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	/*Now we are going to start loading each of the textures*/
	
	//We need to create integers to store the width and height of a texture
	int width = 0;
	int height = 0;
	
	unsigned char* data;
	
	data = loadTextureFromPNG(fileNames[0], width, height);
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
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	//Repeat for other textures:
	data = loadTextureFromPNG(fileNames[2], width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	data = loadTextureFromPNG(fileNames[4], width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	data = loadTextureFromPNG(fileNames[1], width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	data = loadTextureFromPNG(fileNames[3], width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	data = loadTextureFromPNG(fileNames[5], width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	
	//Delete this temporary memory store after loading the file
	delete [] data;
}

unsigned char* CubeMapTexture::loadTextureFromPNG(const string fileName, int& w, int& h)
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




