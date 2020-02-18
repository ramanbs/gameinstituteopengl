#version 330 core

layout (location = 0) in vec3 pos;  // layout of data coming into the shader, 0 - here is what we gave index to position attrib pointer"

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f); // w - coordinate is set to 1, as its a flat triangle ina normalized device space
};