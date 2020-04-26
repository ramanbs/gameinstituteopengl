#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>


std::vector<std::string> split(std::string s, std::string t) 
{
	std::vector<std::string> res;
	while (1)
	{
		int pos = s.find(t);
		
		if (pos == -1) 
		{
			res.push_back(s);
			break;
		}
		res.push_back(s.substr(0, pos));
		s = s.substr(pos + 1, s.size() - pos - 1);
	}

	return res;
}

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
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;

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
			std::stringstream ss(lineBuffer);
			std::string cmd;

			ss >> cmd;

			if (cmd == "v")
			{
				
				glm::vec3 vertex;
				//std::get() does not ignore whitespace whereas extraction operator does
				int dim = 0;
				while (dim < 3 && ss >> vertex[dim])
					dim++;

				tempVertices.push_back(vertex);
			}
			else if (cmd == "vt")
			{
				glm::vec2 uv;
				
				int dim = 0;
				while (dim < 2 && ss >> uv[dim])
					dim++;
				
				tempUVs.push_back(uv);
			}
			else if (cmd == "vn")
			{
				glm::vec3 normal;

				int dim = 0;
				while (dim < 3 && ss >> normal[dim])
					dim++;

				normal = glm::normalize(normal);

				tempNormals.push_back(normal);
			}
			else if (cmd == "f")
			{
				std::string faceData;
				int vertexIndex, uvIndex, normalIndex; // v/vt/vn 

				while (ss >> faceData)
				{
					std::vector<std::string> data = split(faceData, "/");
					
					// vertex index
					if (data[0].size() > 0) 
					{
						sscanf_s(data[0].c_str(), "%d", &vertexIndex);
						vertexIndices.push_back(vertexIndex);
					}

					// if the face vertex has a texture coordinate index 
					if (data.size() >= 1) 
					{
						if (data[1].size() > 0) 
						{
							sscanf_s(data[1].c_str(), "%d", &uvIndex);
							uvIndices.push_back(uvIndex);
						}
					}

					// if the face vertex has a normal index 
					if (data.size() >= 2) 
					{
						if (data[2].size() > 0)
						{
							sscanf_s(data[2].c_str(), "%d", &normalIndex);
							normalIndices.push_back(normalIndex);
						}
					}
				}

			}
		}
		//Close the file
		fin.close();

		// For each vertex of the triangle
		for (unsigned int i = 0; i < vertexIndices.size(); i++) 
		{
			Vertex meshVertex;

			if (tempVertices.size() > 0)
			{
				glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
				meshVertex.position = vertex;
			}
			
			if (tempNormals.size() > 0)
			{
				glm::vec3 normal = tempNormals[normalIndices[i] - 1];
				meshVertex.normal = normal;
			}
			
			if (tempUVs.size() > 0)
			{
				glm::vec2 uv = tempUVs[uvIndices[i] - 1];
				meshVertex.texCoords = uv;
			}

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
	// GL_TRIANGLES - what kind of primitive are we drawing, 0 - the first component in vao to be drawn, 3 - number of vertices, in this case x,y,z 
		//glDrawArrays(GL_TRIANGLES, 0, 6);  - this now we dont use as we are using indexed buffer
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // primitive, number of vertices, type of indices buffer, offset from which to read the indice
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL); // holds info passed to this function in the VAO, so we have to generate VAO first.
	glEnableVertexAttribArray(0); // 0 - attrib index, by default opengl disables vertex attrib array.

	// Normals Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)((sizeof(GLfloat) * 3)));
	glEnableVertexAttribArray(1);
	
	//texture attrib
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)((sizeof(GLfloat) * 6)));
	glEnableVertexAttribArray(2);

	// Unbind vertex array
	glBindVertexArray(0);

}
