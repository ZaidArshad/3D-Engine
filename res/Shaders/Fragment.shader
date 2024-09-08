#version 400 core

in vec4 out_color;
in vec2 out_texCoord;

out vec4 color;

uniform sampler2D u_Texture;

void main()
{
	if (out_color.w != 0)
	{
		color = out_color;
	}
	else if (out_texCoord != 0)
	{
		vec4 texColor = texture(u_Texture, out_texCoord);
		color = texColor;
	}
	else
	{
		color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
};