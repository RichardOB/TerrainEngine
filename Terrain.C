#include "Terrain.h"

Terrain::
Terrain(string heightMapName):
	COMPONENTS(3),
	width(0),
	height(0),
	heightMap(heightMapName.c_str()),
	_indexCount(0),
	 vertices()
{
	width = heightMap.get_width();
	height = heightMap.get_height();
	
	readPNG();
	
	for(int i =0; i < 20; i++)
	{
		smoothVertices();
	}
	
   const unsigned VERTICES_PER_ROW = width;
   const unsigned VERTICES_PER_COL = height;
	
   _vertexCount = (GLsizei)VERTICES_PER_ROW * (GLsizei)VERTICES_PER_COL;

   const unsigned VERTEX_COMPONENT_COUNT = 3;
   const unsigned VERTEX_ARRAY_SIZE = (unsigned)_vertexCount *
      VERTEX_COMPONENT_COUNT;
   float* vertices = new float[VERTEX_ARRAY_SIZE];

   const unsigned SQUARES = height * width;
   const unsigned TRIANGLES_PER_SQUARE = 2;
   const unsigned VERTICES_PER_TRIANGLE = 3;
   const unsigned VERTICES_PER_SQUARE = TRIANGLES_PER_SQUARE * 
      VERTICES_PER_TRIANGLE;
   _indexCount = (GLsizei)VERTICES_PER_SQUARE * (GLsizei)SQUARES;

   const unsigned INDEX_ARRAY_SIZE = (unsigned)_indexCount;
   GLuint* indices = new GLuint[INDEX_ARRAY_SIZE];

   const float Z_DELTA = 2.0f;
   const float X_DELTA = 2.0f;
   
   cout << "width: " << width << endl;
   cout << "height: " << height << endl; 

   unsigned index = 0;
   for (unsigned z = 0; z < VERTICES_PER_COL; z++)
   {
      const float Z = (float)z * Z_DELTA;

      for (unsigned x = 0; x < VERTICES_PER_ROW; x++)
      {
         const float X = (float)x * X_DELTA;

         vertices[index++] = X;
	//cout << "Height: " << ((float)heightMap.get_pixel(x, z).red / 255.0f) * 250.0f;;
        // vertices[index++] =  ((float)heightMap.get_pixel(x, z).red / width) * 500.0f;
	 vertices[index++] =  heights[x][z];
         vertices[index++] = Z;
      }
   }
   
   cout << "Pixel r [0][0] = " <<  (float)heightMap.get_pixel(0, 0).red << endl;

   index = 0;
   for (unsigned z = 0; z < VERTICES_PER_COL - 1; z++)
   {
      for (unsigned x = 0; x < VERTICES_PER_ROW - 1; x++)
      {
         unsigned i = z * VERTICES_PER_COL + x;

         /* Top left square. */
         indices[index++] = i;
         indices[index++] = i + VERTICES_PER_ROW;
         indices[index++] = i + 1;

         /* Bottom right square. */
         indices[index++] = i + 1;
         indices[index++] = i + VERTICES_PER_ROW;
         indices[index++] = i + 1 + VERTICES_PER_ROW;
      }
   }

   bind();

   GLuint buffers[2];
   glGenBuffers(2, buffers);

   glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, VERTEX_ARRAY_SIZE * (GLsizei)sizeof(float),
         vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_ARRAY_SIZE * (GLsizei)sizeof(GLuint),
         indices, GL_STATIC_DRAW);

   GLuint positionLocation = findAttribute("position");
   glEnableVertexAttribArray(positionLocation);
   glVertexAttribPointer(positionLocation, VERTEX_COMPONENT_COUNT, GL_FLOAT,
         GL_FALSE, 0, 0);
}

 void Terrain::readPNG()
{
	heights = new float*[width];
	
	for(int i = 0; i < height; i ++)
	{
		heights[i] = new float[height];
	}
	
	for (unsigned z = 0; z < height; z++)
	{
		for (unsigned x = 0; x < width; x++)
		{
			heights[z][x] =  ((float)heightMap.get_pixel(x, z).red / width) * 400.0f;
		}
	}
}

void Terrain::smoothVertices()
{
	float** smooth = new float*[width];
	
	for(int i = 0; i < height; i ++)
	{
		smooth[i] = new float[height];
	}
	
	
	float mid_middle;
	float mid_top;
	float mid_bottom;
	
	float left_middle;
	float left_top;
	float left_bottom;
	
	float right_middle;
	float right_top;
	float right_bottom;
	
	
	
	for(int z = 1; z < height - 1; z++)
	{
		
		for (int x = 1; x < width - 1; x++)
		{
			//cout << "z: " << z <<" x: " << x; 
			mid_top = heights[x][z-1];//2
			mid_middle = heights[x] [z];//4
			mid_bottom = heights[x][z+1];//2
			
			left_top = heights[x-1][z-1];//1
			left_middle = heights[x-1][z];//2
			left_bottom = heights[x-1][z+1];//1
			
			right_top = heights[x+1][z-1];//1
			right_middle = heights[x+1][z];//2
			right_bottom = heights[x+1][z+1];//1
			
			float value =  (4.0f * mid_middle + 2.0f * mid_top + 2.0f * mid_bottom + 2.0f * left_middle + 2.0f * right_middle + 1.0f * left_top+ 1.0f * right_top + 1.0f * left_bottom + 1.0f * right_bottom) / 16.0f;
			
			
			//cout << "(" << (float)heightMap.get_pixel(x, z).red << "," << (float)heightMap.get_pixel(x, z).green << "," << (float)heightMap.get_pixel(x, z).blue << ")";
			smooth[x][z] = value;
		}
	}
	
	 //smooth.write("heightmaps/GoodHeightmap1Smoothing.png");
	 //image<rgb_pixel > image("heightmaps/GoodHeightmap1Smoothing.png");
	heights = smooth;	
}

void Terrain::
draw()
{
   bind();
   glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, NULL);
   //glDrawArrays(GL_POINTS, 0, _vertexCount);
}

float* Terrain::
getCoord(unsigned x, unsigned z)
{
	cout << "getCoord(" << x << ", " << z << ") = " << vertices + (z * width * 3) + (x * 3) << endl;
   return vertices + (z * width * 3) + (x * 3);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-variable"
void Terrain::
smoothPNG()
{
	 image<rgb_pixel> smooth(width, height);
	float* rgb = new float[3];
	
	float* mid_middle = new float[3];
	float* mid_top = new float[3];
	float* mid_bottom = new float[3];
	
	float* left_middle = new float[3];
	float* left_top = new float[3];
	float* left_bottom = new float[3];
	
	float* right_middle = new float[3];
	float* right_top = new float[3];
	float* right_bottom = new float[3];
	
	
	
	for(int z = 1; z < height - 1; z++)
	{
		for (int x = 1; x < width - 1; x++)
		{

			mid_top = getRGB(x, z-1);//2
			mid_middle = getRGB(x, z);//4
			mid_bottom = getRGB(x, z+1);//2
			
			left_top = getRGB(x-1, z-1);//1
			left_middle = getRGB(x-1, z);//2
			left_bottom = getRGB(x-1, z+1);//1
			
			right_top = getRGB(x+1, z-1);//1
			right_middle = getRGB(x+1, z);//2
			right_bottom = getRGB(x+1, z+1);//1
			
			rgb[0] = (4.0f * mid_middle[0] + 2.0f * mid_top[0] + 2.0f * mid_bottom[0] + 2.0f * left_middle[0] + 2.0f * right_middle[0] + 1.0f * left_top[0] + 1.0f * right_top[0] + 1.0f * left_bottom[0] + 1.0f * right_bottom[0]) / 16.0f;
			rgb[1] = (4.0f * mid_middle[1] + 2.0f * mid_top[1] + 2.0f * mid_bottom[1] + 2.0f * left_middle[1] + 2.0f * right_middle[1] + 1.0f * left_top[1] + 1.0f * right_top[1] + 1.0f * left_bottom[1] + 1.0f * right_bottom[1]) / 16.0f;
			rgb[2] = (4.0f * mid_middle[2] + 2.0f * mid_top[2] + 2.0f * mid_bottom[2] + 2.0f * left_middle[2] + 2.0f * right_middle[2] + 1.0f * left_top[2] + 1.0f * right_top[2] + 1.0f * left_bottom[2] + 1.0f * right_bottom[2]) / 16.0f;

			float r = (float)heightMap.get_pixel(x, z).red;
			float g = (float)heightMap.get_pixel(x, z).green;
			float b = (float)heightMap.get_pixel(x, z).blue;
			
			
			
			//cout << "(" << (float)heightMap.get_pixel(x, z).red << "," << (float)heightMap.get_pixel(x, z).green << "," << (float)heightMap.get_pixel(x, z).blue << ")";
			smooth[x][z] = rgb_pixel(rgb[0], rgb[0], rgb[0]);
		}
	}
	
	 //smooth.write("heightmaps/GoodHeightmap1Smoothing.png");
	 //image<rgb_pixel > image("heightmaps/GoodHeightmap1Smoothing.png");
	heightMap = smooth;
}


float* Terrain:: getRGB(int x, int z)
{
	float* rgb = new float[3];
	rgb[0] = (float)heightMap.get_pixel(x, z).red;

	rgb[1] = (float)heightMap.get_pixel(x, z).green;

	rgb[2] = (float)heightMap.get_pixel(x, z).blue;

	
	return rgb;
}
#pragma GCC diagnostic pop



