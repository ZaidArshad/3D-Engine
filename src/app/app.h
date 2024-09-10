#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "helper/app_tools.h"
#include "options.h"

class App
{
public:
	App();
	~App();
	Status run();

private:
	Status setup();
	void fpsCounter();

	double m_prevTime;
	unsigned int m_frameCount;

	GLFWwindow *m_window;
	Options *m_options;
};
