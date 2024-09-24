#pragma once

#include "glm/glm.hpp"

#include "renderer/vertex_array.h"
#include "renderer/texture.h"

class Shape
{
public:
	Shape();
	~Shape();

	// -- Setters -- //
	void setColor(float r, float g, float b, float a);
	void setTexture(const std::string &path);

	// -- Getters -- //
	const glm::mat4 getModelMatrix();
	const glm::vec3 getRotation();

	// -- Utility -- //
	void shift(glm::vec3 translation);
	void translate(glm::vec3 translation);
	void rotate(glm::vec3 rotation);
	void setRotation(glm::vec3 rotation);
	void scale(glm::vec3 scale);
	void resetModel();

	// -- Virtual -- //
	virtual void draw();

protected:
	VertexArray *m_pVA;
	int m_vertCount;
	Texture *m_pTexture;
	glm::vec3 m_rotation;
	glm::mat4 m_modelMat;
};