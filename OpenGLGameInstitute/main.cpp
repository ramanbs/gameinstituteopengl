#include <iostream>
#include <sstream>
#include "GL/glew.h" // Always before GLFW, creates function pointer to all the openGl function supported for our graphics card driver at runtime
#include "GLFW/glfw3.h"

const char* APP_TITLE = "OPENGL WIndow";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
const bool gFullscreen = false;

void glfw_onKey(GLFWwindow* window, int key, int scanCode, int action, int mode);
void showFPS(GLFWwindow* window);

int main() 
{
	if (!glfwInit())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}

	//TODO: GLFW window hint - lookup documentation 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//for MAC - glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* pWindow = NULL;

	if (gFullscreen)
	{
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);
		if (pVmode != NULL)
		{
			pWindow = glfwCreateWindow(pVmode->width, pVmode->height, APP_TITLE, pMonitor, NULL);
		}
	}
	else
	{
		pWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	}
	
	if (pWindow == NULL) 
	{
		std::cerr << "Failed to create a window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(pWindow); // cant draw to the window withou this

	glfwSetKeyCallback(pWindow, glfw_onKey);

	glewExperimental = GL_TRUE; //TODO: might not initizlaize properly with this - check later

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Main Loop
	while (!glfwWindowShouldClose(pWindow)) 
	{
		showFPS(pWindow);
		glfwPollEvents(); 

		glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // without this old color wouldnt be cleared

		glfwSwapBuffers(pWindow); // makes our application double buffered - front and back buffer
	}

	glfwTerminate();
	return 0;
}

void glfw_onKey(GLFWwindow* window, int key, int scanCode, int action, int mode) 
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
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
		//TODO : why do ostringstream instead of string ?

		glfwSetWindowTitle(window, outs.str().c_str());

		frameCount = 0;
	}

	frameCount++;
}