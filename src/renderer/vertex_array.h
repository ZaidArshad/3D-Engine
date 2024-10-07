#pragma once

#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "texture.h"

struct VertexLayout
{
	GLuint VBO = 0;
	std::vector<float> vertices;
	unsigned int stride = 0;
};

class VertexArray
{
public:
	VertexArray();
	VertexArray(const std::vector<float> &vertices,
				unsigned int stride,
				const std::vector<unsigned int> &indices);
	~VertexArray();

	const std::vector<VertexLayout> getVertexLayouts();
	std::vector<std::vector<glm::vec3>> getTriangles();

	void bind();
	void unbind();
	void addBuffer(const std::vector<float> &vertices, unsigned int stride);
	void setIndices(const std::vector<unsigned int> &indices);
	void updateBuffer(unsigned int i, const std::vector<float> &vertices, GLenum drawType);
	void draw();

private:
	GLuint m_VAO;
	GLuint m_IBO;
	std::vector<VertexLayout> m_vertexLayouts;
	std::vector<unsigned int> m_indices;
};
