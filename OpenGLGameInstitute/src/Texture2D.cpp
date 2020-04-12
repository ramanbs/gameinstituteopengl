// TODO : See what are 1D and 3D textures in detail, we are implementing 2D texture currently in this as evident from the name, duh !
#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION // required by the stb library
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
bool Texture2D::loadTexture(const string& fileName, bool generateMipMaps) 
{
	int width, height, components; // components - whether its rgb (2-components) or rgba 

	//stbi_load() reads image from disk, allocates the memory for array of bytes and return pointer to the array
	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &components, STBI_rgb_alpha); //TODO unsigned char as the range is higher, in signed char -128 to 127, whereas unsigned is 0 to 255., why to use unsigned ? still not sure 

	if (imageData == NULL) 
	{
		std::cerr << "Error loading texture file '" << fileName << "'" << std::endl;
		return false;
	}

	// invert image
	//TODO go through this one more time with more info, basic idea i understood, but have to go through it to be more sure.
	int widthInBytes = width * 4;
	unsigned char * top = NULL;
	unsigned char * bottom = NULL;
	unsigned char temp = 0;
	int halfHeight = height / 2;

	for (int row = 0; row < halfHeight; row++) 
	{
		top = imageData + row * widthInBytes;
		bottom = imageData + (height - row - 1) * widthInBytes;
		for (int col = 0; col < widthInBytes; col++) 
		{
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}

	// Now create the txture in opengl, similar to creating a buffer
	
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	/* there are various overloads to this as glTexParameteriv and glTexParameterfv etc 
	// GL_TEXTURE_2D - texture type GL_TEXTURE_WRAP_S - wrapping around the x-axis ( S - when you follow S&T conevntions otherwise U in U&V convention)
	   GL_REPEAT - wrapping mode */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// Texture minification filtering set 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// Texture magnificaton filtering set 

	// GL_TEXTURE_2D - texture type, 0 - Level of detail, GL_RGBA - format of the image to be loaded (given while loading image from hard disk - STBI_rgb_alpha), width, height, 0 - width of the border, GL_RGBA - format of the resulting texture in opengl, GL_UNSIGNED_BYTE - type of image data being passed
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	if (generateMipMaps)
		glGenerateMipmap(GL_TEXTURE_2D);

	// clean up the loaded image now as the texture is created already and no longer requires the image data
	stbi_image_free(imageData);
	// unbind the texture , so that it is not active after function call causing any unwanted action on this bound texture instead of intended.
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

//TODO 0 - Texture unit that is used in shader by sampler, gives a location of the texture units. Shader can refrence multiple textures using this texture unit. You can have only one texture active (bound) at a time but can reference multiple texture units. Need to know more about this for more clarity, multiple tex units vs multiple texture.
void Texture2D::bind(GLuint texUnit) 
{
	glActiveTexture(GL_TEXTURE0 + texUnit); // activate the texture unit 0, so that when we bind the texture next, it gets attached to this texture unit. We can have upto 16 texture units at the sam time available for shader to be blended.//TODO : api to query how many tex units available fo rour graphics card 
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

void Texture2D::unbind(GLuint texUnit)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}
