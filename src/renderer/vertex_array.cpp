#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "vertex_array.h"
#include "helper/logger.h"

VertexArray::VertexArray()
{
	m_VAO = 0;
	m_IBO = 0;
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &m_IBO);
}

VertexArray::VertexArray(const std::vector<float> &vertices,
						 unsigned int stride,
						 const std::vector<unsigned int> &indices)
{
	m_VAO = 0;
	m_IBO = 0;
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	addBuffer(vertices, stride);

	glGenBuffers(1, &m_IBO);
	setIndices(indices);
}

VertexArray::~VertexArray()
{
	m_VAO = 0;
	m_IBO = 0;
}

const std::vector<VertexLayout> VertexArray::getVertexLayouts()
{
	return m_vertexLayouts;
}

void VertexArray::bind()
{
	glBindVertexArray(m_VAO);
}

void VertexArray::unbind()
{
	glBindVertexArray(0);
}

void VertexArray::addBuffer(const std::vector<float> &vertices,
							unsigned int stride)
{
	glBindVertexArray(m_VAO);

	// Layout generation
	VertexLayout layout;
	layout.vertices = vertices;
	layout.stride = stride;

	// VBO generation
	glGenBuffers(1, &layout.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, layout.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
				 vertices.data(), GL_STATIC_DRAW);

	// Binding VBO to VAO
	int index = m_vertexLayouts.size();
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, layout.stride, GL_FLOAT, GL_FALSE,
						  0,
						  NULL);
	m_vertexLayouts.push_back(layout);
}

void VertexArray::setIndices(const std::vector<unsigned int> &indices)
{
	glBindVertexArray(m_VAO);

	// IBO Generation
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 indices.size() * sizeof(unsigned int),
				 indices.data(), GL_STATIC_DRAW);
	m_indices = indices;
}

void VertexArray::updateBuffer(unsigned int i, const std::vector<float> &vertices, GLenum drawType)
{
	if (i > m_vertexLayouts.size())
	{
		Logger::log("Index " + std::to_string(i) +
					" out of bounds. Number of vertice layouts = " +
					std::to_string(m_vertexLayouts.size()));
		return;
	}

	glBindVertexArray(m_VAO);

	// Updating layout
	VertexLayout *layout = &m_vertexLayouts[i];
	if (vertices.size() != layout->vertices.size())
	{
		Logger::log("Vertice size mismatch (" +
					std::to_string(vertices.size()) + "," +
					std::to_string(layout->vertices.size()) +
					") for vertices at index: " + std::to_string(i));
		return;
	}
	layout->vertices = vertices;

	// Updating VBO
	glBindBuffer(GL_ARRAY_BUFFER, layout->VBO);
	glBufferData(GL_ARRAY_BUFFER, layout->vertices.size() * sizeof(float),
				 layout->vertices.data(), drawType);

	// Binding VBO to VAO
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, layout->stride, GL_FLOAT, GL_FALSE, 0, NULL);
}

void VertexArray::draw()
{
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

glm::vec3 bufferToVec(const std::vector<float> &buffer, const unsigned int i)
{
	const auto trueI = 3 * i;
	return glm::vec3(buffer[trueI], buffer[trueI + 1], buffer[trueI + 2]);
}

std::vector<std::vector<glm::vec3>> VertexArray::getTriangles()
{
	const VertexLayout positionVL = m_vertexLayouts[0];
	const std::vector<float> positionBuffer = positionVL.vertices;
	std::vector<std::vector<glm::vec3>> faces;
	for (int i = 0; i < m_indices.size(); i += 3)
	{
		std::vector<glm::vec3> face{
			bufferToVec(positionBuffer, m_indices[i]),
			bufferToVec(positionBuffer, m_indices[i + 1]),
			bufferToVec(positionBuffer, m_indices[i + 2]),
		};

		faces.push_back(face);
	}
	return faces;
}