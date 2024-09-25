#include <iostream>

#include "glm/gtc/matrix_transform.hpp"

#include "player.h"
#include "helper/app_tools.h"

Player::Player()
{
	m_model = new Model("res/Models/waddle/frames/");
	m_model->setTexture("res/Models/waddle/HrDee.00.png");
	m_model->scale(glm::vec3(0.1f));
	m_velocity = 0.025f;
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
	int z = 0;

	m_controller.move2D(window, &x, &z);
	if (x == 0 && z == 0)
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

	const auto translation = glm::vec3(-x, 0, z);
	m_model->shift(translation * m_velocity);
}

bool Player::checkCollision(Shape *collider)
{
	const auto ellipse = glm::vec3(0.08f);
	const auto playerOrigin = glm::vec3(m_model->getModelMatrix()[3]);
	const auto coords = collider->getHitboxCoords();
	for (const auto &hitboxCoord : coords)
	{
		glm::vec3 result = (playerOrigin - hitboxCoord) / (ellipse);
		if (glm::length(result) < 1)
		{
			return true;
		}
	}
	return false;
}
