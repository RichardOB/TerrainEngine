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
	
	//cube->draw();
	
	
	
	if(drawTerrain)
	{
		terrainShader->apply();
		grid->draw();
	}
		//glDisable(GL_CULL_FACE);
		cowShader->apply();
		cow->draw();
	glEnable(GL_CULL_FACE);
	
	if (drawSkyBox)
	{
		GLint OldCullFaceMode;
		glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
		GLint OldDepthFuncMode;
		glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		//glDepthMask(GL_FALSE);

		mat4 skyboxWorld;
		skyboxWorld = translate(skyboxWorld, vec3(cameraEye.x, cameraEye.y, cameraEye.z));
		skyboxShader->updateWorldMatrix(skyboxWorld);
		skyboxShader->apply();
		cube->draw();
		
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wsign-conversion"
		glCullFace(OldCullFaceMode); 
		glDepthFunc(OldDepthFuncMode);
		#pragma GCC diagnostic pop
		//glDepthMask(GL_TRUE);
		//skybox->render();
	}
	
	//glDisable(GL_CULL_FACE);
	//phongShader->apply();
	//cube->draw();
	
	//Instruct OpenGL to send all our commands to the graphics card (if it hasn't done so already)
	glFlush();
	
	//Swap the buffers. i.e. we write to one while other displays to prevent "screen tearing" where half of the old pixels still remain.
	glutSwapBuffers();
	
	printFPS();
	
	last_frame = getTime();
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
	
	active_keys[key] = true;
	
    switch (key)
   {
      case 'W':
         xRot += ANGLE_DELTA;
         break;
      
      case 'S':
         xRot -= ANGLE_DELTA;
         break;
      
      case 'A':
         yRot += ANGLE_DELTA;
         break;
      
      case 'D':
         yRot -= ANGLE_DELTA;
         break;
      
       case 'Q':
         zRot += ANGLE_DELTA;
         break;
       
      case 'E':
         zRot -= ANGLE_DELTA;
         break;
      
      case 'H':
	xMove += SCALE;
	break;
      
      case 'F':
	xMove -= SCALE;
	break;
      
      case 'T':
	yMove += SCALE;
	break;
      
      case 'G':
	yMove -= SCALE;
	break;
      
      case 'R':
	zMove += SCALE;
	break;
      
      case 'Y':
	zMove -= SCALE;
	break;
      
       case 'I':
	yScale += SCALE;
	break;
       
       case 'K':
	yScale -= SCALE;
	break;
       
       case 'J':
	xScale += SCALE;
	break;
       
       case 'L':
	xScale -= SCALE;
	break;
       
       case 'U':
	zScale += SCALE;
	break;
       
       case 'O':
	zScale -= SCALE;
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
         FOVY += 1;
         break;
	
	case '-':
         FOVY -= 1;
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
	
	case 'C':
		tessLevel += 1.0f;
	break;
	
	case 'X':
		tessLevel -= 1.0f;
	break;
		
	case 'x':
		
		if (!birdsEye)
		{
			xScale = yScale = zScale = 1.0f;
			xRot = yRot = zRot = -0.5f;
			xMove = yMove = zMove = 0.0f;
			FOVY = 45.0f; 
			old_cameraEye = vec3(cameraEye.x, cameraEye.y, cameraEye.z);
			old_cameraAt = vec3(cameraAt.x, cameraAt.y, cameraAt.z);
			cameraAt = vec3(32,1,-32);
			cameraEye = vec3(0,60,0);

			birdsEye = true;
		}
		
	break;
	       
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
	
	//Ask GLUT for a redraw. Tells GLUT to call display function
	glutPostRedisplay();
}

void updateView()
{
	mat4 worldView = lookAt(cameraEye, cameraAt, cameraUp);
	
	terrainShader->updateViewMatrix(worldView);
	skyboxShader->updateViewMatrix(worldView);
	cowShader->updateViewMatrix(worldView);
	//phongShader->updateViewMatrix(worldView);
}

void updateProjection(int width, int height)
{
	GLfloat aspect = (GLfloat)width / (GLfloat)height; //Aspect ratio, where width and height are the dimensions of the window
	
	mat4 projection = perspective(FOVY, aspect, NEAR, FARV);
	
	terrainShader->updateProjectionMatrix(projection);
	skyboxShader->updateProjectionMatrix(projection);
	cowShader->updateProjectionMatrix(projection);
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
		cameraEye += d;
		cameraAt += d;
		cameraEye += d;
		cameraAt += d;
		cameraEye += d;
		cameraAt += d;
		cameraEye += d;
		cameraAt += d;
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
	
	
	//Switch openGL into wireframe drawing mode
	//glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_BACK, GL_LINE);
	
	//set the "clear colour" (background colour in any other API) to blue. (red, green, blue, alpha)
	glClearColor(0.0, 0.0, 1.0, 1.0);
	
	//compile and link shader programs
	//loadShaderPrograms();
	//flatShader = new Shader("terrain");

	//CubeMapTexture* t = new CubeMapTexture("textures/skybox", "terrain_positive_x.png", "terrain_negative_x.png", "terrain_positive_y.png", "terrain_negative_y.png", "terrain_positive_z.png", "terrain_negative_z.png");
	CubeMapTexture* t = new CubeMapTexture("textures/skybox", "dunes_positive_x.png", "dunes_negative_x.png", "dunes_positive_y.png", "terrain_negative_y.png", "dunes_positive_z.png", "dunes_negative_z.png");
	t->load();
	
	terrainShader = new Shader("Grid");
	terrainShader->apply();
	grid = new Terrain("heightmaps/GoodHeightmap1.png");
	
	mat4 tigerWorld;
	//tigerWorld = scale(tigerWorld, vec3(0.025f,0.025f,0.025f));
	tigerWorld = translate(tigerWorld, vec3(60,83,185));
	tigerWorld = scale(tigerWorld, vec3(0.015f,0.015f,0.015f));
	
	Texture* tex = new Texture("textures/bricks", "bricks.png"); 
	tex->load();
	cowShader = new Shader("palm");
	cowShader->updateWorldMatrix(tigerWorld);
	cowShader->apply();
	cow = new ModelLoadedMesh("models/Tiger.obj");
	
	
	skyboxShader = new Shader("Skybox");
	mat4 skyboxWorld;
	skyboxWorld = translate(skyboxWorld, vec3(cameraEye.x, cameraEye.y, cameraEye.z));
	//skyboxWorld = translate(skyboxWorld, vec3(0, 10, 100));
	skyboxShader->updateWorldMatrix(skyboxWorld);
	skyboxShader->apply();
	cube = new Cube(20);
	
	
	//skybox = new Skybox(cameraEye);
	
	
	
	//phongShader  = new Shader ("Grid");
	//phongShader->apply();
	
	//cube = new Cube();

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
