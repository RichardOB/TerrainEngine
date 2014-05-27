#include "Main.h"

using namespace std;


//Display Callback
//Called whenever the contents of the window need to be redrawn on the window. 
//e.g. when window was minimized and gets restored, or if window was behind other windows
void display ()
{
	//Clear the screen
	//Clears both the screen's colour and the depth of anything it is displaying
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//drawTerrain = false;
	//drawSkyBox = false;
	//drawTiger = false;
	//useReflection = true;
	
	if (useReflection)
	{
		writeToCubeMapBuffer();
		useReflection = false;
	}
	
	drawScene();
	
	//Instruct OpenGL to send all our commands to the graphics card (if it hasn't done so already)
	glFlush();
	
	//Swap the buffers. i.e. we write to one while other displays to prevent "screen tearing" where half of the old pixels still remain.
	glutSwapBuffers();
	
	printFPS();
	
	last_frame = getTime();
}

void drawScene()
{
	updateFog();
	
	if (drawGrass)
	{
		glDisable(GL_CULL_FACE);
		mat4 grassWorld;
		/*for (float i = 0; i < 2760; i += 0.5f)
		{
			mat4 grassWorld;
			grassWorld = translate(grassWorld, vec3(i,500.0f,i));
			grassShader->apply();
			grassShader->updateWorldMatrix(grassWorld);
			grassPlane->draw(); 
		}*/
		
		/*for(int i = 0; i < 2760 * 3; i+=3)
		{
			cout<<"coords in main: "  << grassCoordinates[i] <<"," << grassCoordinates[i+1] <<","  << grassCoordinates[i+2] << endl;
		}*/
		for (int i = 0; i < 2760 * 3; i+=3)
		{
			mat4 grassWorld;
			grassWorld = translate(grassWorld, vec3(grassCoordinates[i],grassCoordinates[i+1],grassCoordinates[i+2]));
			grassShader->apply();
			grassShader->updateWorldMatrix(grassWorld);
			grassPlane->draw(); 
			//cout << "Grass Pos: (" << grassCoordinates[i] << "," << grassCoordinates[i+1] << "," << grassCoordinates[i+2] << ")" << endl;
		}
		/*mat4 grassWorld;
		grassWorld = translate(grassWorld, vec3(1.0f, 0.0f, 1.0f));
		grassShader->apply();
		grassShader->updateWorldMatrix(grassWorld);
		grassPlane->draw(); 
		grassWorld = translate(grassWorld, vec3(2.0f, 2.0f, 2.0f));
		grassShader->updateWorldMatrix(grassWorld);
		grassPlane->draw(); */
		glEnable(GL_CULL_FACE);
	}
	
	if(drawTerrain)
	{	
		terrainShader->apply();
		grid->draw();
	}
	
	if(drawTiger)
	{
		
		tigerShader->apply();
		if (enableFog)
		{
			tigerShader->updateUniform("fog", 1.0f);
		}
		else
		{
			tigerShader->updateUniform("fog", 0.0f);
		}
		tiger->draw();
	}
	
	if (drawSkyBox)
	{
		GLint OldCullFaceMode;
		glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
		GLint OldDepthFuncMode;
		glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);

		mat4 skyboxWorld;
		skyboxWorld = translate(skyboxWorld, vec3(cameraEye.x, cameraEye.y, cameraEye.z));
		skyboxShader->updateWorldMatrix(skyboxWorld);
		skyboxShader->apply();
		
		skyboxShader->updateUniform("mixRatio", mixRatio);
		cube->draw();
		
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wsign-conversion"
		glCullFace(OldCullFaceMode); 
		glDepthFunc(OldDepthFuncMode);
		#pragma GCC diagnostic pop
	}
	if (drawWater)
	{	//glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		waterShader->apply();
		if (enableFog)
		{
			//waterShader->updateUniform("fog", 1.0f);
		}
		else
		{
			//waterShader->updateUniform("fog", 0.0f);
		}
		waterPlane->draw();
		//glEnable(GL_DEPTH_TEST);
		
		glEnable(GL_CULL_FACE);
	}
	
	
	
	reflection->apply();
	reflectSphere->apply();
	reflectSphere->updateUniform("mixRatio", mixRatio);
	
	sphere->draw();
	
	/*glDisable(GL_CULL_FACE);
	glRasterPos3f(30.0f, 25.0f, 0.0f);
	string s = "Hello";
	for( int i = 0; i < s.length(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
		//cout << GLUT_BITMAP_HELVETICA_18 << endl;
	}
	glEnable(GL_CULL_FACE);*/
	
}

void writeToScreenShotBuffer()
{
	screenShot->bind();
	FOVY = 90.0f;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//updateAlternateView();
	updateView();
	drawScene();

	screenShot->unbind();
	
	FOVY = 45.0f;
	
	updateView();
}

void writeToCubeMapBuffer()
{
	reflection->bind();
	
	//Set altCameraEye to position of object!!******
	FOVY = 90.0f;
	
	updateProjection(1,1);
	
	//Generate and bind 6 textures
	for (int i = 0; i < 6; i ++)
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		switch(i)
		{
			case 0:
				//POSITIVE X
				altCameraAt.x = 10000.0f;
				altCameraAt.y = altCameraEye.y;
				altCameraAt.z = altCameraEye.z;
				
				//changeViewDirection(-10.0f,0.0f);
			
				updateAlternateView();
				reflection->attachToNewTexture(0);
				drawScene();
			
				//reflection->write(1);
			break;
			
			case 1:
				//NEGATIVE X
				altCameraAt.x = -10000.0f;
				altCameraAt.y = altCameraEye.y;
				altCameraAt.z = altCameraEye.z;
			
				updateAlternateView();
				reflection->attachToNewTexture(1);
				drawScene();
			break;
			
			case 2:
				//POSITIVE Y
				altCameraAt.x = 10000.0f;
				altCameraAt.y = altCameraEye.y;
				altCameraAt.z = altCameraEye.z;
	
				changeViewDirection(0.0f,89.5f);
			
				updateAlternateView();
				reflection->attachToNewTexture(2);
				drawScene();
			
				//reflection->write(3);
				
			break;
			
			case 3:
				//NEGATIVE Y
				altCameraAt.x = 10000.0f;
				altCameraAt.y = altCameraEye.y;
				altCameraAt.z = altCameraEye.z;
	
				changeViewDirection(0.0f,-89.50f);
			
				updateAlternateView();
				reflection->attachToNewTexture(3);
				drawScene();
				
			break;
			
			case 4:
				//POSITIVE Z
				altCameraAt.x = 10000.0f;
				altCameraAt.y = altCameraEye.y;
				altCameraAt.z = altCameraEye.z;
	
				changeViewDirection(-90.0f,0.0f);
			
				updateAlternateView();
				reflection->attachToNewTexture(4);
				drawScene();
			
				//reflection->write(5);
			
				
			break;
			
			case 5:
				//NEGATIVE Z
				altCameraAt.x = 10000.0f;
				altCameraAt.y = altCameraEye.y;
				altCameraAt.z = altCameraEye.z;
	
				changeViewDirection(90.0f,0.0f);
			
				updateAlternateView();
				reflection->attachToNewTexture(5);
				drawScene();
				
			break;
			
			default:
				cout << "[ERROR] Cannot create that face number when writing to CubeMapBuffer in Main.C" << endl;
			break;
		}
	}
	
	for(int i = 0; i < 6; i++)
	{
		reflection->attachToNewTexture(i);
		reflection->write(i+1);
	}
	
	reflection->unbind();
	
	FOVY = 45.0f;
	updateView();
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void keyboardUp(unsigned char key, int x, int y)
{
	active_keys[key] = false;
}
#pragma GCC diagnostic error "-Wunused-parameter"

void keyboard(unsigned char key, int x, int y)
{
	//int size;
	vec3 yAxis (0, 1, 0);
	active_keys[key] = true;
	
    switch (key)
   {
	case '1':
		//toggle skybox
		cout << "drawSkybox: " << drawSkyBox << endl; 
		if (drawSkyBox)
		{
			drawSkyBox = false;
		}
		else
		{
			drawSkyBox = true;
		}
	break;
	
	case '2':
		//toggle terrain		
		if (drawTerrain)
		{
			drawTerrain = false;
		}
		else
		{
			drawTerrain = true;
		}
		
	break;
	
	case '3':
		//toggle tiger		
		if (drawTiger)
		{
			drawTiger = false;
		}
		else
		{
			drawTiger = true;
		}
		
	break;
	
	case '4':
		//toggle reflection	
		if (useReflection)
		{
			useReflection = false;
		}
		else
		{
			useReflection = true;
		}
		
	break;
		
	case '5':
		if (drawWater)
		{
			drawWater = false;
		}
		else
		{
			drawWater = true;
		}
	break;
	
	case '6':
		if (drawGrass)
		{
			drawGrass = false;
		}
		else
		{
			drawGrass = true;
		}
	break;
		
	case 'f':
		//toggle fog
		if (enableFog)
		{
			enableFog = false;
		}
		else
		{
			enableFog = true;
		}
	break;

       case 'z':
		xScale = yScale = zScale = 1.0f;
		xRot = yRot = zRot = 0.0f;
		xMove = yMove = zMove = -0.5f;
		FOVY = 45.0f; 
	        cameraAt = vec3(64,1,-64);
	        cameraEye = vec3(0,2,0); 
	        old_cameraEye = cameraEye;
		old_cameraAt = cameraAt;
	break;
       
	case '=':
		if (mixRatio < 1.0f)
		{
			cout << "Mix Ratio: " << mixRatio << endl;
			mixRatio = mixRatio + 0.01f;
		}
         break;
	
	case '-':
		if (mixRatio > 0.0f)
		{
			mixRatio = mixRatio - 0.01f;
		}
		
         break;
	
	case ',':
		toggleWireFrame();
	break;
	
	case '.':
		 cout << "CONTROLS: " << endl;
			cout << endl;
			cout << "1: Flat Shading" << endl;
			cout << "2: Gouraud Shading" << endl;
			cout << "3: Phong Shading" << endl;
			cout << ",: Toggle Wire Frame" << endl;
			cout << "w: Increase Rotation about X" << endl;
			cout << "s: Decrease Rotation about X" << endl;
			cout << "a: Increase Rotation about Y" << endl;
			cout << "d: Decrease Rotation about Y" << endl;
			cout << "z: Position camera at default position" << endl;
			cout << "q: Increase Rotation about Z" << endl;
			cout << "e: Decrease Rotation about Z" << endl;
			cout << "t: Move up" << endl;
			cout << "g: Move down" << endl;
			cout << "f: Move left" << endl;
			cout << "h: Move right" << endl;
			cout << "r: Move deeper into scene" << endl;
			cout << "y: Move towards the viewer" << endl;
			cout << "i: Stretch Y axis" << endl;
			cout << "k: Shrink Y axis" << endl;
			cout << "j: Stretch X axis" << endl;
			cout << "l: Shrink X axis" << endl;
			cout << "u: Stretch Z axis" << endl;
			cout << "o: Shrink Z axis" << endl;
			cout << "z: Reset" << endl;
			cout << "c: Increase tesselation" << endl;
			cout << "x: Decrease tesselation" << endl;
			cout << ".: help" << endl;
			cout << endl;
	break;
	
	case ']':
		if (!anti_alias)
		{
			cout << "Applying anti-aliasing" << endl;
			glEnable(GL_POINT_SMOOTH);
			glEnable(GL_LINE_SMOOTH);
			//glEnable(GL_POLYGON_SMOOTH);
			
			anti_alias = true;
		}
		else if (anti_alias)
		{
			cout << "Disabling anti-aliasing" << endl;
			glDisable(GL_POINT_SMOOTH);
			glDisable(GL_LINE_SMOOTH);
			//glDisable(GL_POLYGON_SMOOTH);
			
			anti_alias = false;
		}
	break;
		
	case 'x':
		
	break;
		
	/*case '5':
		//POSITIVE Y
		cameraAt.x = 10000.0f;
		//cameraAt.y = 0.0f;
		cameraAt.y = cameraEye.y;
		//cameraAt.z = 0.0f;
		cameraAt.z = cameraEye.z;
	
		lookDirection(0.0f,89.5f);
	break;
	
	case '6':
		//NEGATIVE Y
		cameraAt.x = 10000.0f;
		//cameraAt.y = 0.0f;
		cameraAt.y = cameraEye.y;
		//cameraAt.z = 0.0f;
		cameraAt.z = cameraEye.z;
	
		lookDirection(0.0f,-89.50f);
	break;
	
	case '7':
		//POSITIVE X
		cameraAt.x = 10000.0f;
		//cameraAt.y = 0.0f;
		cameraAt.y = cameraEye.y;
		//cameraAt.z = 0.0f;
		cameraAt.z = cameraEye.z;
	
		lookDirection(0.0f,0.0f);
	break;
	
	case '8':
		//NEGATIVE X
		cameraAt.x = -10000.0f;
		//cameraAt.y = 0.0f;
		cameraAt.y = cameraEye.y;
		//cameraAt.z = 0.0f;
		cameraAt.z = cameraEye.z;
	
		lookDirection(0.0f,0.0f);
	break;
	
	case '9':
		//POSITIVE Z
		cameraAt.x = 10000.0f;
		//cameraAt.y = 0.0f;
		cameraAt.y = cameraEye.y;
		//cameraAt.z = 0.0f;
		cameraAt.z = cameraEye.z;
	
		lookDirection(-90.0f,0.0f);
	break;
	
	case '0':
		//NEGATIVE Z
		cameraAt.x = 10000.0f;
		//cameraAt.y = 0.0f;
		cameraAt.y = cameraEye.y;
		//cameraAt.z = 0.0f;
		cameraAt.z = cameraEye.z;
	
		lookDirection(90.0f,0.0f);
	break;*/
	       
	case 'c':
		xScale = yScale = zScale = 1.0f;
		xRot = yRot = zRot = 0.0f;
		xMove = yMove = zMove = -0.5f;
		FOVY = 45.0f; 
		
		if (birdsEye)
		{
			
			cameraAt = old_cameraAt;
			cameraEye = old_cameraEye; 
				
			birdsEye = false;

		}
	break;
		
	case 'v':
		cout << "Camera:\n Position = (" << cameraEye.x << "," << cameraEye.y << "," << cameraEye.z <<")";
		cout << "\nUp = (" << cameraUp.x << "," << cameraUp.y << "," << cameraUp.z <<")";
		cout << "\nLooking At = (" << cameraAt.x << "," << cameraAt.y << "," << cameraAt.z <<")" << endl;
	break;
	
	case 'p':
	writeToScreenShotBuffer();
         screenShot->bind();
         screenShot->write();
         screenShot->unbind();
         break;
	
	case KEY_ESCAPE:
         printf("Bye!\n");
         exit(0);
         break;
	
      default:
         printf("Key '%c' pressed, mouse at (%d, %d).\n", key, x, y);
         break;
   }
	updateView();
	updateProjection(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutPostRedisplay();

}

void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
		{
			cout << "Left ";
			break;
		}
		case GLUT_MIDDLE_BUTTON:
		{
			cout << "Middle ";
			break;
		}
		case GLUT_RIGHT_BUTTON:
		{
			cout << "Right ";
			break;
		}
		
		default:
			cout << "unknown ";
			break;
	}
	
	cout << "mouse button ";
	
	switch (state)
	{
		case GLUT_UP:
			cout << "released ";
			break;
		
		case GLUT_DOWN:
			cout << "pressed ";
			break;
		
		default:
			cout << "did something we don't know...";
			break;

	}
	
	cout << "at [" << x << ", " << y << "] " << endl;
}

void lookDirection(GLfloat hRotation, GLfloat vRotation)
{
	vec3 direction = (cameraAt - cameraEye);
	vec3 right = glm::cross(direction, cameraUp);
	
	mat4 rotation; //create empty 4x4 rotation matrix
	rotation = glm::rotate (rotation, hRotation, cameraUp);
	rotation = glm::rotate (rotation, vRotation, right);
	
	glm::vec4 d (direction.x, direction.y, direction.z, 0.0f);//create a vec4 direction vector so that we can rotate it
	
	d = rotation * d;
	
	cameraAt = cameraEye + vec3(d.x, d.y, d.z);
}

void changeViewDirection(GLfloat hRotation, GLfloat vRotation)
{
	vec3 direction = (altCameraAt - altCameraEye);
	vec3 right = glm::cross(direction, altCameraUp);
	
	mat4 rotation; //create empty 4x4 rotation matrix
	rotation = glm::rotate (rotation, hRotation, cameraUp);
	rotation = glm::rotate (rotation, vRotation, right);
	
	glm::vec4 d (direction.x, direction.y, direction.z, 0.0f);//create a vec4 direction vector so that we can rotate it
	
	d = rotation * d;
	
	altCameraAt = altCameraEye + vec3(d.x, d.y, d.z);
}

void mouseLook (int x, int y)
{
	int horizontal_centre = WINDOW_WIDTH / 2;
	int vertical_centre = WINDOW_HEIGHT / 2;
	
	const int xDistance = -(x - horizontal_centre); //negated because positive rotation is ant-clockwise
	const int yDistance = -(y - vertical_centre);
	
	//We need to find our angle of rotation (max 180 degrees) by scaling max rotation (180) with the ratio of screen moved from centre
	GLfloat horizontalScale = (GLfloat) xDistance / (GLfloat) horizontal_centre;
	GLfloat verticalScale = (GLfloat) yDistance / (GLfloat) vertical_centre;
	
	GLfloat vRotation = verticalScale * 3.14f; //NB: 3.14f is 180 degrees in radions
	GLfloat hRotation = horizontalScale * 3.14f;
	
	//calc current direction vector
	vec3 direction = (cameraAt - cameraEye);
	
	//calculate current right vector
	vec3 right = glm::cross(direction, cameraUp);//will always stay horizontal
	
	//We can thus rotate around the "y-axis" (CameraUp) for left/right looking
	//We can thus rotate around the "x-axis" (right) for up/down looking
	
	mat4 rotation; //create empty 4x4 rotation matrix
	rotation = glm::rotate (rotation, hRotation, cameraUp);
	rotation = glm::rotate (rotation, vRotation, right);
	
	glm::vec4 d (direction.x, direction.y, direction.z, 0.0f);//create a vec4 direction vector so that we can rotate it
	
	d = rotation * d;
	
	cameraAt = cameraEye + vec3(d.x, d.y, d.z);
	
	//Trap mouse in the window
	if ((xDistance!= 0) || (yDistance != 0))
	{
		//each call to this function generates another motion event.
		//Therefore if we do not check if the mouse has moved from centre, this will be continuously called, allowing for no possible mouse movement
		glutWarpPointer(horizontal_centre, vertical_centre);
	}
	
}

void toggleWireFrame()
{
	grid->toggleWireFrame();
	//cube->toggleWireFrame();
}

void reshape(int newWidth, int newHeight)
{
	//Fix our viewport
	glViewport (0, 0, newWidth, newHeight);
	
	if (screenShot != NULL)
	{
		delete screenShot;
	}
	screenShot = new ColourBuffer(newWidth, newHeight);
	
	if (reflection != NULL)
	{
		delete reflection;
	}
	reflection = new CubeMap(newWidth, newHeight);
	//screenShot->unbind();
	
	//Ask GLUT for a redraw. Tells GLUT to call display function
	glutPostRedisplay();
}

void updateView()
{
	mat4 worldView = lookAt(cameraEye, cameraAt, cameraUp);
	
	vec3 xAxis(1,0,0);
	worldView = rotate(worldView, xRot,xAxis);
	
	vec3 yAxis(0,1,0);
	worldView = rotate(worldView, yRot, yAxis);
	
	vec3 zAxis(0,0,1);
	worldView = rotate(worldView, zRot, zAxis);
	
	terrainShader->updateViewMatrix(worldView);
	skyboxShader->updateViewMatrix(worldView);
	tigerShader->updateViewMatrix(worldView);
	reflectSphere->updateViewMatrix(worldView);
	waterShader->updateViewMatrix(worldView);
	grassShader->updateViewMatrix(worldView);
	//phongShader->updateViewMatrix(worldView);
}

void updateAlternateView()
{
	mat4 worldView = lookAt(altCameraEye, altCameraAt, altCameraUp);
	
	terrainShader->updateViewMatrix(worldView);
	skyboxShader->updateViewMatrix(worldView);
	tigerShader->updateViewMatrix(worldView);
	reflectSphere->updateViewMatrix(worldView);
	waterShader->updateViewMatrix(worldView);
	grassShader->updateViewMatrix(worldView);
	//phongShader->updateViewMatrix(worldView);
}

void updateFog()
{
	if (enableFog)
	{
		terrainShader->apply();
		terrainShader->updateUniform("fog", 1.0f);
		//tigerShader->apply();
		//tigerShader->updateUniform("fog", 1.0f);
		//reflectSphere->updateUniform("fog", 1.0f);
	}
	else
	{
		terrainShader->apply();
		terrainShader->updateUniform("fog", 0.0f);
		//tigerShader->apply();
		//tigerShader->updateUniform("fog", 0.0f);
		//reflectSphere->updateUniform("fog", 0.0f);
	}
	
}

void updateProjection(int width, int height)
{
	GLfloat aspect = (GLfloat)width / (GLfloat)height; //Aspect ratio, where width and height are the dimensions of the window
	
	mat4 projection = perspective(FOVY, aspect, NEAR, FARV);
	
	terrainShader->updateProjectionMatrix(projection);
	skyboxShader->updateProjectionMatrix(projection);
	tigerShader->updateProjectionMatrix(projection);
	reflectSphere->updateProjectionMatrix(projection);
	waterShader->updateProjectionMatrix(projection);
	grassShader->updateProjectionMatrix(projection);
	//phongShader->updateProjectionMatrix(projection);
}

int getTime()
{
	//Return time in milliseconds since the program started (Elapsed Time)
	return glutGet(GLUT_ELAPSED_TIME);
}

void printFPS()
{
	
	int now = getTime();
		
	if (now - last_print > MILLISECONDS_PER_SECOND)
	{
		//Measure the actual time between to frames
		int deltaT = now - last_frame;
	
		//Convert deltaT to seconds 
		double seconds = (double)deltaT/MILLISECONDS_PER_SECOND; 
		double fps = 1.0 / seconds; //1.0 instead of 1 so that we get floating point division.
		
		cout << "FPS: "<<fps<<endl;
		last_print = now;
	}
}

void idle()
{
	int now = getTime();
	int deltaT = now - last_frame;
	const GLfloat seconds = (GLfloat)deltaT / (GLfloat)MILLISECONDS_PER_SECOND; //Get deltaT in seconds
	const GLfloat step = seconds * 10.0f;
	
	if (deltaT >= TARGET_DELTA_T)
	{
		vec3 d = step  + 0.1f * glm::normalize(cameraAt - cameraEye);
		vec3 r = 0.1f * glm::normalize(glm::cross(d, cameraUp));
		
		checkKeys(d, r);
	}

}

void checkKeys(vec3 d, vec3 r)
{
	if (active_keys['w'])
	{
		//cout << d << endl;
		cameraEye += 3.0f*d;
		cameraAt += 3.0f*d;
		
	}
	else if (active_keys['s'])
	{
		//cout << d << endl;
		cameraEye -= d;
		cameraAt -= d;
	}
	
	if (active_keys['d'])
	{
		cameraEye += r;
		cameraAt += r;
	}
	else if (active_keys['a'])
	{
		cameraEye -= r;
		cameraAt -= r;
	}
	
	updateView();
	glutPostRedisplay();
}

void initGLEW()
{
	//initialise GLEW (GL Extension Wrangler - Managers/Wrangles shader programs)
	glewInit();
	
	if (!GLEW_VERSION_3_2)
	{
		fprintf(stderr, "This progarm requires OpenGL 3.2");
		exit(-1);
	}
}

void init ()
{
	glEnable(GL_VERTEX_ARRAY);//?
	//switch on Depth Testing to ensure that if something is behind something, it does not show
	glEnable(GL_DEPTH_TEST);
	//switch on Face Culling (Gets rid of faces looking away from us)
	glEnable(GL_CULL_FACE);
	
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Switch openGL into wireframe drawing mode
	//glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_BACK, GL_LINE);
	
	//set the "clear colour" (background colour in any other API) to blue. (red, green, blue, alpha)
	glClearColor(0.0, 0.0, 1.0, 1.0);
	
	//compile and link shader programs
	//loadShaderPrograms();
	//flatShader = new Shader("terrain");

	
	
	terrainShader = new Shader("Grid");
	terrainShader->apply();
	grid = new Terrain("heightmaps/GoodHeightmap1.png");
	
	grassCoordinates = new float[2760 * 3];
	grassCoordinates = grid->getCoordinates();
	//cout << "HERE!!!!! " << hhh << endl;
	
	
	
	
	mat4 tigerWorld;
	//tigerWorld = scale(tigerWorld, vec3(0.025f,0.025f,0.025f));
	tigerWorld = translate(tigerWorld, vec3(60,83,185));
	tigerWorld = scale(tigerWorld, vec3(0.015f,0.015f,0.015f));
	
	//Texture* tex = new Texture("textures/bricks", "bricks.png"); 
	//tex->load();
	tigerShader = new Shader("palm");
	tigerShader->updateWorldMatrix(tigerWorld);
	tigerShader->apply();
	tiger = new ModelLoadedMesh("models/Tiger.obj");
	
	mat4 sphereWorld;
	sphereWorld = translate(sphereWorld, vec3(60,120,185));
	sphereWorld = scale(sphereWorld, vec3(20,20,20));
	reflectSphere = new Shader("ReflectionSphere");
	
	reflectSphere->updateWorldMatrix(sphereWorld);
	reflectSphere->apply();
	sphere = new Sphere(1000);
	
	mat4 waterWorld;
	waterWorld = translate(waterWorld, vec3(7.0f, 10.0f, 9.0f));
	//waterWorld = translate(waterWorld, vec3(7.0f, 90.0f, 9.0f));
	waterShader = new Shader("water");
	waterShader->updateWorldMatrix(waterWorld);
	waterShader->apply();
	waterPlane = new Grid(2048, 100.0f);
	
	mat4 grassWorld;
	grassWorld = translate(grassWorld, vec3(1.0f, 0.0f, 1.0f));
	//grassWorld = scale(grassWorld, vec3(4.0f, 50.0f,4.0f));
	grassShader = new Shader("water2");
	grassShader->updateWorldMatrix(grassWorld);
	grassShader->apply();
	grassPlane = new Board(2.0f, 2.0f, 2.0f); 
	//grassPlane = new Grid(10.0f, 1.0f); 
	
	
	skyboxShader = new Shader("Skybox");
	mat4 skyboxWorld;
	skyboxWorld = translate(skyboxWorld, vec3(cameraEye.x, cameraEye.y, cameraEye.z));
	//skyboxWorld = translate(skyboxWorld, vec3(0, 10, 100));
	skyboxShader->updateWorldMatrix(skyboxWorld);
	skyboxShader->apply();
	cube = new Cube(20);
	
	//CubeMapTexture* t = new CubeMapTexture("textures/skybox", "terrain_positive_x.png", "terrain_negative_x.png", "terrain_positive_y.png", "terrain_negative_y.png", "terrain_positive_z.png", "terrain_negative_z.png");
	
	
	glActiveTexture(GL_TEXTURE2);
	waterTex = new Texture("textures/skybox", "evening_dunes_positive_y.png", false); 
	//waterTex->load();
	
	glActiveTexture(GL_TEXTURE3);
	grassTex = new Texture("textures/grass", "grass2.png", true); 
	//cout << "Finished making texture" << endl;
	//grassTex->load();
	
	glActiveTexture(GL_TEXTURE1);
	daySkyBox = new CubeMapTexture("textures/skybox", "dunes_positive_x.png", "dunes_negative_x.png", "dunes_positive_y.png", "dunes_negative_y.png", "dunes_positive_z.png", "dunes_negative_z.png");
	daySkyBox->load();
	
	glActiveTexture(GL_TEXTURE0);
	nightSkyBox = new CubeMapTexture("textures/skybox", "evening_dunes_positive_x.png", "evening_dunes_negative_x.png", "evening_dunes_positive_y.png", "evening_dunes_negative_y.png", "evening_dunes_positive_z.png", "evening_dunes_negative_z.png");
	nightSkyBox->load();
	
	//vector<vec3> grassPos;
	//grassPos = grid->getHeights(65.0f);
	
	

	updateProjection(WINDOW_WIDTH, WINDOW_HEIGHT);
	updateView();
	
	cout << "CONTROLS: " << endl;
			cout << endl;
			cout << "1: Flat Shading" << endl;
			cout << "2: Gouraud Shading" << endl;
			cout << "3: Phong Shading" << endl;
			cout << ".: Toggle Wire Frame" << endl;
			cout << "w: Increase Rotation about X" << endl;
			cout << "s: Decrease Rotation about X" << endl;
			cout << "a: Increase Rotation about Y" << endl;
			cout << "d: Decrease Rotation about Y" << endl;
			cout << "z: Position camera at default position" << endl;
			cout << "q: Increase Rotation about Z" << endl;
			cout << "e: Decrease Rotation about Z" << endl;
			cout << "t: Move up" << endl;
			cout << "g: Move down" << endl;
			cout << "f: Move left" << endl;
			cout << "h: Move right" << endl;
			cout << "r: Move deeper into scene" << endl;
			cout << "y: Move towards the viewer" << endl;
			cout << "i: Stretch Y axis" << endl;
			cout << "k: Shrink Y axis" << endl;
			cout << "j: Stretch X axis" << endl;
			cout << "l: Shrink X axis" << endl;
			cout << "u: Stretch Z axis" << endl;
			cout << "o: Shrink Z axis" << endl;
			cout << "z: Reset" << endl;
			cout << "c: Increase tesselation" << endl;
			cout << "x: Decrease tesselation" << endl;
			cout << "/: help" << endl;
			cout << endl;
	
	last_print = getTime();
	last_frame = getTime();
}

void cleanUp()
{
	delete phongShader;
	phongShader = NULL;

	int size=(int)(sizeof shaders/sizeof(Shader*));
		
		for(int i = 0; i < size; i ++)
		{
			shaders[i]=NULL;
		}
}

int main (int argc, char** argv)
{
	glutInit(&argc, argv); //Glut needs access to argc and argv since it implements some command line switches
	
	//Use two buffers to prevent screen tearing, 
	//use a depth buffer to work in 3D, 
	//and use a colour screen
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	//glutInitWindowSize(640, 480); //Size of the window we would like
	glutInitWindowSize(1280, 960);
	glutCreateWindow("COS344_Assignment_3");//Set title of the window
	
	glutDisplayFunc(display); //Register Display Event/ Callback
	glutSetCursor(GLUT_CURSOR_NONE);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouseLook);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	initGLEW();
	init();
	
	//Hand over to GLUT
	//Will cause GLUT to start calling our callback functions (like Display) and handle anything else it needs to.
	glutMainLoop();
	
	cleanUp();
	
	return 0;	
}
