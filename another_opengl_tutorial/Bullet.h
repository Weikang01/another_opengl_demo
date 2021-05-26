#pragma once
#include <glm.hpp>
#include <test_Engine/SpriteBatch.h>

using test_Engine::SpriteBatch;

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 size, glm::vec2 dir, float speed, int lifeTime);
	~Bullet();

	bool update();
	void draw(SpriteBatch& spriteBatch);

private:
	int _lifeTime;
	float _speed;
	glm::vec2 _direction;
	glm::vec4 _pos_and_size;
};

