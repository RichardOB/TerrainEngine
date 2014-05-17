#include <stddef.h>
#include "TextureUnit.h"

/*Global static pointer to ensure a single instance of class*/
TextureUnit* TextureUnit::TextureUnitInstance = NULL;

TextureUnit* TextureUnit::getInstance()
{
	if (!TextureUnitInstance)
	{
		TextureUnitInstance = new TextureUnit;
	}
	
	return TextureUnitInstance;
}

TextureUnit::TextureUnit():count(0)
{

}

GLenum TextureUnit::getNextUnitEnum()
{
	if (GL_TEXTURE0 + count == GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
	{
		cout << "ERROR: Max Texture Units Reached" << endl;
	}
	
	GLenum nextUnit = GL_TEXTURE0 + count;
	
	count = count + 1;
	
	return nextUnit;
}