#include "ColourBuffer.h"

ColourBuffer::
ColourBuffer(GLsizei w, GLsizei h):
	width(w),
	height(h),
	handle(0)
{
	//Create buffers
	glGenFramebuffers(1, &handle);
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
	
	//Make texture for our frame buffers to store stuff in
	glActiveTexture(GL_TEXTURE10);
	
	GLuint textureHandle;
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	
	//We create our image in a slightly different way:
	//1. It has a different format (we only store distance from light source
	//2. We won't be loading in any data (since we will be rendering to it)
	//NB: Depth of fragment will be distance from the light source, in NDC
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	
	//Set some texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width,height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	
	
	//Now attach the texture to the frame buffer:
	//1. Target
	//2. Where we want to attach the texture
	//3. What type of texture we are about to pass in
	//4. Texture's id
	//5. Layer argument (Used for mipmapping)
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureHandle, 0);
	
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

ColourBuffer::
~ColourBuffer()
{
  
}

void ColourBuffer::
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
   image.write("screenshot.png");
   
   unbind();

}

void ColourBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

void ColourBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

