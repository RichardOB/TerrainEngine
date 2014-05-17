#include <cmath>
#include <cstdlib>

#include "Constants.h"
#include "Sphere.h"

Sphere::
Sphere(int resolution):
   Mesh(),
   _resolution(resolution)
{
   /* The amount to change the angle by with each step, determined by 
      resolution.*/
   const float ANGLE_DELTA = (float)REVOLUTION / (float)_resolution;
   /* Convert the delta to radians, since that's what C++ works in. */
   const float R_ANGLE_DELTA = ANGLE_DELTA * (float)M_PI / 180.0f;

   /* Allocate vertices for each step along the x-axis. */
   _vertexCount =
      (int) ceilf(abs(X_ANGLE_STOP - X_ANGLE_START) / ANGLE_DELTA)
   /* Allocate vertices for each step along the y-axis. */
      * (int) ceilf(abs(Y_ANGLE_STOP - Y_ANGLE_START) / ANGLE_DELTA)
   /* Each step requires two triangles. */
      * 2
   /* Each triangle has three vertices. */
      * 3;

   /* Each vertex has three elements. */
   GLsizei VERTEX_ARRAY_SIZE = _vertexCount * 3;
   /* Allocate space for the positions. */
   float* positions = new float[VERTEX_ARRAY_SIZE];

   unsigned index = 0;

   /* Step through x / y combinations, and use sin / cos to determine the 
      coordinates for each step. */
   for (float longitude = X_ANGLE_START; longitude < X_ANGLE_STOP;
      longitude += ANGLE_DELTA)
   {
      float rlong = longitude * (float)M_PI / 180.0f;

      for (float latitude = Y_ANGLE_START; latitude < Y_ANGLE_STOP;
            latitude += ANGLE_DELTA)
      {
         float rlat = latitude * (float)M_PI / 180.0f;

         /* Bottom left. */
         positions[index + X] = sinf(rlong) * cosf(rlat);
         positions[index + Y] = sinf(rlat);
         positions[index + Z] = cosf(rlong) * cosf(rlat);

         index += W;


         /* Top right. */
         positions[index + X] =
            sinf(rlong + R_ANGLE_DELTA) * cosf(rlat + R_ANGLE_DELTA);
         positions[index + Y] =
            sinf(rlat + R_ANGLE_DELTA);
         positions[index + Z] =
            cosf(rlong + R_ANGLE_DELTA) * cosf(rlat + R_ANGLE_DELTA);

         index += W;


         /* Top left. */
         positions[index + X] = sinf(rlong) * cosf(rlat + R_ANGLE_DELTA);
         positions[index + Y] = sinf(rlat + R_ANGLE_DELTA);
         positions[index + Z] = cosf(rlong) * cosf(rlat + R_ANGLE_DELTA);

         index += W;


         /* Bottom left. */
         positions[index + X] = sinf(rlong) * cosf(rlat);
         positions[index + Y] = sinf(rlat);
         positions[index + Z] = cosf(rlong) * cosf(rlat);

         index += W;


         /* Bottom right. */
         positions[index + X] =
            sinf(rlong + R_ANGLE_DELTA) * cosf(rlat);
         positions[index + Y] =
            sinf(rlat);
         positions[index + Z] =
            cosf(rlong + R_ANGLE_DELTA) * cosf(rlat);

         index += W;


         /* Top right. */
         positions[index + X] =
            sinf(rlong + R_ANGLE_DELTA) * cosf(rlat + R_ANGLE_DELTA);
         positions[index + Y] =
            sinf(rlat + R_ANGLE_DELTA);
         positions[index + Z] =
            cosf(rlong + R_ANGLE_DELTA) * cosf(rlat + R_ANGLE_DELTA);

         index += W;
      }
   }

   bind();

   GLuint buffer;
   glGenBuffers(1, &buffer);
   glBindBuffer(GL_ARRAY_BUFFER, buffer);
   glBufferData(GL_ARRAY_BUFFER, VERTEX_ARRAY_SIZE * (GLsizei)sizeof(float),
         positions, GL_STATIC_DRAW);

   GLuint positionLocation = findAttribute("position");
   glEnableVertexAttribArray(positionLocation);
   glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

   /* Since the normals are equal to the positions, we can simply
      reuse the buffer. */
   GLuint normalLocation = findAttribute("normal");
   glEnableVertexAttribArray(normalLocation);
   glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

