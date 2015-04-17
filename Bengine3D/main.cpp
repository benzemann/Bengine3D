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
void buildFrameBufferObject(int width, int height);
void updateShadowTexture();
void keyOperations();
void keyPressed(unsigned char, int, int);
void keyReleased(unsigned char, int, int);
void mouseMovement(int x, int y);
//TESTING!
void makeCheckImage(void);
static GLubyte checkImage[64][64][4];
GLuint texName;

GLuint loadBufferData(Vertex* vertices, int vertexCount, Shader shader);

int shadowDims = 4096;
GLuint depthTextureID;
GLuint framebufferobject;

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
	// Load the shaders
	shader.loadShader();
	shadowShader.loadShader();
	
	
	//TESTING!
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64,
		64, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		checkImage);
	// Build frame buffer object for shadow mapping
	buildFrameBufferObject(shadowDims, shadowDims);
	// Setups the scene (create objects, user, and lights)
	setupScene();
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	// Check for errors
	Angel::CheckError();
	// Enters the GLUT event processing loop
	glutMainLoop();
}
// TESTING!
void makeCheckImage(void)
{
	int i, j, c;

	for (i = 0; i < 64; i++) {
		for (j = 0; j < 64; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
}
// Setup the scene here!q
void setupScene(){
	scene = Scene();
	Material green = Material(vec4(0.0f, 1.0f, 0.0f, 1.0f), vec4(1.0), 2.0f);
	Material red = Material(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	Material blue = Material(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	user = User(vec3(5.0f, 0.5f, 0.0f));
	scene.createPlane(vec3(0.0f, 0.0f, 0.0f), vec2(20.0f, 20.0f), shader, green);
	scene.createBox(vec3(0.0f, 5.0f, 0.0f), vec3(2.0f), shader, blue, vec3(0.0f, 45.0f, 0.0f));
	scene.createBox(vec3(0.0f, 0.5f, 2.0f), vec3(1.0f), shader, red, vec3(0.0f, 45.0f, 0.0f));
	scene.createLightSource(vec3(-8.0f, 10.0f, 3.0f), 1.0f);
}
// Function to render the scene, called each time the window needs to be redrawn
void render()
{
	// Check for key inputs
	keyOperations();
	// Use the shadow shader and render the scene from the lights point of view in updateShadowTexture
	shadowShader.useShader();
	updateShadowTexture();
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
	shader.setShadowTextureUniform(depthTextureID);
	shader.setTextureUniform(texName);
	shader.setUserPosUniform(user.getPosition());
	// Calculate depth view projection matrix and the shadow bias matrix and send it to the shader
	mat4 lightView = LookAt(scene.getLightPos(), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
	mat4 lightProjection = Ortho(-30.0f, 30.0f, -30.0f, 30.0f, -10.0f, 60.0);
	mat4 depthVP = lightProjection * lightView;
	mat4 biasMatrix = Translate(vec3(0.5f));
	biasMatrix *= Scale(vec3(0.5f));
	shader.setdepthVPUniform(depthVP);
	shader.setDepthBiasUniform(biasMatrix);
	// Draw the scene from the users point of view
	mat4 view = user.getViewMatrix();
	scene.drawObjects(shader, view);
	// Set the ambient and light uniforms
	shader.setLighPosUniform(scene.getLightPos());
	shader.setLightIntensityUniform(scene.getLightInt());
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

	// Tells glut to call the render function when finished render the scene.
	glutPostRedisplay();
}
// Build framebufferobject for depthTesting
void buildFrameBufferObject(int width, int height) {
	// Generate shadow texture and set the filtering, wraping and comparison parameters
	glGenTextures(1, &depthTextureID);
	glBindTexture(GL_TEXTURE_2D, depthTextureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
	// Generate frame buffer object to draw the depth value into
	glGenFramebuffers(1, &framebufferobject);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferobject);
	// Attaches the depth texture to the framebuffer
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureID, 0);
	// Checks for errors with the frame buffer object
	if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "Something wrong with FBO" << endl;
	// Binds the normal frame buffer for rendering the scene
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
// Updates the shadow Map
void updateShadowTexture(){
	// Disable culling to let the back faces contribute to shadows
	glDisable(GL_CULL_FACE);
	// Bind the framebufferobject assosiated with the shadow texture
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferobject);
	// Clear depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);
	// Set light view and projection matrix
	mat4 lightView = LookAt(scene.getLightPos(), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
	mat4 lightProjection = Ortho(-30.0f, 30.0f, -30.0f, 30.0f, -10.0f, 60.0);
	// OPTIONAL bind the texture if another texture is bound!
	// Set the projection uniform and set the viewport to fit the shadow texture
	shadowShader.setProjectionUniform(lightProjection);
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
	cout << "  L - Move light position to your position" << endl;
	cout << "  - and + - Adjust the intensity of the light source" << endl;
	cout << "  O and p - Adjust the speed of the camera" << endl;
	cout << "  X - Fly mode" << endl;
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
		scene.setLightPos(user.getPosition());
	}
	if (keyStates['+'] == true){
		scene.setLightInt(scene.getLightInt() + 0.1f);
	}
	if (keyStates['-'] == true){
		scene.setLightInt(scene.getLightInt() - 0.1f);
		if (scene.getLightInt() < 0.0f){
			scene.setLightInt(0.0f);
		}
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
}
// Sets the key states when a key is released
void keyReleased(unsigned char key, int x, int y){
	keyStates[key] = false;
}


