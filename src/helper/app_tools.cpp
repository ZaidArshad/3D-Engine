#include <iostream>
#include <sstream>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "app_tools.h"
#include "logger.h"

float AppTools::normalizeX(float position)
{
	return (position - WINDOW_WIDTH / 2) / (WINDOW_WIDTH / 2);
}

float AppTools::normalizeY(float position)
{
	return (WINDOW_HEIGHT / 2 - position) / (WINDOW_HEIGHT / 2);
}

// Calculated for 45 degree special triangle
float AppTools::calculateHeading(float x, float y)
{
	double heading = 0;
	x = -x;
	if (x != 0 && y != 0)
	{
		heading = glm::atan(x / y);
	}
	// When y is zero
	else if (x != 0)
	{
		heading = x * glm::half_pi<double>();
	}
	if (y < 0)
	{
		heading += glm::pi<double>();
	}

	return heading;
}

template <class T>
void AppTools::printVec(const T &vec, int length)
{
	for (int i = 0; i < length; i++)
	{
		std::cout << vec[i] << " ";
	}
	std::cout << std::endl;
}
