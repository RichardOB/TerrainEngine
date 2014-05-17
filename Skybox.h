#ifndef SKYBOX_H
#define SKYBOX_H

#include <iostream>
#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-pedantic"

#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#pragma GCC diagnostic pop

#include <cmath>

#include "Shader.h"
#include "Cube.h"
#include "Mesh.h"
#include "CubeMapTexture.h"

using glm::mat4;
using glm::vec3;
using glm::rotate;
using glm::translate;
using glm::scale;
using glm::perspective;
using glm::lookAt;
using glm::value_ptr;

class Skybox
{
	public:
		Skybox(vec3 cameraEye);

		~Skybox();
	
		Skybox(const Skybox&);
	
		//operator=(const Skybox&);

		bool init(const string& Directory,
			const string& PosXFilename,
			const string& NegXFilename,
			const string& PosYFilename,
			const string& NegYFilename,
			const string& PosZFilename,
			const string& NegZFilename);

		void render();

	private: 
		Shader* skyboxShader = NULL;
		//const Camera* m_pCamera;
		vec3 cameraEye;
		CubeMapTexture* cubemapTex = NULL;
		Mesh* skyboxMesh = NULL;
};

#endif