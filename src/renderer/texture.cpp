#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "texture.h"
#include "3rd/stb_image/stb_image.h"

Texture::Texture(const std::string &path)
{
	m_filePath = path;
	m_renderID = 0;
	m_imgBuffer = nullptr;
	m_width = 0;
	m_height = 0;
	m_BPP = 0;

	m_imgBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 4);
	stbi_set_flip_vertically_on_load(1);

	glGenTextures(1, &m_renderID);
	glBindTexture(GL_TEXTURE_2D, m_renderID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imgBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_imgBuffer)
	{
		stbi_image_free(m_imgBuffer);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_renderID);
}

void Texture::bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_renderID);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
