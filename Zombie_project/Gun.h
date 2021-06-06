#pragma once
#include <glm.hpp>
#include <random>
#include <test_Engine/AudioEngine.h>

using test_Engine::SoundEffect;

class Bullet;

class Gun
{
public:
	Gun(const char* name, int fireRate, 
		int bulletsPerShot, float spread,
		const char* projectileTexturePath, float projSize,
		int damage, float projSpeed, int projLifeTime, SoundEffect fireSoundFX);
	~Gun() {}

	void update(bool isTriggered, const glm::vec2& _firePos, const glm::vec2& direction,
		std::vector<Bullet>& bullets, float deltaTime);

	const char* _name;
private:
	void fire(const glm::vec2& _firePos, const glm::vec2& direction, std::vector<Bullet>& bullets);
	int _fireRate; // fire rate in terms of frames
	int _bulletsPerShot; // how many bullets are fired at a time
	float _spread; // accuracy
	SoundEffect _fireSoundFX;

	// projectile
	int _projectileTextID;
	float _projSize;
	int _damage;
	float _projSpeed;
	int _projLifeTime;

	// frameCounter
	float _frameCounter;
	std::uniform_real_distribution<float> _randRotate;
};

