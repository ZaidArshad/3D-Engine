#pragma once

#include "shape.h"

class Cube : public Shape
{
public:
	Cube();
	Cube(float length);
	~Cube();

	// -- Utility -- //
	std::vector<float> getPosition();

private:
	float m_length;
};
