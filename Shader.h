#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-pedantic"
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#pragma GCC diagnostic pop

#include "CompilationError.h"
#include "LinkError.h"

using glm::mat4;
using glm::value_ptr;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Shader
{
   public:
      /**
        * Constructor.
        *
        * Obtains OpenGL handles, loads shader program source, compiles and 
        * links.
        *
        * @param name The shader's name. This is the name of the directory that 
        * will be used as a prefix when loading the shader program source.
        */
      Shader(const string& name);

      /**
        * Destructor.
        *
        * Releases OpenGL handles.
        */
      ~Shader();

      /**
        * Apply the shader.
        *
        * This will activate the shader on the graphics card's execution units 
        * and load any uniforms it's using.
        */
      void apply();

      /**
        * Helper function to find uniform values.
        *
        * @param name The uniform's name in the shader source.
        *
        * @return The uniform's location as a GLint.
        *
        * @throws LinkError If the uniform could not be found.
        */
      GLint findUniform(const char* name);
      
      /**
        * Update the shader's projection matrix.
        *
        * @param matrix The projection matrix to pass to the shader program.
        */
      void updateProjectionMatrix(mat4 matrix);

      /**
        * Update the shader's view matrix.
        *
        * @param matrix The view matrix to pass to the shader program.
        */
      void updateViewMatrix(mat4 matrix);

      /**
        * Update the shader's world matrix.
        *
        * @param matrix The world matrix to pass to the shader program.
        */
      void updateWorldMatrix(mat4 matrix);

      /**
        * Update an arbitrary uniform. Prints a warning if the uniform could
        * not be found.
        *
        * @param name The uniform's name in the shader program.
        * @param value The uniform's new value.
        */
      void updateUniform(const char* name, float value);

   private:
      /** The OpenGL handle for this shader program. */
      unsigned _programHandle;
      /** The OpenGL handle for the vertex shader. */
      unsigned _vertexShaderHandle;
      /** The OpenGL handle for the geometry shader. */
      unsigned _geometryShaderHandle;
      /** The OpenGL handle for the Tesselation Evaluation shader. */
      unsigned _tesselationEvalHandle;
      /** The OpenGL handle for the Tesselation Control shader. */
      unsigned _tesselationControlHandle;
      /** The OpenGL handle for the fragment shader. */
      unsigned _fragmentShaderHandle;

      /** Shader program location of the projection matrix. */
      int _projectionLocation;
      /** Shader program location of the view matrix. */
      int _viewLocation;
      /** Shader program location of the world matrix. */
      int _worldLocation;

      /** The current projection matrix. */
      mat4 _projectionMatrix;
      /** The current view matrix. */
      mat4 _viewMatrix;
      /** The current world matrix. */
      mat4 _worldMatrix;

      /** Standard file name for the vertex shader. */
      const static string VERTEX_SHADER_FILE_NAME;
      /** Standard file name for the geometry shader. */
      const static string GEOMETRY_SHADER_FILE_NAME;
      /** Standard file name for the fragment shader. */
      const static string FRAGMENT_SHADER_FILE_NAME;

      /**
        * Checks if the given file exists.
        *
        * @param file The file to check for.
        *
        * @return True if the file exists, false otherwise.
        */
      bool fileExists(const string& file);

      /**
        * Helper function to initialize a shader.
        *
        * @param handle An unsigned to store the OpenGL handle in.
        * @param file The file name.
        * @param shaderType The type of shader to initialize.
        */
      void initShader(unsigned& handle, const string& file, GLenum shaderType);

      /**
        * Helper function to check whether this is the currently bound program.
        *
        * @return True if this is currently bound, false otherwise.
        */
      bool isCurrentProgram();

      /**
        * Helper function to load a file in the way required of shaders.
        * 
        * @param name The name of the file to load.
        *
        * @returns The whole file as a continuous c-string.
        */
      char* loadFile(const string& name);
};

#endif

