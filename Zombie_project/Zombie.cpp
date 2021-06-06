#include "Zombie.h"
#include <iostream>

static glm::vec2 _diff;

void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime)
{
	_pos += m_direction * _speed * deltaTime;
	collideWithLevel(levelData);
}

void Zombie::setTarget(const Human* first)
{
	_diff = first->getPos() - _pos;
	_dist = glm::length(_diff);
	m_direction = glm::normalize(_diff);
}

void Zombie::checkTarget(const Human* human)
{
	_diff = human->getPos() - _pos;
	if (_dist > glm::length(_diff))
		m_direction = glm::normalize(_diff);
}
