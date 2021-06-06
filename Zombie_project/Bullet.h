#pragma once
#include <glm.hpp>
#include <test_Engine/GLTexture.h>
#include <test_Engine/SpriteBatch.h>

class Agent;
class Human;
class Zombie;

using test_Engine::SpriteBatch;
using test_Engine::GLTexture;

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 size, glm::vec2 dir, int damage,
		float speed, int lifeTime, unsigned int textureID);
	~Bullet();

	bool update(const std::vector<std::string>& levelData, float deltaTime);
	void draw(SpriteBatch& spriteBatch);

	bool collideWithAgent(Agent* agent);

	glm::vec2 getPos() { return _pos; }
	glm::vec2 getSize() { return _size; }
private:
	bool collideWithLevel(const std::vector<std::string>& levelData);

	int _damage;
	glm::vec2 _pos;
	glm::vec2 _size;
	int _lifeTime;
	float _speed;
	bool _getHit;
	glm::vec2 _direction;
	unsigned int _texture;
};
