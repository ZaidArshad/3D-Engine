#include <iostream>

#include "glm/gtc/matrix_transform.hpp"

#include "player.h"
#include "helper/app_tools.h"

Player::Player()
{
	m_model = new Model("res/Models/waddle/frames/");
	m_model->setTexture("res/Models/waddle/HrDee.00.png");
	m_model->scale(glm::vec3(0.1f));
	m_velocity = 0.05f;
}

Player::~Player()
{
	delete m_model;
}

Shape *Player::getModel()
{
	return m_model;
}

void Player::move(GLFWwindow *window)
{
	int x = 0;
	int y = 0;
	int z = 0;

	m_controller.move3D(window, &x, &y, &z);
	if (!x && !y && !z)
	{
		m_model->resetFrames();
		return;
	}
	m_model->nextFrame();

	float heading = AppTools::calculateHeading(x, z);
	if (m_model->getRotation()[1] != heading)
	{
		m_model->setRotation(glm::vec3(0, heading, 0));
		m_rotation = heading;
	}

	const auto translation = glm::vec3(-x, y, z);
	m_model->shift(translation * m_velocity);
}

bool sameSide(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b)
{
	const auto cp1 = glm::cross(b - a, p1 - a);
	const auto cp2 = glm::cross(b - a, p2 - a);
	// std::cout << glm::dot(cp1, cp2) << std::endl;
	return glm::dot(cp1, cp2) >= 0;
}

bool pointInTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	return sameSide(p, a, b, c) && sameSide(p, b, a, c) && sameSide(p, c, a, b);
}

float signedDistance(glm::vec3 p, glm::vec3 normal, glm::vec3 originT)
{
	float plane = glm::dot(originT, normal);
	return glm::dot(p, normal) - plane;
}

bool Player::checkCollision(Shape *collider)
{
	const auto ellipse = m_model->getDimensions() * (0.1f / 2);

	const auto playerOrigin = m_model->getModelMatrix()[3];
	// const auto coords = collider->getHitboxCoords();
	// for (const auto &hitboxCoord : coords)
	// {
	// 	glm::vec3 result = (playerOrigin - hitboxCoord) / (ellipse);
	// 	if (glm::length(result) < 1)
	// 	{
	// 		return true;
	// 	}
	// }
	// return false;
	const auto colliderOrigin = collider->getModelMatrix()[3];
	const auto faces = collider->getAbsoluteTriangles();
	for (const auto &face : faces)
	{
		const auto normal = glm::normalize(glm::cross(face[1] - face[0], face[2] - face[0]));
		std::cout << glm::abs(signedDistance(playerOrigin, normal, colliderOrigin)) << " " << pointInTriangle(playerOrigin, face[0], face[1], face[2]) << " " << glm::length(ellipse) << std::endl;
		if (glm::abs(signedDistance(playerOrigin, normal, colliderOrigin)) < glm::length(ellipse) &&
			pointInTriangle(playerOrigin, face[0], face[1], face[2]))
		{
			collider->setColor(0, 1.0f, 0, 1.0f);
			return true;
		}
	}
	collider->setColor(1.0f, 0, 0, 1.0f);
	return false;
}
