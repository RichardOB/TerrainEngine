#include "Shader.h"

const string Shader::VERTEX_SHADER_FILE_NAME = "vertex.glsl";
const string Shader::GEOMETRY_SHADER_FILE_NAME = "geometry.glsl";
const string Shader::FRAGMENT_SHADER_FILE_NAME = "fragment.glsl";
const string TESSELATION_CONTROL_SHADER = "tesselationControlShader.glsl";
const string TESSELATION_EVALUATION_SHADER = "tesselationEvaluationShader.glsl";

Shader::
Shader(const string& name):
   _programHandle(0),
   _vertexShaderHandle(0),
   _geometryShaderHandle(0),
   _fragmentShaderHandle(0),
   _projectionLocation(0),
   _viewLocation(0),
   _worldLocation(0),
   _projectionMatrix(mat4(1.0f)),
   _viewMatrix(mat4(1.0f)),
   _worldMatrix(mat4(1.0f))
{
   /* Start by defining a shader program which acts as a container. */
   _programHandle = glCreateProgram();

   string path = "./shaders/" + name + "/";
   const string VSF = path + VERTEX_SHADER_FILE_NAME;
   const string GSF = path + GEOMETRY_SHADER_FILE_NAME;
   const string FSF = path + FRAGMENT_SHADER_FILE_NAME;
   const string TESC = path +TESSELATION_CONTROL_SHADER;
   const string TESE = path +TESSELATION_EVALUATION_SHADER;

   initShader(_vertexShaderHandle, VSF, GL_VERTEX_SHADER);
   glAttachShader(_programHandle, _vertexShaderHandle);

   if (fileExists(GSF))
   {
      initShader(_geometryShaderHandle, GSF, GL_GEOMETRY_SHADER);
      glAttachShader(_programHandle, _geometryShaderHandle);
   }
   else
   {
      cout << "[INFO] Shader '" << name << "' lacks geometry shader." << endl;
   }
   
   if (fileExists(TESE))
   {
	initShader(_tesselationEvalHandle, TESE, GL_TESS_EVALUATION_SHADER);
	glAttachShader(_programHandle, _tesselationEvalHandle);
   }
   else
   {
      cout << "[INFO] Shader '" << name << "' lacks tesselation evaluation shader." << endl;
   }
   
   if (fileExists(TESC))
   {
	initShader(_tesselationControlHandle, TESC, GL_TESS_CONTROL_SHADER);
	glAttachShader(_programHandle, _tesselationControlHandle);
   }
   else
   {
      cout << "[INFO] Shader '" << name << "' lacks tesselation control shader." << endl;
   }

   initShader(_fragmentShaderHandle, FSF, GL_FRAGMENT_SHADER);
   glAttachShader(_programHandle, _fragmentShaderHandle);

   /* Finally, the program must be linked. */
   glLinkProgram(_programHandle);

   /* Check if it linked  properly. */
   int status;
   glGetProgramiv(_programHandle, GL_LINK_STATUS, &status);

   if (status == GL_FALSE)
   {
      /* Get the length of the info log. */
      int len;
      glGetProgramiv(_programHandle, GL_INFO_LOG_LENGTH, &len);

      /* Get the info log. */
      char* log = new char[len];
      glGetProgramInfoLog(_programHandle, len, &len, log);

      /* Throw a exception. */
      throw LinkError(log);

      /* Finally, free the memory allocated. */
      delete log;

      /* Exit the program. */
      exit(-1);
   }

   _projectionLocation = findUniform("projection");
   _viewLocation = findUniform("view");
   _worldLocation = findUniform("world");
}

Shader::
~Shader()
{
   glDetachShader(_programHandle, _vertexShaderHandle);
   glDeleteShader(_vertexShaderHandle);

   glDetachShader(_programHandle, _fragmentShaderHandle);
   glDeleteShader(_fragmentShaderHandle);

   glDeleteProgram(_programHandle);
}

void Shader::
apply()
{
   /* Load the program, and use it. */
   glUseProgram(_programHandle);

   /* Load matrix uniforms. */
   glUniformMatrix4fv(_projectionLocation, 1, false, value_ptr(_projectionMatrix));
   glUniformMatrix4fv(_viewLocation, 1, false, value_ptr(_viewMatrix));
   glUniformMatrix4fv(_worldLocation, 1, false, value_ptr(_worldMatrix));
}

void Shader::
updateProjectionMatrix(mat4 matrix)
{
   _projectionMatrix = matrix;

   if (isCurrentProgram())
   {
      glUniformMatrix4fv(_projectionLocation, 1, false, value_ptr(_projectionMatrix));
   }
}

void Shader::
updateViewMatrix(mat4 matrix)
{
   _viewMatrix = matrix;

   if (isCurrentProgram())
   {
      glUniformMatrix4fv(_viewLocation, 1, false, value_ptr(_viewMatrix));
   }
}

void Shader::
updateWorldMatrix(mat4 matrix)
{
   _worldMatrix = matrix;

   if (isCurrentProgram())
   {
      glUniformMatrix4fv(_worldLocation, 1, false, value_ptr(_worldMatrix));
   }
}

void
Shader::updateUniform(const char* name, float value)
{
	GLint location = findUniform(name);
	glUniform1f(location, value);
}

void Shader::updateUniform(const char* name, vec3 value)
{
	GLint location = findUniform(name);
	glUniform3fv(location, 1, value_ptr(value));
}
      
void Shader::updateUniform(const char* name, mat4 value)
{
	GLint location = findUniform(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
}


GLint Shader::
findUniform(const char* name)
{
   GLint location = glGetUniformLocation(_programHandle, name);

   if (location == -1)
   {
      string sName(name);
      cerr << "Could not locate uniform '" << sName << "'." << endl;
   }

   return location;
}

bool Shader::
fileExists(const string& file)
{
   FILE* f = fopen(file.c_str(), "r");

   if (f == NULL)
   {
      return false;
   }
   else
   {
      fclose(f);
      return true;
   }
}

void Shader::
initShader(unsigned& handle, const string& file, GLenum shaderType)
{
   /* First, a handle to a shader object of the appropriate type must be 
    * obtained. */
   handle = glCreateShader(shaderType);

   /* Second, the shader source must be loaded... */
   char* source = loadFile(file);

   /* ...and set to be the current source in the OpenGL state machine. The first 
    * parameter here is a handle to a shader object, the second is the number of 
    * strings in the array which we provide next. The last parameter is an 
    * integer array indicating the length of these strings. The third parameter
    * may also contain the whole source code as one long string, while the 
    * fourth parameter may be NULL, to indicate that the string(s) is (are) NULL 
    * terminated. Since that is the case in our implementation, the arguments 
    * passed here correspond. */
   glShaderSource(handle, 1, (const GLchar**)&source, NULL);

   /* Third, we compile the shader program. */
   glCompileShader(handle);

   /* After the shader source has been set inside OpenGL, we can free the memory 
    * we allocated. */
   free(source);

   /* Check if it compiled properly, if not, print any log information. */
   int status;
   glGetShaderiv(handle, GL_COMPILE_STATUS, &status);

   if (status == GL_FALSE)
   {
      /* Get the length of the info log. */
      int len;
      glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);

      /* Get the info log. */
      char* log = new char[len];
      glGetShaderInfoLog(handle, len, &len, log);

      /* Throw an exception. */
      cout << "[ERROR] Compilation Error in " + file + ": " + log << endl;
      string sLog(log);
      string error = "In file '" + file + "':\n" + sLog;
     // throw new CompilationError(error);

      /* Finally, free the memory allocated. */
      delete log;

      /* Exit the program. */
      exit(-1);
   }
}

bool Shader::
isCurrentProgram()
{
   GLint currentProgram;
   glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

   return _programHandle == (GLuint)currentProgram;
}

char* Shader::
loadFile(const string& fileName)
{
   FILE* file = fopen(fileName.c_str(), "r");
   if (file == NULL)
   {
      string error = "Could not open file '" + fileName + "'.";
      throw CompilationError(error);
   }

   unsigned bufferSize = 1;
   char* buffer = (char*)malloc(bufferSize);

   unsigned long long index = 0;
   while (true)
   {
      char c = (char)fgetc(file);
      if (ferror(file) != 0)
      {
         string error = "Could not read from file '" + fileName + "'.";
         throw CompilationError(error);
      }

      if (feof(file))
      {
         break;
      }
      else
      {
         /* There must be space at bufferSize - 2 for the new char, and at 
          * bufferSize - 1 for the \0 character terminating the string. So if 
          * the buffer's index has already reached bufferSize - 1 it is already 
          * out of space! */
         if (index == bufferSize - 1)
         {
            bufferSize *= 2;
            buffer = (char*)realloc((void*)buffer, bufferSize);
         }
            
         buffer[index] = c;
         index++;
      }
   }
   buffer[index] = '\0';

   fclose(file);

   return buffer;
}
