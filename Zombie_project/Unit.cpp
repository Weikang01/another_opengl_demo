#include "Unit.h"
#include<iostream>
#include <test_Engine/ResourceManager.h>
static constexpr float PI = 3.1415926535f;

Unit::Unit(glm::vec2 pos, glm::vec2 size, float speed, const char* texturePath)
	:_pos_and_size(pos.x, pos.y, size.x, size.y), _speed(speed)
{
	_texture =
		test_Engine::ResourceManager::getTexture(texturePath);
}

void Unit::setPos(float x, float y)
{
	_pos_and_size.x = x;
	_pos_and_size.y = y;
}

void Unit::move(const glm::vec2& dir)
{
	_pos_and_size.x += dir.x * _speed;
	_pos_and_size.y += dir.y * _speed;
}

void Unit::draw(SpriteBatch& spriteBatch) const
{
	glm::vec4 uv(0.f, 0.f, 1.f, 1.f);
	spriteBatch.draw(_pos_and_size, uv, _texture.id, 0.f, glm::vec4(255, 255, 255, 255));
}

bool Human::detect_zombies(const std::vector<Zombie>& zombies)
{
	glm::vec2 _pos = this->getPos();
	bool refresh = false;
	for (size_t i = 0; i < zombies.size(); i++)
	{
		glm::vec2 z_pos = zombies[i].getPos();
		float dist = glm::distance(_pos, z_pos);
		if (dist < _range)
		{
			if (!refresh)
			{
				_target_dir.x = 0.f;
				_target_dir.y = 0.f;
				refresh = true;
			}
			float attenuation = 1.f / (dist + .01f);
			_target_dir += attenuation * (z_pos - _pos);
		}
	}

	if (refresh)
	{
		_has_roam_dir = false;
		_target_dir = glm::normalize(_target_dir);
		_mState = MovingState::ESCAPE;
		return true;
	}
	else
	{
		roam();
		return false;
	}
}

void Human::roam()
{
	if (_lastTicks + _roamInterval < SDL_GetTicks())
	{
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // rand(0.f,1.f)
		if (_mState == MovingState::IDLE && r < _roamVec[0])
		{
			if (!_has_roam_dir)
			{
				float r_theta = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.f * PI; // rand(0.f,1.f)
				float r_x = cosf(r_theta);
				float r_y = sinf(r_theta);
				_target_dir = glm::vec2(r_x, r_y);
				_has_roam_dir = true;
			}
			_mState = MovingState::ROAM;
		}
		else if (_mState == MovingState::ROAM && r < _roamVec[1])
		{
			_has_roam_dir = false;
			_mState = MovingState::IDLE;
		}
		_lastTicks = SDL_GetTicks();
	}
}

void Human::update()
{
	if (_mState == MovingState::ESCAPE)
		this->move(-_target_dir);
	else if (_mState == MovingState::ROAM)
		this->move(_target_dir);
}


bool Zombie::_ifSpawn = false;
Zombie Zombie::newcommer;

bool Zombie::detect_humans(std::vector<Human>& humans)
{
	bool refresh = false;
	glm::vec2 _pos = this->getPos();
	for (size_t i = 0; i < humans.size(); i++)
	{
		glm::vec2 h_pos = humans[i].getPos();
		float dist = glm::distance(_pos, h_pos);
		if (dist < _detect_range)
		{
			if (!refresh)
			{
				_closest_dist = dist;
				_target_dir = h_pos - _pos;
				refresh = true;
			}
			if (dist < _closest_dist)
			{
				_closest_dist = dist;
				_target_dir = h_pos - _pos;
			}
			if (dist < _convert_range)
			{
				Zombie::_ifSpawn = true;
				Zombie::newcommer = Zombie(this, humans[i]);
				humans[i] = humans.back();
				humans.pop_back();
			}
		}
	}

	if (refresh)
	{
		_has_roam_dir = false;
		_target_dir = glm::normalize(_target_dir);
		_mState = MovingState::CHASE;
		return true;
	}
	else
	{
		roam();
		return false;
	}
}

void Zombie::update()
{
	if (_mState != MovingState::IDLE)
		this->move(_target_dir);
}

void Zombie::roam()
{
	if (_lastTicks + _roamInterval < SDL_GetTicks())
	{
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // rand(0.f,1.f)
		if (_mState == MovingState::IDLE && r < _roamVec[0])
		{
			if (!_has_roam_dir)
			{
				float r_theta = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.f * PI; // rand(0.f,1.f)
				float r_x = cosf(r_theta);
				float r_y = sinf(r_theta);
				_target_dir = glm::vec2(r_x, r_y);
				_has_roam_dir = true;
			}
			_mState = MovingState::ROAM;
		}
		else if (_mState == MovingState::ROAM && r < _roamVec[1])
		{
			_has_roam_dir = false;
			_mState = MovingState::IDLE;
		}
		_lastTicks = SDL_GetTicks();
	}
}

