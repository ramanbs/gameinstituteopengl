// TODO : See what are 1D and 3D textures in detail, we are implementing 2D texture currently in this as evident from the name, duh !
#include "Texture2D.h"
#include "stb_image/stb_image.h"
#include <iostream>

Texture2D::Texture2D() 
	:mTexture(0)
{
}

Texture2D::~Texture2D()
{
}

//Opengl by default do no tprovide the functionality of loading image into your application from your hard drive
bool Texture2D::loadTexture(const string& fileName, bool generateMipMaps = true) 
{
	int width, height, components; // components - whether its rgb (2-components) or rgba 

	//stbi_load() reads image from disk, allocates the memory for array of bytes and return pointer to the array
	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &components, STBI_rgb_alpha); //TODO unsigned char as the range is higher, in signed char -128 to 127, whereas unsigned is 0 to 255., why to use unsigned ? still not sure 

	if (imageData == NULL) 
	{
		std::cerr << "Error loading texture file '" << fileName << "'" << std::endl;
		return false;
	}

	// Now create the txture in opengl, similar to creating a buffer
	
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	return true;
}

void Texture2D::bind(GLuint texUnit = 0) 
{

}
