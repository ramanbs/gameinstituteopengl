#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>



Mesh::Mesh()
	:mLoaded(false)
{
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
}

bool Mesh::loadOBJ(const std::string& filename) 
{
	std::vector<unsigned int> vertexIndices, uvIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;

	if (filename.find(".obj") != std::string::npos) 
	{
		std::ifstream fin(filename, std::ios::in);
		
		if (!fin)
		{
			std::cerr << "Cannot open " << filename << std::endl;
			return false;
		}

		std::cout << "Loading OBJ file " << filename << "..." << std::endl;

		std::string lineBuffer;
		while (std::getline(fin, lineBuffer)) 
		{
			if (lineBuffer.substr(0, 2) == "v ")
			{
				std::istringstream v(lineBuffer.substr(2));
				glm::vec3 vertex;
				//std::get() does not ignore whitespace whereas extraction operator does
				v >> vertex.x;
				v >> vertex.y;
				v >> vertex.z;

				tempVertices.push_back(vertex);
			}
			else if (lineBuffer.substr(0, 2) == "vt")
			{
				std::istringstream vt(lineBuffer.substr(3));
				glm::vec2 uv;
				
				vt >> uv.s; 
				vt >> uv.t;
				
				tempUVs.push_back(uv);
			}
			else if (lineBuffer.substr(0, 2) == "f ")
			{
				int p1, p2, p3;
				int t1, t2, t3;
				int n1, n2, n3;

				const char* face = lineBuffer.c_str();
				//sscanf_s is a Microsoft compiler version of sscanf that reads formatted input from a string. This variant is helpful as it warns the users if the value assigned overflows when assigned to a variable, returns number of data it found and read
				int match = sscanf_s(face, "f %i/%i/%i %i/%i/%i %i/%i/%i",
					&p1, &t1, &n1,
					&p2, &t2, &n2,
					&p3, &t3, &n3);

				if (match != 9)
					std::cout << "Failed to parse OBJ file using our very simple OBJ loader" << std::endl;
				//We are igonring normals for now 
				vertexIndices.push_back(p1);
				vertexIndices.push_back(p2);
				vertexIndices.push_back(p3);

				uvIndices.push_back(t1);
				uvIndices.push_back(t2);
				uvIndices.push_back(t3);

			}
		}
		//Close the file
		fin.close();

		// For each vertex of the triangle
		for (unsigned int i = 0; i < vertexIndices.size(); i++) 
		{
			// Get the attributes using the indices
			glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
			glm::vec2 uv = tempUVs[uvIndices[i] - 1];

			Vertex meshVertex;
			meshVertex.position = vertex;
			meshVertex.texCoords = uv;

			mVertices.push_back(meshVertex);
		}

		//Create and initialize buffers
		initBuffers();

		return (mLoaded = true);
	}

	return false;
}

void Mesh::draw() 
{
	if (!mLoaded) return;

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
	glBindVertexArray(0);
}

void Mesh::initBuffers() 
{
	glGenBuffers(1, &mVBO); // creates a buffer in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, mVBO); //makes the vbo buffer as current buffer, only one buffer at a time

	//GL_STATIC_DRAW - create once, setup once use it a lot, GL_DYNAMIC_DRAW - create once, change a lot and use it a lot, GL_STREAM_DRAW - create once, setup once and use once
	//mVertices.size() - gives the count of elements in that data structure
	//TODO &mVertices[0] - why first element
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

	/*For core openGL we need to create VertexArrayObject - holds info for our buffer so that when we draw we need not refer buffer object but VAO instead. MOst meshes object holds a collections of one or more VBO which holds vertext point, vertex normal, texture coords. Older OpenGL needs to bind each data buffer and define memory layout every time you need to draw. VAO collects various info like pointer to VBO and only VAO can be binded and used before we draw something instead of providing everything every time we need to draw something (old way of doing it).  SImplifies and speeds rendering and is mandatory
	*/
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO); // make it the current vao

	// Create vertex attribute (always after we have bound the vao as these attrib works on the current vao) - for shaders to interpret the vertex data (how the vertex data is laid out in the buffer)
	// 0 (index) - attribute identifier, 3 - number of components that constitute this attribute (x, y, z), GL_FLOAT - type of data, GL_FALSE - need to normalize the data in screen space,  0 - stride (continuous bytes of data that constitute the given attribute incase the vertext data is interleaved with some other data in the buffer like e.g. color), NULL - offset from which the actual data starts for this attrib in the buffer. 
	//position attrib pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, NULL); // holds info passed to this function in the VAO, so we have to generate VAO first.
	glEnableVertexAttribArray(0); // 0 - attrib index, by default opengl disables vertex attrib array.

	//texture attrib
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)((sizeof(GLfloat) * 3)));
	glEnableVertexAttribArray(1);

	// Unbind vertex array
	glBindVertexArray(0);

}
