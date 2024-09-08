#pragma once

#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Texture
{
public:
	Texture(const std::string &path);
	~Texture();

	// -- Utility -- //
	void bind(unsigned int slot);
	void unbind();

private:
	std::string m_filePath;
	GLuint m_renderID;
	unsigned char *m_imgBuffer;
	int m_width;
	int m_height;
	int m_BPP;
};
