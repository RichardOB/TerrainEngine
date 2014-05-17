#include "Skybox.h"

Skybox::Skybox(vec3 eye):
	cameraEye()
{
	cameraEye = eye;
	skyboxShader = new Shader("Cube");
	skyboxShader->apply();
	skyboxMesh = new Cube(20);
}

Skybox::~Skybox()
{
  
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
void Skybox::render()
{

	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	
	mat4 skyboxWorld;
	skyboxWorld = translate(skyboxWorld, vec3(cameraEye.x, cameraEye.y, cameraEye.z));
	skyboxShader->updateWorldMatrix(skyboxWorld);
	skyboxShader->apply();
	
	skyboxMesh->draw();
	
	glCullFace(OldCullFaceMode); 
	glDepthFunc(OldDepthFuncMode);
}
#pragma GCC diagnostic pop