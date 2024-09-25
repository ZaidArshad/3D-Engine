#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class InputController
{
public:
	InputController();
	void move3D(GLFWwindow *window, int *x, int *y, int *z);
	void mouseDrag2D(GLFWwindow *window, double *x, double *y);

private:
	bool m_mouseClicked;
	double m_originMouseX;
	double m_originMouseY;
};
