#include <iostream>
#include <test_Engine/GLTexture.h>
#include <test_Engine/ResourceManager.h>
#include "Bullet.h"
#include "Zombie.h"
#include "Level.h"

using test_Engine::GLTexture;
using test_Engine::ResourceManager;


Bullet::Bullet(glm::vec2 pos, glm::vec2 size, glm::vec2 dir, int damage,
	float speed, int lifeTime, unsigned int textureID)
	: _pos(pos), _size(size), _speed(speed), _damage(damage),
	_direction(dir), _lifeTime(lifeTime), _getHit(false), _texture(textureID)
{
}

Bullet::~Bullet()
{
}

bool Bullet::update(const std::vector<std::string>& levelData)
{
	_pos.x += _direction.x * _speed;
	_pos.y += _direction.y * _speed;
	return (_lifeTime--) && (!collideWithLevel(levelData));
}

void Bullet::draw(SpriteBatch& spriteBatch)
{
	glm::vec4 uv(0.f, 0.f, 1.f, 1.f);
	spriteBatch.draw(glm::vec4(_pos.x, _pos.y, _size.x, _size.y), uv, _texture, 0.f, glm::vec4(255, 255, 255, 255));
}


float diff_x;
float diff_y;
bool Bullet::collideWithAgent(Agent* agent)
{
	diff_x = agent->getPos().x + AGENT_WIDTH * .5f - _pos.x - _size.x * .5f;
	diff_y = agent->getPos().y + AGENT_WIDTH * .5f - _pos.y - _size.y * .5f;

	if (diff_x * diff_x + diff_y * diff_y < AGENT_WIDTH * AGENT_WIDTH * .25f)
	{
		agent->_health -= _damage;
		return true;
	}
	return false;
}

bool Bullet::collideWithLevel(const std::vector<std::string>& levelData)
{
	int gridX = floor(_pos.x / TILE_WIDTH);
	int gridY = floor(_pos.y / TILE_WIDTH);


	if (gridY < 0 || gridX < 0 || gridY >= levelData.size() || gridX >= levelData[0].size())
		return true;
	return levelData[gridY][gridX] != '.';
}

