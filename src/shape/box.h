#pragma once

#include "glm/glm.hpp"

#include "shape.h"

class Box : public Shape
{
public:
	Box(float w, float h);
	~Box();
	void makeTiled(float rows, float cols);

	// -- Utility -- //
	std::vector<float> getPosition();

private:
	float m_width;
	float m_height;
};
