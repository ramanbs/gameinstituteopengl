#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>

ShaderProgram::ShaderProgram() 
	:mHandle(0)
{
}

ShaderProgram::~ShaderProgram() 
{
	glDeleteProgram(mHandle);
}

bool ShaderProgram::loadShaders(const char* vsFilename, const char* fsFilename)
{
	string vsString = fileToString(vsFilename);
	string fsString = fileToString(fsFilename);

	const GLchar* vsSourcePtr = vsString.c_str();
	const GLchar* fsSourcePtr = fsString.c_str();

	// Create vertex shader and fragment shader

	GLuint vs = glCreateShader(GL_VERTEX_SHADER); // returns id of the created shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vsSourcePtr, NULL); // link the shader object to the shader string, 1 - no of shader strings, NULL - length of the shader string
	glCompileShader(vs);

	GLint result; // stores 
	GLchar infoLog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &result); //GL_COMPILE_STATUS - pname ( more available ) , &result  - stores the result of compile status in this variable

	if (!result)
	{
		glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog); // why infoLog and not &infoLog, NULL - returns the length of the info log returned
		std::cout << "Error! Vertex Shader compilation failed" << infoLog << std::endl;
	}

	
	glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Fragment Shader compilation failed" << infoLog << std::endl;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Shader Program Linker failed" << infoLog << std::endl;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
	return true;
}

void ShaderProgram::use()
{
	if (mHandle > 0)
		glUseProgram(mHandle);
}

string ShaderProgram::fileToString(const string& fileName) 
{
	std::stringstream ss;
	std::ifstream file;

	try 
	{
		//TODO: readup files in http://www.cplusplus.com/doc/tutorial/files/
		file.open(filename, std::ios::in);

		if (!file.fail()) 
		{
			ss << file.rdbuf();
		}

		file.close();
	}
	catch (std::exception ex) 
	{
		std::cerr << "Error reading shader filename!" << std::endl;
	}

	return ss.str();

}

void ShaderProgram::checkCompilerErrors(GLuint shader, ShaderType type) 
{

}