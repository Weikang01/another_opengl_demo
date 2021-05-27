#pragma once
#include <glm.hpp>
#include <test_Engine/GLTexture.h>
#include <test_Engine/SpriteBatch.h>
#include "Zombie.h"
#include "Wall.h"

using test_Engine::SpriteBatch;
using test_Engine::GLTexture;

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 size, glm::vec2 dir, float speed, int lifeTime, const char* texturePath);
	~Bullet();

	void detectHit(const std::vector<Wall>& walls, std::vector<Zombie>& zombies);
	bool update();
	void draw(SpriteBatch& spriteBatch);

private:
	int _lifeTime;
	float _speed;
	bool _getHit;
	glm::vec2 _direction;
	glm::vec4 _pos_and_size;
	GLTexture _texture;
};
