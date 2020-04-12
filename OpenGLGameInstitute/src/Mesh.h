#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>

#include "GL/glew.h"
#include "glm/glm.hpp"


struct Vertex 
{
	glm::vec3 position;
	glm::vec2 texCoords;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool loadOBJ(const std::string& filename);
	void draw();

private:
	void initBuffers();

	bool mLoaded;
	std::vector<Vertex> mVertices;
	//Vertex Buffer Object - generic place in GPU memory to hold vertices and minimize the traffic b/w CPU and GPU
//also this means we are running in retained mode rather than immediate mode
	GLuint mVBO, mVAO;
};

#endif //ifndef MESH_H 

