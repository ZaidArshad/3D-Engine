#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "camera.h"
#include "helper/app_tools.h"

Camera::Camera()
{
	m_offset = glm::vec3(0.0f, 0.25f, -0.75f);
	m_view = glm::mat4(1.0f);
	m_orthoProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	m_perspProjection = glm::perspective(45.0f,
										 (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
										 0.01f,
										 1000.0f);
}

Camera::~Camera()
{
}

void Camera::setView(glm::mat4 view)
{
	m_view = view;
}

const glm::mat4 Camera::getView()
{
	return m_view;
}

const glm::mat4 Camera::getOrthoProjection()
{
	return m_orthoProjection;
}

const glm::mat4 Camera::getPerpsProjection()
{
	return m_perspProjection;
}

void Camera::translate(float x, float y, float z)
{
	m_view = glm::translate(m_view, glm::vec3(x, y, z));
}

void Camera::rotate(float radians, float x, float y, float z)
{
	m_view = glm::rotate(m_view, radians, glm::vec3(x, y, z));
}

void Camera::followModel(const glm::mat4 &model)
{
	glm::vec3 modelPos = model[3];
	glm::vec3 cameraPos = modelPos + m_offset;
	m_view = glm::lookAt(cameraPos,
						 modelPos,
						 glm::vec3(0, 1, 0));
}

void Camera::pan(GLFWwindow *window, double scroll)
{
	double dragX = 0;
	double dragY = 0;
	m_controller.mouseDrag2D(window, &dragX, &dragY);
	m_offset[0] += dragX;
	m_offset[1] -= dragY;
	m_offset[2] += scroll;
}
