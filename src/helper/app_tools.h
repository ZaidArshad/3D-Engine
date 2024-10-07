#pragma once

#include <vector>
#include "glm/glm.hpp"

#define Status unsigned int

constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;

constexpr Status STATUS_OK = 0U;
constexpr Status STATUS_BAD = 1U;

class AppTools
{
public:
	static float normalizeX(float position);
	static float normalizeY(float position);
	static float calculateHeading(float x, float y);
	static void printVec(const glm::vec3 &vec);
};
