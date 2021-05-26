#include "Bullet.h"
#include <test_Engine/GLTexture.h>
#include <test_Engine/ResourceManager.h>
#include <iostream>

using test_Engine::GLTexture;
using test_Engine::ResourceManager;


Bullet::Bullet(glm::vec2 pos, glm::vec2 size, glm::vec2 dir, float speed, int lifeTime)
	: _pos_and_size(pos.x, pos.y, size.x, size.y), _speed(speed), _direction(dir), _lifeTime(lifeTime)
{
}

Bullet::~Bullet()
{
}


bool Bullet::update()
{
	_pos_and_size.x += _direction.x * _speed;
	_pos_and_size.y += _direction.y * _speed;
	if (!--_lifeTime) return false;
	return true;
}

void Bullet::draw(SpriteBatch& spriteBatch)
{
	glm::vec4 uv(0.f, 0.f, 1.f, 1.f);
	static GLTexture texture =
		test_Engine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/Bullet.png");
	//std::cout << texture.id << std::endl;
	spriteBatch.draw(_pos_and_size, uv, texture.id, 0.f, glm::vec4(255, 255, 255, 255));
}
