#version 330 core

layout(location = 0) in vec3 aPos; 
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model; // model data
uniform mat4 view;  // camera data
uniform mat4 projection; // projection data 

void main()
{
	TexCoord = aTexCoord;
	gl_Position = projection * view * model * vec4(aPos, 1.0); 
};