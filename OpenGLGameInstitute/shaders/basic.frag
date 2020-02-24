#version 330 core

in vec2 TexCoord;
out vec4 frag_color;

uniform sampler2D myTexture; //TODO built in sampler by GLSL. Have to find out more types of sampler available. 

void main()
{
	frag_color = texture(myTexture, TexCoord);
};
