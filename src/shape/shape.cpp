
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include "shape.h"
#include "shader/color.h"
#include "helper/app_tools.h"

Shape::Shape()
{
	m_pVA = new VertexArray();
	m_vertCount = 0;
	m_pTexture = nullptr;
	m_rotation = glm::vec3(0.0f);
	m_modelMat = glm::mat4(1.0f);
}

Shape::~Shape()
{
	delete m_pVA;
	delete m_pTexture;
}

// -- Setters -- //
void Shape::setColor(float r, float g, float b, float a)
{
	std::vector<float> colors;
	for (int i = 0; i < m_vertCount; i++)
	{
		colors.insert(colors.end(), {r, g, b, a});
	}
	m_pVA->updateBuffer(1, colors, GL_STATIC_DRAW);
}

void Shape::setTexture(const std::string &path)
{
	std::vector<float> colors;
	for (int i = 0; i < m_vertCount; i++)
	{
		colors.insert(colors.end(), COLOR_TEXTURE.begin(), COLOR_TEXTURE.end());
	}
	m_pVA->updateBuffer(1, colors, GL_STATIC_DRAW);
	m_pTexture = new Texture(path);
}

// -- Getters -- //
const glm::mat4 Shape::getModelMatrix()
{
	return m_modelMat;
}

const glm::vec3 Shape::getRotation()
{
	return m_rotation;
}

void Shape::shift(glm::vec3 translation)
{
	glm::vec3 rotation = m_rotation;
	setRotation(glm::vec3(0.0f));
	translate(translation);
	setRotation(rotation);
}

//  -- Utility -- //
void Shape::translate(glm::vec3 translation)
{
	m_modelMat = glm::translate(m_modelMat, translation);
}

void Shape::rotate(glm::vec3 rotation)
{
	for (int i = 0; i < 3; i++)
	{
		glm::vec3 axis = glm::mat4(1.0f)[i];
		m_modelMat = glm::rotate(m_modelMat, rotation[i], axis);
	}
	m_rotation += rotation;
}

void Shape::setRotation(glm::vec3 rotation)
{
	rotate(-m_rotation);
	rotate(rotation);
}

void Shape::scale(glm::vec3 scale)
{
	m_modelMat = glm::scale(m_modelMat, scale);
}

void Shape::resetModel()
{
	m_modelMat = glm::mat4(1.0f);
}

void Shape::draw()
{
	if (m_pTexture)
	{
		m_pTexture->bind(0);
		m_pVA->draw();
		m_pTexture->unbind();
	}
	else
	{
		m_pVA->draw();
	}
}

std::vector<glm::vec3> Shape::getHitboxCoords()
{
	std::vector<glm::vec3> hitboxCoords;
	const glm::vec3 origin = glm::vec3(m_modelMat[3]);
	const auto positionVL = m_pVA->getVertexLayouts()[0];

	for (int i = 0; i < positionVL.vertices.size(); i += 3)
	{
		glm::vec3 coordinate(positionVL.vertices[i],
							 positionVL.vertices[i + 1],
							 positionVL.vertices[i + 2]);
		hitboxCoords.push_back(coordinate + origin);
	}
	return hitboxCoords;
}

std::vector<std::vector<glm::vec3>> Shape::getAbsoluteTriangles()
{
	auto faces = m_pVA->getTriangles();
	for (auto &face : faces)
	{
		for (auto &vertex : face)
		{
			vertex = m_modelMat * glm::vec4(vertex, 1.0f);
		}
	}

	return faces;
}

std::vector<std::vector<glm::vec3>> Shape::getRelativeTriangles()
{
	return m_pVA->getTriangles();
}
