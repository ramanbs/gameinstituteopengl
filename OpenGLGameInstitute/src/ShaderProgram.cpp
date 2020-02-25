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
	glShaderSource(fs, 1, &fsSourcePtr, NULL);

	glCompileShader(vs);

	checkCompilerErrors(vs, VERTEX);

	glCompileShader(fs);

	checkCompilerErrors(fs, FRAGMENT);

	mHandle = glCreateProgram();
	glAttachShader(mHandle, vs);
	glAttachShader(mHandle, fs);
	glLinkProgram(mHandle);

	checkCompilerErrors(mHandle, PROGRAM); // not needed to send mHandle, have to find a better way

	glDeleteShader(vs);
	glDeleteShader(fs);

	mUniformLocations.clear();
	
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
		file.open(fileName, std::ios::in);

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
	int status = 0;

	if (type == PROGRAM)
	{
		glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
		
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);

			string errorLog(length, ' ');
			glGetProgramInfoLog(mHandle, length, NULL, &errorLog[0]);
			std::cerr << "Error! Program failed to link" << errorLog << std::endl;
		}
	}
	else // VERTEX or FRAGMENT 
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		
		if (status == GL_FALSE) 
		{
			GLint length = 0;
			glGetShaderiv(mHandle, GL_INFO_LOG_LENGTH, &length);

			string errorLog(length, ' ');
			//TODO: check NULL as *length vs &length implementation
			glGetShaderInfoLog(shader, length, NULL, &errorLog[0]);
			std::cerr << "Error! Shader Compilation failed." << errorLog << std::endl;
		}
	}

}

GLuint ShaderProgram::getProgram() const 
{
	return mHandle;
}

GLint ShaderProgram::getUniformLocation(const GLchar* name)
{
	std::map<string, GLint>::iterator it = mUniformLocations.find(name);

	if (it == mUniformLocations.end())
	{
		mUniformLocations[name] = glGetUniformLocation(mHandle, name);
	}

	return mUniformLocations[name];
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& v) 
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, v.x, v.y); // sets 2 floating point elements basically a floating vec2
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v)
{
	GLint loc = getUniformLocation(name);
	glUniform3f(loc, v.x, v.y, v.z); // opengl follows rules of 'c' language and hence no overloads for glUniform available
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v) 
{
	GLint loc = getUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}