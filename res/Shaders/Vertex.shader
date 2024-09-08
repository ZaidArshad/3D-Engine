#version 400 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;

out vec4 out_color;
out vec2 out_texCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	out_color = color;
	out_texCoord = texCoord;
};