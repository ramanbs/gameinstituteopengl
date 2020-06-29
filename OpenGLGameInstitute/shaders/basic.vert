#version 330 core

layout (location = 0) in vec3 pos;  // layout of data coming into the shader, 0 - here is what we gave index to position attrib pointer"
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f); // w - coordinate is set to 1, as its a flat triangle ina normalized device space
	TexCoord = texCoord;
};