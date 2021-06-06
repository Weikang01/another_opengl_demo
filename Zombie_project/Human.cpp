#include "Human.h"
#include <gtx/rotate_vector.hpp>
#include <random>
#include <ctime>

static std::mt19937 randomEngine((time(nullptr)));
static std::uniform_real_distribution<float> _rand(-1.f, 1.f);
static std::uniform_real_distribution<float> _randRotate(glm::radians(-45.f), glm::radians(45.f));

void Human::init()
{
	m_direction.x = _rand(randomEngine);
	m_direction.y = _rand(randomEngine);
	if (m_direction.x == 0.f && m_direction.y == 0.f) m_direction.x = 1.f;
	m_direction = glm::normalize(m_direction);
}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime)
{
	if (_frame > 120)
	{
		m_direction = glm::rotate(m_direction, _randRotate(randomEngine));
		_frame = 0;
	}
	else
		_frame++;

	_pos += m_direction * _speed * deltaTime;
	if (collideWithLevel(levelData))
	{
		m_direction = glm::rotate(m_direction, _randRotate(randomEngine));
	}
}

