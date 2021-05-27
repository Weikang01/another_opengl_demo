#include "Agent.h"
#include<iostream>
#include <test_Engine/ResourceManager.h>

static constexpr float PI = 3.1415926535f;

Agent::Agent(glm::vec2 pos, glm::vec2 size, float speed, const char* texturePath)
	:_pos_and_size(pos.x, pos.y, size.x, size.y), _speed(speed)
{
	_texture =
		test_Engine::ResourceManager::getTexture(texturePath);
}

void Agent::setPos(float x, float y)
{
	_pos_and_size.x = x;
	_pos_and_size.y = y;
}

void Agent::move(const glm::vec2& dir)
{
	_pos_and_size.x += dir.x * _speed;
	_pos_and_size.y += dir.y * _speed;
}

void Agent::draw(SpriteBatch& spriteBatch) const
{
	glm::vec4 uv(0.f, 0.f, 1.f, 1.f);
	spriteBatch.draw(_pos_and_size, uv, _texture.id, 0.f, glm::vec4(255, 255, 255, 255));
}

