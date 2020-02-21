#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "GL/glew.h"
#include <string>
using std::string;

class Texture2D
{
public:
	Texture2D();
	virtual ~Texture2D(); //TODO: Why and when do we need virtual destructor (was not explained)

	bool loadTexture(const string& fileName, bool generateMipMaps = true);
	void bind(GLuint texUnit = 0);

private:

	GLuint mTexture; // like other units, texture are held by opengl and reffered by id
};


#endif // TEXTURE2D_H	
