#include <iostream>

#include "glm/gtc/matrix_transform.hpp"

#include "player.h"
#include "helper/app_tools.h"

Player::Player()
{
	m_model = new Model("res/Models/waddle/frames/");
	m_model->setTexture("res/Models/waddle/HrDee.00.png");
	m_model->scale(0.1f, 0.1f, 0.1f);
	m_model->translate(0.0f, 0.0f, 0.0f);
	m_velocity = 0.10;
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

	m_model->shift(-x * m_velocity, 0, z * m_velocity);
}
