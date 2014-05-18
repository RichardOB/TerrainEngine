#include "ModelLoadedMesh.h"

ModelLoadedMesh::
ModelLoadedMesh(string fileName):
   _indexCount(0)
{
  // _vertexCount = 8;
	
	vector<vec3> temp_vertices;
	vector<float> vertexIndices;
	string str;
	unsigned endPos;
	float index;
	
	//open the file
	FILE * file = fopen(fileName.c_str(), "r");
	
	if (file == NULL)
	{
		cout << "[ERROR]: Could not open file '" << fileName << "' in ModelLoadedMesh.C" << endl;
		exit(-1);
	}
	
	while (!feof(file))
	{
		char line[128];
		
		if (strcmp(line, "v") == 0)
		{
			//if the first word of the line is "v", then the rest has to be 3 floats representing the components of the vertex
			vec3 vertex;
			fscanf(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
			
			fscanf(file, "%s", line);
		}
		else if (strcmp(line, "f") == 0)
		{
			fscanf(file, "%s", line);
			
			while (isdigit(line[0]))
			{
				str = charArrayToString(line);
				endPos = str.find_first_of("/");//end of number
				
				index = stringToFloat(str.substr(0,endPos));
				vertexIndices.push_back(index-1.0f);// subtract 1.0f since indices in .obj file start with 1, not 0
				
				fscanf(file, "%s", line);
				
				if (feof(file))
				{
					str = charArrayToString(line);
					endPos = str.find_first_of("/");//end of number
					
					index = stringToFloat(str.substr(0,endPos));
					vertexIndices.push_back(index-1.0f);// subtract 1.0f since indices in .obj file start with 1, not 0
					
					break;
				}
			}
		}
		else
		{
			fscanf(file, "%s", line); //i.e. skip content for now
		}
	}
	
	fclose(file);
	
	unsigned VERTEX_ARRAY_SIZE = temp_vertices.size() * 3;
	unsigned INDEX_ARRAY_SIZE = vertexIndices.size();
	_indexCount = (GLsizei) INDEX_ARRAY_SIZE;
	
	GLfloat* tempVertices = new GLfloat[VERTEX_ARRAY_SIZE];
	GLuint* tempIndices = new GLuint[INDEX_ARRAY_SIZE];

	
	int j = 0;
	
	for (unsigned i = 0; i <  temp_vertices.size(); i ++)
	{
		tempVertices[j++] = temp_vertices[i].x;
		tempVertices[j++] = temp_vertices[i].y;
		tempVertices[j++] = temp_vertices[i].z;
	}
	
	for (unsigned i = 0; i < vertexIndices.size(); i++)
	{
		tempIndices[i] = vertexIndices[i];
	}
	
	bind();

   GLuint buffers[2];
   glGenBuffers(2, buffers);

   glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, VERTEX_ARRAY_SIZE * (GLsizei)sizeof(float),
         tempVertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_ARRAY_SIZE * (GLsizei)sizeof(GLuint),
         tempIndices, GL_STATIC_DRAW);

   GLuint positionLocation = findAttribute("position");
   glEnableVertexAttribArray(positionLocation);
   glVertexAttribPointer(positionLocation, 3, GL_FLOAT,
         GL_FALSE, 0, 0);

}

void ModelLoadedMesh::draw()
{
	bind();
	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, NULL);
}

string ModelLoadedMesh::charArrayToString(char* characters)
{
	string str (characters);
	
	return str;
}

float ModelLoadedMesh::stringToFloat(string str)
{
	float result;

      std::stringstream stream(str);
      stream >> result;
      
      return result;
}