#include "Zombie.h"

bool Zombie::_ifSpawn = false;
Zombie Zombie::newcommer;
static constexpr float PI = 3.1415926535f;

bool Zombie::detect_walls(const std::vector<Wall>& walls)
{
	float _center_x{ _target_dir.x * _speed + _pos_and_size.x + _pos_and_size.z * .5f },
		_center_y{ _target_dir.y * _speed + _pos_and_size.y + _pos_and_size.w * .5f };
	glm::vec4 w_pos_and_size;
	float test_x{ 0.f }, test_y{ 0.f };
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
			return true;
		}
	}
	return false;
}

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

void Zombie::update(const std::vector<Wall>& walls)
{
	if (_mState != MovingState::IDLE)
		if (!detect_walls(walls))
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