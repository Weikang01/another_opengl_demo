#include "Zombie.h"
#include <iostream>

static glm::vec2 _diff;

void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans, std::vector<Zombie*>& zombies)
{
	_pos += _targetDir * _speed;
	collideWithLevel(levelData);
}

void Zombie::setTarget(const Human* first)
{
	_diff = first->getPos() - _pos;
	_dist = glm::length(_diff);
	_targetDir = glm::normalize(_diff);
}

void Zombie::checkTarget(const Human* human)
{
	_diff = human->getPos() - _pos;
	if (_dist > glm::length(_diff))
		_targetDir = glm::normalize(_diff);
}
