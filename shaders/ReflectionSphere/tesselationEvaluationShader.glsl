#version 420

layout (triangles, equal_spacing) in;

uniform mat4 view;
uniform mat4 world;
uniform mat4 projection;

uniform vec3 lightProj;
uniform mat4 lightView;

uniform vec3 cameraEye;

//out vec3 t_colour;
//out vec3 t_lightDir;
//out vec3 t_viewDir;
//out vec3 t_normal;

out vec3 reflection;

void main()
{

   // Compute Euclidian coordinates from Barycentric coordinates provided by the
   // tesselator.
   vec3 p0 = gl_in[0].gl_Position.xyz * gl_TessCoord.x;
   vec3 p1 = gl_in[1].gl_Position.xyz * gl_TessCoord.y;
   vec3 p2 = gl_in[2].gl_Position.xyz * gl_TessCoord.z;

   // Normalize the model-space coordinates to place them on a unit sphere.
   vec3 position = normalize(p0 + p1 + p2);
   vec4 worldPos = world * vec4(position, 1.0f);
	
   //position point in world space
   vec3 w_position = (world * vec4(position, 1.0f)).xyz;
	
   //normal vector in world space
   vec3 w_normal = (world * vec4(position, 0.0f)).xyz;
	
   /* Now we need to calculate the view vector. This is a vector that goes from camera to the object 
    * (Might need to be swapped in different implementations). In our implementation, it needs to be this way since
    * the GLSL reflect() function requires an incident vector as first argument (i.e. a vector that hits the surface of the object, not leaves it).
    */
    
    /*To calculate view vector we need:
     * 1. Position of object in world space (which we have)
     * 2. Position of camera in world space (which we don't have)
     */
     
     /*To calculate this last value (instead of passing it as a uniform):
      * 1. Take the position of the camera in view space (simply 0 0 0).
      * 2. Convert to world space through the inverse of the world->view space conversion matrix (i.e. inverse of view matrix)
      */
	
    //position of camera
    vec3 w_eye = (inverse(view) * vec4(0, 0, 0, 1)).xyz;
    //view vector
    vec3 w_view = normalize(w_position - w_eye);
    
    /*Now we have everything we need to calculate the reflection vector:
     * 1. The view vector
     * 2. The surface normal
     */
      reflection = reflect(w_view, w_normal);
	
	
/*
   // The normal for a point on a unit sphere has the same components as the
   // point.
   t_normal = worldPos.xyz;

   // Ditto for colour on a colour sphere.
   t_colour = position;

   // The direction towards the light. Subtract the world position of the point
   // we want to shade from the light's world position to get the light vector.
   t_lightDir = normalize(vec4(lightPos, 1) - worldPos).xyz;

   // The direction towards the viewer. Subtract the world position of the
   // point we want to shade from the viewer's world position to get the
   // view vector.
   t_viewDir = normalize(vec4(cameraEye, 1) - worldPos).xyz;

   // Apply the rest of the matrices to the geometry so we get output as per
   // usual.
   */
   gl_Position = projection * view * worldPos;
  
}
