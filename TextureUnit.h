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

using std::cout;
using std::endl;
using std::string;

class TextureUnit
{
	public:
		/*Only function that can call constructor*/
		static TextureUnit* getInstance();
		
		GLenum getNextUnitEnum();
	
	private:
		
		/*Constructor. Private so that it cannot be called*/
		TextureUnit();
		
		/*Copy constructor is private*/
		TextureUnit(TextureUnit const&){};
			
		/*Assignment operator is private*/
		//TextureUnit& operator=(TextureUnit const&){};
		
		static TextureUnit* TextureUnitInstance;

		GLuint count = 0;
	
};