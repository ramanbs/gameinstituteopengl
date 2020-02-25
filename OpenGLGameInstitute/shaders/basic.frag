#version 330 core

in vec2 TexCoord;
out vec4 frag_color;
// in case of a single texture, the default location was 0, hence dont need to provide for the sampler uniform, but in multitexturing we need to assign the location
uniform sampler2D myTexture1; //TODO built in sampler by GLSL. Have to find out more types of sampler available. 
uniform sampler2D myTexture2;

void main()
{
	frag_color = mix(texture(myTexture1, TexCoord), texture(myTexture2, TexCoord), 0.8); // mix - does a linear interpolation between the colors of two texture witha certain blend factor(0.2).
};
