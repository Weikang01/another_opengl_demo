#include "Bullet.h"
#include <test_Engine/GLTexture.h>
#include <test_Engine/ResourceManager.h>
#include <iostream>

using test_Engine::GLTexture;
using test_Engine::ResourceManager;


Bullet::Bullet(glm::vec2 pos, glm::vec2 size, glm::vec2 dir, float speed, int lifeTime, const char* texturePath)
	: _pos_and_size(pos.x, pos.y, size.x, size.y), _speed(speed), _direction(dir), _lifeTime(lifeTime), _getHit(false)
{
	_texture = 
		test_Engine::ResourceManager::getTexture(texturePath);
}

Bullet::~Bullet()
{
}


void Bullet::detectHit(const std::vector<Wall>& walls, std::vector<Zombie>& zombies)
{

	float _center_x{ _pos_and_size.x + _pos_and_size.z * .5f },
		_center_y{ _pos_and_size.y + _pos_and_size.w * .5f };
	glm::vec4 w_pos_and_size;
	float test_x{0.f}, test_y{ 0.f };
	for (size_t i = 0; i < walls.size(); i++)
	{
		test_x = _center_x;
		test_y = _center_y;
		w_pos_and_size = walls[i].getPos_and_size();
		if (_center_x < w_pos_and_size.x) test_x = w_pos_and_size.x;
		else if (_center_x > w_pos_and_size.x + w_pos_and_size.z) test_x = w_pos_and_size.x + w_pos_and_size.z;

		if (_center_y < w_pos_and_size.y) test_y = w_pos_and_size.y;
		else if (_center_y > w_pos_and_size.y + w_pos_and_size.w) test_y = w_pos_and_size.y + w_pos_and_size.w;

		float distX = _center_x - test_x;
		float distY = _center_y - test_y;

		if (sqrtf((distX * distX) + (distY * distY)) < _pos_and_size.z)
		{
			_getHit = true;
			return;
		}
	}
	glm::vec2 _pos = glm::vec2(_pos_and_size.x, _pos_and_size.y);
	glm::vec2 _z_pos{};
	for (size_t i = 0; i < zombies.size(); i++)
	{
		_z_pos = zombies[i].getPos();
		if (glm::distance(_z_pos, _pos)
			< .5f * (_pos_and_size.z + zombies[i].getSize().x))
		{
			zombies[i] = zombies.back();
			zombies.pop_back();
			_getHit = true;
			return;
		}
	}
	_getHit = false;
}

bool Bullet::update()
{
	_pos_and_size.x += _direction.x * _speed;
	_pos_and_size.y += _direction.y * _speed;
	return (--_lifeTime) && (!_getHit);
}

void Bullet::draw(SpriteBatch& spriteBatch)
{
	glm::vec4 uv(0.f, 0.f, 1.f, 1.f);
	spriteBatch.draw(_pos_and_size, uv, _texture.id, 0.f, glm::vec4(255, 255, 255, 255));
}
