#pragma once

#include <string>

#include "helper/app_tools.h"
#include "color.h"

class Shader
{
public:
	Shader();
	Status generateShader(const std::string &vPath, const std::string &fPath);
	GLuint getProgram() const;
	void useShader();
	void deleteShader();
	void setFragmentColor(const Color &color);

private:
	Status compileShader(GLuint &shader, std::string &shaderSource, GLenum type);
	std::string parseShaderFile(const std::string &path);

	GLuint program;
};
