#pragma once

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "camera/camera.h"
#include "shader/shader.h"
#include "shape/shape.h"
#include "helper/app_tools.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	// -- Getters -- //
	Camera *getCamera();

	// -- Utility -- //
	void updateMVP(glm::mat4 model,
				   glm::mat4 view,
				   glm::mat4 projection,
				   Shader *shader);
	void addShape(Shape *shape, bool is3D = false);
	void drawShapes(Shader *shader);
	void clear();

private:
	std::vector<Shape *> m_2DShapes;
	std::vector<Shape *> m_3DShapes;

	Camera m_camera;
};
