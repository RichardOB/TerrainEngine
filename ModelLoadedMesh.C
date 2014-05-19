#include "ModelLoadedMesh.h"

ModelLoadedMesh::
ModelLoadedMesh(string fileName):
	_indexCount(0),
	out_vertices(),
	out_uvs(),
	out_normals()
{
  // _vertexCount = 8;
	
	vector<vec3> temp_vertices;
	vector<vec2> temp_uvs;
	vector<vec3> temp_normals;
	
	vector<float> vertexIndices, uvIndices, nIndices;
	string str;
	//unsigned endPos;
	//float index;
	
	//open the file
	FILE * file = fopen(fileName.c_str(), "r");
	
	if (file == NULL)
	{
		cout << "[ERROR]: Could not open file '" << fileName << "' in ModelLoadedMesh.C" << endl;
		exit(-1);
	}
	
	while (!feof(file))
	{
		char line[100];
		
		//fscanf(file, "%s", line);
		
		if (strcmp(line, "v") == 0)
		{
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			
			cout << "v: " << vertex.x << " " << vertex.y << " " << vertex.z << endl;
			temp_vertices.push_back(vertex);
			
			fscanf(file, "%s", line);
		}
		else if (strcmp(line, "vt") == 0)
		{
			vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
			
			fscanf(file, "%s", line);
			
		}
		else if (strcmp(line, "vn") == 0)
		{
			vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
			
			fscanf(file, "%s", line);
		}
		else if (strcmp(line, "f") == 0)
		{
			string vertex1, vertex2, vertex3;
			unsigned vertexIndex[3], uvIndex[3], normalIndex[3];
			
			int count = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			
			if (count != 9)
			{
				cout << "[ERROR] Model type cannot be loaded using this simple model looader. Please use correctly formatted .obj files" << endl;
				break;
			}
			
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			
			nIndices.push_back(normalIndex[0]);
			nIndices.push_back(normalIndex[1]);
			nIndices.push_back(normalIndex[2]);
			
			fscanf(file, "%s", line);
			
		}
		/*else if (strcmp(line, "f") == 0)
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
		}*/
		else if (strcmp(line, "f") == EOF)
		{
			break;
		}
		else
		{
			fscanf(file, "%s", line); //i.e. skip content for now
		}
	}
	
	fclose(file);
	
	unsigned VERTEX_ARRAY_SIZE = temp_vertices.size() * 3;
	unsigned NORMAL_ARRAY_SIZE = temp_normals.size() * 3;
	unsigned UV_ARRAY_SIZE = temp_uvs.size() * 2;
	
	unsigned INDEX_ARRAY_SIZE = vertexIndices.size();
	unsigned NORMALS_INDEX_ARRAY_SIZE = nIndices.size();
	unsigned UV_INDEX_ARRAY_SIZE = uvIndices.size();
	
	_indexCount = (GLsizei) INDEX_ARRAY_SIZE;
	
	GLfloat* tempVertices = new GLfloat[VERTEX_ARRAY_SIZE];
	GLuint* tempIndices = new GLuint[INDEX_ARRAY_SIZE];
	
	GLfloat* tempNormals = new GLfloat[NORMAL_ARRAY_SIZE];
	GLuint* tempNormalsIndices = new GLuint[NORMALS_INDEX_ARRAY_SIZE];
	
	GLfloat* tempUvs = new GLfloat[UV_ARRAY_SIZE];
	GLuint* tempUvsIndices = new GLuint[UV_INDEX_ARRAY_SIZE];


	int j = 0;
	
	for (unsigned i = 0; i <  temp_vertices.size(); i ++)
	{
		tempVertices[j++] = temp_vertices[i].x;
		tempVertices[j++] = temp_vertices[i].y;
		tempVertices[j++] = temp_vertices[i].z;
	}
	
	for (unsigned i = 0; i < vertexIndices.size(); i++)
	{
		unsigned vertexIndex =  vertexIndices[i];
		//cout << vertexIndex -1;
		tempIndices[i] =  vertexIndex -1;
	}
	cout << endl;
	
	j = 0;
	
	for (unsigned i = 0; i <  temp_normals.size(); i ++)
	{
		tempNormals[j++] = temp_normals[i].x;
		tempNormals[j++] = temp_normals[i].y;
		tempNormals[j++] = temp_normals[i].z;
	}
	
	for (unsigned i = 0; i < nIndices.size(); i++)
	{
		tempNormalsIndices[i] = nIndices[i] - 1;
	}
	
	j = 0;
	
	for (unsigned i = 0; i <  temp_uvs.size(); i ++)
	{
		tempUvs[j++] = temp_uvs[i].x;
		tempUvs[j++] = temp_uvs[i].y;
	}
	
	for (unsigned i = 0; i < uvIndices.size(); i++)
	{
		unsigned uvIndex = uvIndices[i];
		cout << uvIndex -1;
		tempUvsIndices[i] = uvIndex - 1;
	}
	
	bind();

	GLuint buffers[6];
	glGenBuffers(6, buffers);

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
	
	
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, NORMAL_ARRAY_SIZE * (GLsizei)sizeof(float),
         tempNormals, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NORMALS_INDEX_ARRAY_SIZE * (GLsizei)sizeof(GLuint),
         tempNormalsIndices, GL_STATIC_DRAW);
	 
	GLuint normalsLocation = findAttribute("normals");
	glEnableVertexAttribArray(normalsLocation);
	glVertexAttribPointer(normalsLocation, 3, GL_FLOAT,
	GL_FALSE, 0, 0);
	
	
	glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
	glBufferData(GL_ARRAY_BUFFER, UV_ARRAY_SIZE * (GLsizei)sizeof(float),
	tempUvs, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[5]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, UV_INDEX_ARRAY_SIZE * (GLsizei)sizeof(GLuint),
	tempUvsIndices, GL_STATIC_DRAW);
	
	GLuint uvsLocation = findAttribute("uv");
	glEnableVertexAttribArray(uvsLocation);
	glVertexAttribPointer(uvsLocation, 2, GL_FLOAT,
	GL_FALSE, 0, 0);


	
	
	/*//process vertex data
	for (unsigned i = 0; i < vertexIndices.size(); i++)
	{
		unsigned vertexIndex = vertexIndices[i];
		
		//subtract 1.0f since indices in .obj file start with 1, not 0
		vec3 vertex = temp_vertices[vertexIndex - 1.0f];
		
		out_vertices.push_back(vertex);
	}
	
	//process normals data
	for (unsigned i = 0; i < nIndices.size(); i++)
	{
		unsigned normalIndex = nIndices[i];
		
		//subtract 1.0f since indices in .obj file start with 1, not 0
		vec3 vertex = temp_normals[normalIndex - 1.0f];
		
		out_normals.push_back(vertex);
	}
	
	//process uv data
	for (unsigned i = 0; i < uvIndices.size(); i++)
	{
		unsigned uvIndex = uvIndices[i];
		
		//subtract 1.0f since indices in .obj file start with 1, not 0
		vec2 vertex = temp_uvs[uvIndex - 1.0f];
		
		out_uvs.push_back(vertex);
	}
	
	bind();
	
	
	GLuint buffers[2];
	glGenBuffers(2, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(vec3),
	&out_vertices[0], GL_STATIC_DRAW);*/
	
	
	
	
	
	
	
	
	/*unsigned VERTEX_ARRAY_SIZE = temp_vertices.size() * 3;
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
         GL_FALSE, 0, 0);*/

}

void ModelLoadedMesh::draw()
{
	bind();
	glDrawElements(GL_TRIANGLES, _indexCount * 3, GL_UNSIGNED_INT, NULL);
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