#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Angel.h"
#include "math.h"
#include "Angel\Object.h"
#include "User.h"
#include "Shader.h"
#include "Material.h"
#include "Scene.h"


using namespace Angel;
using namespace std;

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
int WIN_ID;
float speed = 0.1f;
bool fullscreen = false;
bool showMsPerFrame = false;
bool flyMode = false;
bool reloadShaders = false;
int frames = 0;
int oldTimeSinceStart = 0;

vec4 background_color = vec4(0.1f, 0.3f, 0.6f, 1.0f);

GLuint projectionUniform, modelViewUniform;
GLuint shaderProgram;
GLuint colorAttribute, positionAttribute, normalAttribute;

void loadShader();
void setupScene();
void render();
void loadGeometry();
void init(int, char**);
void changeViewPort(int w, int h);
void loop();
void showControls();
void buildFrameBufferObject(int id);
void updateShadowTexture(int id);
void keyOperations();
void keyPressed(unsigned char, int, int);
void keyReleased(unsigned char, int, int);
void mouseMovement(int x, int y);
void loadShaders();

GLuint loadBufferData(Vertex* vertices, int vertexCount, Shader shader);

User user;
Shader shader, shadowShader;
Scene scene;
bool* keyStates = new bool[256];

int main(int argc, char* argv[])
{
	cout << "--------------------------------BENGINE 3D v0.1---------------------------------" << endl;
	cout << endl;
	cout << "  Press 'h' for help" << endl;

	shadowShader = Shader("shadow.vert", "shadow.frag");
	shader = Shader("diffuse.vert", "diffuse.frag");

	init(argc, argv);
	
	// Error handling
	GLenum err = glewInit();
	if (GLEW_OK != err){
		fprintf(stderr, "GLEW error");
		return 1;
	}
	return 0;
}
// Initializes glut and creates a window
void init(int argc, char* argv[])
{
	// Initialize GLUT
	glutInit(&argc, argv);
	// Setup some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// Set the window size
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// Set the window position
	glutInitWindowPosition(800, 100);
	// Creates the window with a title
	WIN_ID = glutCreateWindow("Bengine3D");
	// Binds the functions to respond when necessary
	glutDisplayFunc(render);
	glutReshapeFunc(changeViewPort);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyReleased);
	glutPassiveMotionFunc(mouseMovement);
	// Removes the cursor
	glutSetCursor(GLUT_CURSOR_NONE);
	// Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// Initialize openGL
	Angel::InitOpenGL();
	// load shaders
	loadShaders();
	// Setups the scene (create objects, user, and lights)
	setupScene();
	// Build frame buffer object for shadow mapping
	for (int i = 0; i < scene.getNumberOfLights(); i++){
		buildFrameBufferObject(i);
	}
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	// Check for errors
	Angel::CheckError();
	// Enters the GLUT event processing loop
	glutMainLoop();
}
// Function to load the shaders
void loadShaders(){
	shader.loadShader();
	shadowShader.loadShader();
}
// Setup the scene here!q
void setupScene(){
	scene = Scene();
	// Materials
	// Internet page with material properties: http://www.real3dtutorials.com/tut00008.php
	//Material ruby = Material(vec4(0.61424f, 0.04136f, 0.04136f, 1.0f), vec4(0.727811f, 0.626959f, 0.626959f, 1.0f), 76.8f);
	//Material chrome = Material(vec4(0.4f, 0.4f, 0.4f, 1.0f), vec4(0.774597f, 0.774597f, 0.774597, 1.0f), 76.8f);
	Material defaultMat = Material();
	user = User(vec3(5.0f, 0.5f, 0.0f));
	Material copper = Material(vec4(0.7038f, 0.27048f, 0.0828f, 1.0f), vec4(0.256777f, 0.137622f, 0.086014f, 1.0f), 12.8);
	copper.setDiffuseColor(vec4(1.0f));
	copper.createTexture(400, 400, "./Textures/copper.bmp");
	Material box = Material(512, 512, "./Textures/box.bmp");
	Material grass = Material(640, 640, "./Textures/grass.bmp", vec2(30.0f));
	Material checkerBoard = Material(512, 512, "./Textures/checkerBoard.bmp", vec2(0.5f));
	// The User
	user = User(vec3(5.0f, 0.5f, 0.0f));
	// Objects
	scene.createPlane(vec3(0.0f, 0.0f, 0.0f), vec2(20.0f, 20.0f), shader, grass);
	scene.createBox(vec3(0.0f, 5.0f, 0.0f), vec3(2.0f), shader, checkerBoard, vec3(0.0f, 45.0f, 0.0f));
	scene.createBox(vec3(0.0f, 0.5f, 2.0f), vec3(1.0f), shader, box, vec3(0.0f, 45.0f, 0.0f));
	scene.createPlane(vec3(5.0f, 0.05f, 5.0f), vec2(2.0f, 2.0f), shader, copper);
	scene.createOBJ(vec3(-5.0f, 1.0f, -5.0f), vec3(1.0f), shader, defaultMat, vec3(0.0f), "./Models/test.obj");
	// Ligth Sources
	scene.createLightSource(vec3(0.5f, 0.5f, 0.0f), vec3(1.0f), 1);
	scene.createLightSource(vec3(-0.5f, 0.5f, 0.0f), vec3(1.0f), 0);
}
// Function to render the scene, called each time the window needs to be redrawn
void render()
{
	// Reload the shaders if it is necessary
	if (reloadShaders){
		reloadShaders = false;
		loadShaders();
		cout << "  Shaders reloaded" << endl;
	}
	// Check for key inputs
	keyOperations();
	// Use the shadow shader and render the scene from the lights point of view in updateShadowTexture
	shadowShader.useShader();
	for (int i = 0; i < scene.getNumberOfLights(); i++){
		updateShadowTexture(i);
	}
	// Clears the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//updateShadowTexture();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	// Sets the background color
	glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
	// Use the normal shader and sets the projection and shadow texture uniforms
	mat4 projection = Perspective(53.13f, WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.001f, 10000.0f);
	shader.useShader();
	shader.setProjectionUniform(projection);
	shader.setUserPosUniform(user.getPosition());
	scene.setShadowUniforms(shader);
	// Draw the scene from the users point of view
	mat4 view = user.getViewMatrix();
	scene.drawObjects(shader, view);
	// Set the ambient and light uniforms
	scene.setLightUniforms(shader);
	shader.setAmbientUniform(vec4(0.2f));
	// Swaps the buffers of the current window and checks for errors
	glutSwapBuffers();
	Angel::CheckError();
	// Start the loop where the game mechanics is implemented
	loop();
} 
// This function is called each frame.
void loop(){
	// Calculate miliseconds per frame. 
	// Target is 16.6 ms for 60 frames per second
	// Target is 33.3 ms for 30 frames per second
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = timeSinceStart - oldTimeSinceStart;
	frames++;
	if (showMsPerFrame && frames > 300){
		cout << "  Ms per frame: " << deltaTime/(float)frames << endl;
		frames = 0;
		oldTimeSinceStart = timeSinceStart;
	}
	// Rotate the cube slowly
	Object tempObj = scene.getObject(1);
	vec3 rot = tempObj.getRotation();
	tempObj.rotate('Y', rot.y + 1.0f);
	scene.setObject(1, tempObj);
	// Flashlight
	vec3 viewVector = user.getDir() - user.getPosition();
	viewVector.y = 0.0f;
	viewVector = normalize(viewVector);
	vec3 rightVector;
	rightVector.x = -viewVector.z;
	rightVector.z = viewVector.x;
	Light lightTemp = scene.getLight(1);
	lightTemp.setPosition(user.getPosition() + (rightVector * 0.1f) );
	lightTemp.setDir(user.getDir());
	scene.setLight(1, lightTemp);
	// Tells glut to call the render function when finished render the scene.
	glutPostRedisplay();
}
// Build framebufferobject for depthTesting
void buildFrameBufferObject(int id) {
	GLuint depthTextureID, framebufferobject;
	// Generate shadow texture and set the filtering, wraping and comparison parameters
	glGenTextures(1, &depthTextureID);
	glBindTexture(GL_TEXTURE_2D, depthTextureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, scene.getLight(id).getShadowDims(), scene.getLight(id).getShadowDims(), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
	// Generate frame buffer object to draw the depth value into
	glGenFramebuffers(1, &framebufferobject);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferobject);
	// Attaches the depth texture to the framebuffer
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureID, 0);
	// Checks for errors with the frame buffer object
	if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "  Something wrong with FBO" << endl;
	Light temp = scene.getLight(id);
	temp.setShadowBufferObject(framebufferobject);
	temp.setShadowTexture(depthTextureID);
	scene.setLight(id, temp);
	// Binds the normal frame buffer for rendering the scene
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
// Updates the shadow Map
void updateShadowTexture(int id){
	// Disable culling to let the back faces contribute to shadows
	glDisable(GL_CULL_FACE);
	// Bind the framebufferobject assosiated with the shadow texture
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, scene.getLight(id).getShadowBufferObject());
	// Clear depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);
	// Set light view and projection matrix
	mat4 lightView = LookAt(scene.getLightPos(id), scene.getLight(id).getDir(), vec3(0.0f, 1.0f, 0.0f));
	mat4 lightProjection;
	if (scene.getLight(id).getType() == 1){
		lightProjection = Ortho(-30.0f, 30.0f, -30.0f, 30.0f, -10.0f, 60.0);
	}
	else {
		lightProjection = Perspective(53.13f, 1, 0.1f, 60.0f);
	}
	// OPTIONAL bind the texture if another texture is bound!
	// Set the projection uniform and set the viewport to fit the shadow texture
	shadowShader.setProjectionUniform(lightProjection);
	int shadowDims = scene.getLight(id).getShadowDims();
	glViewport(0, 0, shadowDims, shadowDims);
	// Draw from the light’s point of view
	scene.drawObjects(shadowShader, lightView);
	// Bind the normal framebuffer for rendering the normal scene
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	// Reenable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}
// Shows the keys for the different controls
void showControls(){
	cout << endl;
	cout << "-----------------------------------CONTROLS-------------------------------------" << endl;
	cout << "  WASD - Moving the user" << endl;
	cout << "  Move mouse to look arround" << endl;
	cout << "  F - Fullscreen on/off" << endl;
	cout << "  L - Move sunlight dirction from your position to (0,0,0)" << endl;
	cout << "  K - Turn flashlight on and off" << endl;
	cout << "  - and + - Adjust the intensity of the sun light" << endl;
	cout << "  O and p - Adjust the speed of the camera" << endl;
	cout << "  X - Fly mode" << endl;
	cout << "  C - Create a box at the users position" << endl;
	cout << "  R - Reload shaders" << endl;
	cout << "  M - Show the performance in miliseconds per frame (updated every 300 frames)" << endl;
	cout << "  H - Show this info again" << endl;
	cout << "  Q - Quit application" << endl;
	cout << endl;
}
// Function to change the size of the viewport
void changeViewPort(int w, int h){
	// Changes the size of the viewport
	glViewport(0, 0, w, h);
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;
}
// Handles all the keyboard inputs
void keyOperations(){
	if (keyStates['w'] == true){
		if (!flyMode){
			user.moveForward(speed);
		}
		else {
			user.flyForward(speed);
		}
	}
	if (keyStates['s'] == true){
		if (!flyMode){
			user.moveForward(-speed);
		}
		else {
			user.flyForward(-speed);
		}
	}
	if (keyStates['d'] == true){
		user.strafe(speed);
	}
	if (keyStates['a'] == true){
		user.strafe(-speed);
	}
	if (keyStates['q'] == true){
		glutDestroyWindow(WIN_ID);
		exit(0);
	}
	if (keyStates['f'] == true){
		if (!fullscreen){
			glutFullScreen();
			fullscreen = true;
		}
		else {
			glutPositionWindow(800, 100);
			glutReshapeWindow(800, 600);
			fullscreen = false;
		}
		
	}
	if (keyStates['l'] == true){
		scene.setLightPos(user.getPosition(),0);
	}
	if (keyStates['+'] == true){
		scene.setLightInt(scene.getLightInt(0) + 0.1f,0);
		cout << "  Light intensity: " << scene.getLightInt(0) << endl;
	}
	if (keyStates['-'] == true){
		scene.setLightInt(scene.getLightInt(0) - vec3(0.1f),0);
		if (scene.getLightInt(0).x < 0.0f){
			scene.setLightInt(vec3(0.0f), 0);
		}
		cout << "  Light intensity: " << scene.getLightInt(0) << endl;
	}
	if (keyStates['p'] == true){
		speed += 0.01f;
		cout << "  Speed: " << speed << endl;
	}
	if (keyStates['o'] == true){
		speed -= 0.01f;
		if (speed < 0.0f){
			speed = 0.0f;
		}
		cout << "  Speed: " << speed << endl;
	}
	if (keyStates['r'] == true){
		reloadShaders = true;
	}
}
// Called when the mouse is moved
void mouseMovement(int x, int y){
	user.rotateWithMouse(x, y, WINDOW_WIDTH, WINDOW_HEIGHT);
}
// Sets the key states when a key is pressed
void keyPressed(unsigned char key, int x, int y){
	keyStates[key] = true;
	if (key == 'm'){
		showMsPerFrame = !showMsPerFrame;
	}
	else if (key == 'h'){
		showControls();
	}
	else if (key == 'x'){
		flyMode = !flyMode;
		if (!flyMode){
			user.setPostion(vec3(user.getPosition().x, 0.5f, user.getPosition().z));
			cout << "  FlyMode is OFF" << endl;
		}
		else {
			cout << "  FlyMode is ON" << endl;
		}
	}
	else if (key == 'c'){
		Material box = Material(512, 512, "./Textures/box.bmp");
		scene.createBox(user.getPosition(), vec3(1.0f), shader, box, vec3(0.0f, 0.0f, 0.0f));
	}
	else if (key == 'k'){
		if (scene.getLightInt(1).x == 1.0f){
			scene.setLightInt(vec3(0.0f), 1);
			cout << "  Flashlight off" << endl;
		}
		else {
			scene.setLightInt(vec3(1.0f), 1);
			cout << "  Flashlight on" << endl;
		}
	}
}
// Sets the key states when a key is released
void keyReleased(unsigned char key, int x, int y){
	keyStates[key] = false;
}


