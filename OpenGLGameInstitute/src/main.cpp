#include <iostream>
#include <sstream>
#include "GL/glew.h" // Always before GLFW, creates function pointer to all the openGl function supported for our graphics card driver at runtime
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ShaderProgram.h"
#include "Texture2D.h"

const char* APP_TITLE = "OPENGL WIndow - Hello Shader!";
const std::string texture1FileName = "textures/wooden_crate.jpg";
const std::string texture2FileName = "textures/crate.jpg";

GLFWwindow* gWindow = NULL;

int gWindowWidth = 1024;
int gWindowHeight = 768;

bool gFullscreen = false;
bool gWireframe = false;


void glfw_onKey(GLFWwindow* window, int key, int scanCode, int action, int mode);
void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height);
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
	//TODO : In quad it was clockwise and now in cube they are using counter clockwise, why ?

	GLfloat vertices[] = {
		// position		 // tex coords

	   // front face
	   -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
	   -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
	   -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		// back face
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		 // left face
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		 -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		 // right face
		  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		  // top face
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
		  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,

		  // bottom face
		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
	};

	glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, -5.0f);

	// this is quad indices
	//GLuint indices[] = {
	//	0, 1, 2, // tri 0
	//	0, 2, 3 // tri 1
	//};

	//Vertex Buffer Object - generic place in GPU memory to hold vertices and minimize the traffic b/w CPU and GPU
	//also this means we are running in retained mode rather than immediate mode
	GLuint vbo, vao;
	glGenBuffers(1, &vbo); // creates a buffer in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //makes the vbo buffer as current buffer, only one buffer at a time
	
	//GL_STATIC_DRAW - create once, setup once use it a lot, GL_DYNAMIC_DRAW - create once, change a lot and use it a lot, GL_STREAM_DRAW - create once, setup once and use once
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*For core openGL we need to create VertexArrayObject - holds info for our buffer so that when we draw we need not refer buffer object but VAO instead. MOst meshes object holds a collections of one or more VBO which holds vertext point, vertex normal, texture coords. Older OpenGL needs to bind each data buffer and define memory layout every time you need to draw. VAO collects various info like pointer to VBO and only VAO can be binded and used before we draw something instead of providing everything every time we need to draw something (old way of doing it).  SImplifies and speeds rendering and is mandatory
	*/
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao); // make it the current vao

	// Create vertex attribute (always after we have bound the vao as these attrib works on the current vao) - for shaders to interpret the vertex data (how the vertex data is laid out in the buffer)
	// 0 (index) - attribute identifier, 3 - number of components that constitute this attribute (x, y, z), GL_FLOAT - type of data, GL_FALSE - need to normalize the data in screen space,  0 - stride (continuous bytes of data that constitute the given attribute incase the vertext data is interleaved with some other data in the buffer like e.g. color), NULL - offset from which the actual data starts for this attrib in the buffer. 
	//position attrib pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, NULL); // holds info passed to this function in the VAO, so we have to generate VAO first.
	glEnableVertexAttribArray(0); // 0 - attrib index, by default opengl disables vertex attrib array.

	//texture attrib
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)((sizeof(GLfloat) * 3)));
	glEnableVertexAttribArray(1);
	
	// Creating index buffer - commenting out for cube implementation
	/*glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("shaders/basic.vert", "shaders/basic.frag");

	Texture2D texture1;
	texture1.loadTexture(texture1FileName, true);

	//Texture2D texture2;
	//texture2.loadTexture(texture2FileName, true);

	float cubeAngle = 0.0f;
	double lastTime = glfwGetTime();

	// Main Loop
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		glfwPollEvents(); 

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // without this old color wouldnt be cleared

		texture1.bind(0); //TODO 0 - Texture unit that is used in shader by sampler, gives a location of the texture units. Shader can refrence multiple textures using this texture unit. You can have only one texture active (bound) at a time but can reference multiple texture units. Need to know more about this for more clarity, multiple tex units vs multiple texture.
		//texture2.bind(1);

		cubeAngle += (float)(deltaTime * 50.0f);
		if (cubeAngle >= 360.0)
			cubeAngle = 0.0f;

		//create model, view and projection matrix
		glm::mat4 model(1.0f), view(1.0f), projection(1.0f);

		// glm::translate - translate the vertices of the model by the given positon
		// glm::rotate - rotate the given model w.r.t given axis with angle in radians 
		model = glm::translate(model, cubePos) * glm::rotate(model, glm::radians(cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec3 camPos(0.0f, 0.0f, 0.0f);
		glm::vec3 targetPos(0.0f, 0.0f, -1.0f);
		glm::vec3 up(0.0f, 1.0f, 0.0f);

		// camPos or eyePos = glm::vec3 position the virtual camera is located in the world
		// targetPos = the aim point where the camera should be looking
		// up = up direction unit vector of the camera
		// lookat generates the view matrix where in  everything translates and rotates in opp direction of the camera
		view = glm::lookAt(camPos, targetPos, up);
		//TODO : bookmarked perspective matrix derivation videos in virtual camera episode
		// 45.0f - field of view angle, 1024/768 - aspect ratio, 0.1 - near plane, 100 - far plane
		projection = glm::perspective(glm::radians(45.0f), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);
		
		shaderProgram.use();// should be used before draw arrays

		//set the locations of sampler in case of multiple texturing, have to be set after using the program. IT has to be active.
		//glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "myTexture1"), 0);
		////TODO - compare with shader program implementation, and & remove accessor to program
		//glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "myTexture2"), 1);

		shaderProgram.setUniform("model", model);
		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);
		

		glBindVertexArray(vao); // bind to make vao active
		// GL_TRIANGLES - what kind of primitive are we drawing, 0 - the first component in vao to be drawn, 3 - number of vertices, in this case x,y,z 
		//glDrawArrays(GL_TRIANGLES, 0, 6);  - this now we dont use as we are using indexed buffer
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // primitive, number of vertices, type of indices buffer, offset from which to read the indices
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0); // unbind the active vao

		glfwSwapBuffers(gWindow); // makes our application double buffered - front and back buffer
		lastTime = currentTime;
	}

	//Cleanup
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	//glDeleteBuffers(1, &ibo);

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

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
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