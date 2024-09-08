#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include "renderer.h"
#include "helper/app_tools.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

Camera *Renderer::getCamera()
{
	return &m_camera;
}

void Renderer::addShape(Shape *shape, bool is3D)
{
	if (is3D)
	{
		m_3DShapes.push_back(shape);
	}
	else
	{
		m_2DShapes.push_back(shape);
	}
}

void Renderer::drawShapes(Shader *shader)
{
	// Switch to orthographic projection and not using view matrix for 2D shapes
	for (Shape *shape : m_2DShapes)
	{
		updateMVP(shape->getModelMatrix(),
				  glm::mat4(1.0f),
				  m_camera.getOrthoProjection(),
				  shader);
		shape->draw();
	}

	// Switch to perspective projection for 3D shapes
	for (Shape *shape : m_3DShapes)
	{
		updateMVP(shape->getModelMatrix(),
				  m_camera.getView(),
				  m_camera.getPerpsProjection(),
				  shader);
		shape->draw();
	}
}

void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::updateMVP(glm::mat4 model,
						 glm::mat4 view,
						 glm::mat4 projection,
						 Shader *shader)
{
	glm::mat4 mvp = projection * view * model;
	GLint location = glGetUniformLocation(shader->getProgram(), "u_MVP");
	glUniformMatrix4fv(location, 1, GL_FALSE, &mvp[0][0]);
}