#include "Gun.h"
#include <test_Engine/ResourceManager.h>
#include <gtx/rotate_vector.hpp>
#include <time.h>

#include "Bullet.h"

Gun::Gun(const char* name, int fireRate,
	int bulletsPerShot, float spread,
	const char* projectileTexturePath, float projSize,
	int damage, float projSpeed, int projLifeTime)
	:_name(name), _fireRate(fireRate), _bulletsPerShot(bulletsPerShot),
	_spread(spread), _projSize(projSize), _damage(damage), 
	_projLifeTime(projLifeTime), _projSpeed(projSpeed), _frameCounter(0)
{
	_projectileTextID =
		test_Engine::ResourceManager::getTexture(projectileTexturePath).id;
	_randRotate = std::uniform_real_distribution<float>(-_spread, _spread);
}

void Gun::update(bool isTriggered, const glm::vec2& _firePos, const glm::vec2& direction,
	std::vector<Bullet>& bullets)
{
	if (!isTriggered)
		return;
	_frameCounter++;
	if (_frameCounter >= _fireRate)
	{
		fire(_firePos, direction, bullets);
		_frameCounter = 0;
	}
}

void Gun::fire(const glm::vec2& _firePos, const glm::vec2& direction, std::vector<Bullet>& bullets)
{
	static std::mt19937 randomEngine((time(nullptr)));

	for (unsigned int i = 0; i < _bulletsPerShot; i++)
	{
		bullets.emplace_back(_firePos, glm::vec2(_projSize), glm::rotate(direction, _randRotate(randomEngine)), _damage,
			_projSpeed, _projLifeTime, _projectileTextID);
	}
}

