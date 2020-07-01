#include <iostream>
#include <sstream>
#include "GL/glew.h" // Always before GLFW, creates function pointer to all the openGl function supported for our graphics card driver at runtime
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Mesh.h"

const char* APP_TITLE = "OPENGL WIndow - Hello Shader!";

FPSCamera fpsCamera(glm::vec3(0.0f, 3.0f, 10.0f));
const double ZOOM_SENSITIVITY = -3.0f;
const float MOVE_SPEED = 5.0f;
const float MOUSE_SENSITIVITY = 0.1f;

float gCubeAngle = 0.0f;

//OrbitCamera orbitCamera;
//float gYaw = 0.0f;
//float gPitch = 0.0f;
//float gRadius = 10.0f;
//const float MOUSE_SENSITIVITY = 0.25f;

GLFWwindow* gWindow = NULL;

int gWindowWidth = 1024;
int gWindowHeight = 768;

bool gFullscreen = false;
bool gWireframe = false;


void glfw_onKey(GLFWwindow* window, int key, int scanCode, int action, int mode);
void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height);
void glfw_OnMouseMove(GLFWwindow* window, double posX, double posY);
void glfw_OnMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
void update(double elapsedTime);
void showFPS(GLFWwindow* window);
bool initOpenGL();

int main() 
{
	if (!initOpenGL())
	{
		std::cerr << "OpenGL initialization failed" << std::endl;
		return -1;
	}

	//// clockwise
	//GLfloat vertices[] = {
	//	// position			// tex coordinates
	//   -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, // top left
	//	0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right 
	//    0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
	//   -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left (0,0) for u,v coordinates
	//};
	// In quad it was clockwise and now in cube they are using counter clockwise, why ? - Because winding order does not matter if we dont specify - answer is given by author in udemy

	// GLfloat vertices[] = was earlier using this to specify quad or triangle vertices manually 
	// this is quad indices
	//GLuint indices[] = {
	//	0, 1, 2, // tri 0
	//	0, 2, 3 // tri 1
	//};

	// Creating index buffer - commenting out for cube implementation
	/*glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	ShaderProgram lightShader;
	lightShader.loadShaders("shaders/basic.vert", "shaders/basic.frag");
	
	ShaderProgram lightingShader;
	lightingShader.loadShaders("shaders/lighting.vert", "shaders/lighting.frag");

	//Model Positions

	// Model positions
	glm::vec3 modelPos[] = {
		glm::vec3(-3.5f, 0.0f, 0.0f),	// crate1
		glm::vec3(3.5f, 0.0f, 0.0f),	// crate2
		glm::vec3(0.0f, 0.0f, -2.0f),	// robot
		glm::vec3(0.0f, 0.0f, 0.0f),	// floor
		glm::vec3(0.0f, 0.0f, 2.0f),	// pin
		glm::vec3(-2.0f, 0.0f, 2.0f)	// bunny
	};

	// Model scale

	glm::vec3 modelScale[] = {
		glm::vec3(1.0f, 1.0f, 1.0f),  //crate
		glm::vec3(1.0f, 1.0f, 1.0f),  //wood crate
		glm::vec3(1.0f, 1.0f, 1.0f),  //robot
		glm::vec3(10.0f, 1.0f, 10.0f),  //floor
		glm::vec3(0.1f, 0.1f, 0.1f),  //pin
		glm::vec3(0.7f, 0.7f, 0.7f)  //bunny
	};

	// Load meshes and textures
	const int numModels = 6;
	Mesh mesh[numModels];
	Texture2D texture[numModels];

	mesh[0].loadOBJ("models/crate.obj");
	mesh[1].loadOBJ("models/woodcrate.obj");
	mesh[2].loadOBJ("models/robot.obj");
	mesh[3].loadOBJ("models/floor.obj");
	mesh[4].loadOBJ("models/bowling_pin.obj");
	mesh[5].loadOBJ("models/bunny.obj");

	texture[0].loadTexture("textures/crate.jpg", true);
	texture[1].loadTexture("textures/woodcrate_diffuse.jpg", true);
	texture[2].loadTexture("textures/robot_diffuse.jpg", true);
	texture[3].loadTexture("textures/tile_floor.jpg", true);
	texture[4].loadTexture("textures/AMF.tga", true);
	texture[5].loadTexture("textures/bunny_diffuse.jpg", true);

	Mesh lightMesh;
	lightMesh.loadOBJ("models/light.obj");

	double lastTime = glfwGetTime();
	float angle = 0.0f;

	// Main Loop
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		glfwPollEvents(); 
		update(deltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // without this old color wouldnt be cleared

		//create model, view and projection matrix
		glm::mat4 model(1.0f), view(1.0f), projection(1.0f);

		//orbitCamera.setLookAt(cubePos); // need not be done every frame since we are not changing the target
		//orbitCamera.rotate(gYaw, gPitch);
		//orbitCamera.setRadius(gRadius);

		// glm::translate - translate the vertices of the model by the given positon
		// glm::rotate - rotate the given model w.r.t given axis with angle in radians 
		// glm::vec3(0.0f, 1.0f, 0.0f) - tells rotate function which axis to rotate against
	

		view = fpsCamera.getViewMatrix();
		//TODO : bookmarked perspective matrix derivation videos in virtual camera 1 episode
		// 45.0f - field of view angle, 1024/768 - aspect ratio, 0.1 - near plane, 100 - far plane
		//projection = glm::perspective(glm::radians(45.0f), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);

		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);

		glm::vec3 viewPos;
		viewPos.x = fpsCamera.getPosition().x;
		viewPos.y = fpsCamera.getPosition().y;
		viewPos.z = fpsCamera.getPosition().z;

		// the light 
		glm::vec3 lightPos(0.0f, 1.0f, 10.0f);
		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

		// move the light
		angle += (float)deltaTime * 50.0f;
		lightPos.x = 8.0f * sinf(glm::radians(angle));
		
		lightingShader.use();// should be used before draw arrays

		//set the locations of sampler in case of multiple texturing, have to be set after using the program. IT has to be active.
		//glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "myTexture1"), 0);
		////TODO - compare with shader program implementation, and & remove accessor to program
		//glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "myTexture2"), 1);

		
		lightingShader.setUniform("view", view);
		lightingShader.setUniform("projection", projection);
		lightingShader.setUniform("viewPos", viewPos);
		lightingShader.setUniform("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setUniform("light.diffuse", lightColor);
		lightingShader.setUniform("light.specular", glm::vec3(1.0f, 0.8f, 0.0f));
		lightingShader.setUniform("light.position", lightPos);
		

		for (int i = 0; i < numModels; i++) 
		{
			model = glm::translate(glm::mat4(1.0f), modelPos[i]) * glm::scale(glm::mat4(1.0f), modelScale[i]);
			
			lightingShader.setUniform("model", model);

			lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
			lightingShader.setUniformSampler("material.diffuseMap", 0);
			lightingShader.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			lightingShader.setUniform("material.shininess", 32);

			texture[i].bind(0);
			mesh[i].draw();
			texture[i].unbind(0);
		}

		//render the light
		model = glm::translate(glm::mat4(), lightPos);
		lightShader.use();
		lightShader.setUniform("lightColor", lightColor);
		lightShader.setUniform("model", model);
		lightShader.setUniform("view", view);
		lightShader.setUniform("projection", projection);

		lightMesh.draw();

		glfwSwapBuffers(gWindow); // makes our application double buffered - front and back buffer
		lastTime = currentTime;
	}

	glfwTerminate();
	return 0;
}

bool initOpenGL()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	//TODO: GLFW window hint - lookup documentation 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//TODO: for MAC - glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	if (gFullscreen)
	{
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);
		if (pVmode != NULL)
		{
			gWindow = glfwCreateWindow(pVmode->width, pVmode->height, APP_TITLE, pMonitor, NULL);
		}
	}
	else
	{
		gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	}

	if (gWindow == NULL)
	{
		std::cerr << "Failed to create a window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(gWindow); // cant draw to the window withou this

	glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetCursorPosCallback(gWindow, glfw_OnMouseMove);
	glfwSetScrollCallback(gWindow, glfw_OnMouseScroll);

	//Hides and grabs cursor, unlimited movement
	glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // disables cursor
	glfwSetCursorPos(gWindow, gWindowWidth / 2.0f, gWindowHeight / 2.0f);

	glewExperimental = GL_TRUE; //TODO: might not initialize properly without this - check later

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
	// resize viewport whenever window is resized
	glViewport(0, 0, gWindowWidth, gWindowHeight);
	// Enable this so that the pixels nearer to the camera are drawn last on top of compared to pixel further away
	glEnable(GL_DEPTH_TEST);

	return true;
}

void glfw_onKey(GLFWwindow* window, int key, int scanCode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		gWireframe = !gWireframe;

		//GL_FRONT_AND_BACK - winding order of vertices
		if (gWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height) 
{
	gWindowWidth = width;
	gWindowHeight = height;

	// Converts from OpenGl 2D coordinates to Screen Coordinates
	// OPenGL Coordinates x varies -1 to 1 and y varies -1 to 1
	// Screen Coordinates starts from lower left corner - (0,0) to (width, height)
	glViewport(0, 0, gWindowWidth, gWindowHeight);
}

void glfw_OnMouseMove(GLFWwindow* window, double posX, double posY) 
{
	//static glm::vec2 lastMousePos = glm::vec2(0, 0);

	//// Update angles based on Left Mouse Button input to orbit around the cube
	//if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_LEFT) == 1) 
	//{
	//	// each pixel represents a quarter of a degree rotation (this is our mouse sensitivity)
	//	// MOUSE_SENSITIVITY - too fast a change otherwise
	//	// Subracting the difference between current and last known mouse position - Why ? Will know if i run it :D 
	//	gYaw -= ((float)posX - lastMousePos.x) * MOUSE_SENSITIVITY;
	//	// Adding the difference between current and last known mouse position - Why ? 
	//	gPitch += ((float)posY - lastMousePos.y) * MOUSE_SENSITIVITY;
	//}

	//// Change orbit camera radius with the Right Mouse Button
	//if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_RIGHT) == 1) 
	//{
	//	float dx = 0.01f * ((float)posX - lastMousePos.x);
	//	float dy = 0.01f * ((float)posY - lastMousePos.y);
	//	gRadius += dx - dy;
	//}

	//lastMousePos.x = (float)posX;
	//lastMousePos.y = (float)posY;

}

void glfw_OnMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
{
	double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

	fov = glm::clamp(fov, 1.0, 120.0);

	fpsCamera.setFOV((float)fov);
}

// Update stuff every frame
void update(double elapsedTime) 
{
	//Camera orientation 
	double mouseX, mouseY;

	// Get the current mouse cursor position delta
	glfwGetCursorPos(gWindow, &mouseX, &mouseY);

	// Rotate the camera the difference in mouse distance from the center screen. Multiply this delat by a speed scaler 
	fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY);

	// reset the mouse cursor to center of screen every frame since above calculation fo pitch and yaw is dependent on it
	glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

	// Camera FPS movement

	// Forward / Backward
	if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
	else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());

	// Strafe left/ right
	if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());
	else if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());

	// Up / Down
	if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
	else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());

}

void showFPS(GLFWwindow* window)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	// returns number of seconds since GLFW started, uses h/w on motherbaord called high resolution timer (can be used for accuracy upto microseconds), Windows get time resolution is in milliseconds
	double currentSeconds = glfwGetTime(); 

	elapsedSeconds = currentSeconds - previousSeconds;

	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		std::ostringstream outs;
		outs.precision(3);

		outs << std::fixed
			<< APP_TITLE << "  "
			<< "FPS : " << fps << "  "
			<< "Frame Time : " << msPerFrame << " (ms)";
		//TODO : why use ostringstream instead of string ?

		glfwSetWindowTitle(window, outs.str().c_str());

		frameCount = 0;
	}

	frameCount++;
}