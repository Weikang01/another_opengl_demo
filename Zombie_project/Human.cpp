#include "Human.h"
#include <gtx/rotate_vector.hpp>
#include <random>
#include <ctime>

static std::mt19937 randomEngine((time(nullptr)));
static std::uniform_real_distribution<float> _rand(-1.f, 1.f);
static std::uniform_real_distribution<float> _randRotate(glm::radians(-45.f), glm::radians(45.f));

void Human::init()
{
	_targetDir.x = _rand(randomEngine);
	_targetDir.y = _rand(randomEngine);
	if (_targetDir.x == 0.f && _targetDir.y == 0.f) _targetDir.x = 1.f;
	_targetDir = glm::normalize(_targetDir);
}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans, std::vector<Zombie*>& zombies)
{
	if (_frame > 120)
	{
		_targetDir = glm::rotate(_targetDir, _randRotate(randomEngine));
		_frame = 0;
	}
	else
		_frame++;

	_pos += _targetDir * _speed;
	if (collideWithLevel(levelData))
	{
		_targetDir = glm::rotate(_targetDir, _randRotate(randomEngine));
	}
}

