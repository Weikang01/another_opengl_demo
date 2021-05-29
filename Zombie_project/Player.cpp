#include "Player.h"
#include <test_Engine/ResourceManager.cpp>
#include "Gun.h"
#include <iostream>

void Player::addGun(Gun* gun)
{
	_guns.push_back(gun);
	if (_currentGun == -1)
		_currentGun = _guns.size() - 1;
}

void Player::init(InputManager* inputManager, Camera2D* camera, std::vector<Bullet>* bullets)
{
	_inputManager = inputManager;
	_camera = camera;
	_bullets = bullets;
}

void Player::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans, std::vector<Zombie*>& zombies)
{
	if (_inputManager->isKeyPressed(SDLK_w))
	{
		_pos.y += _speed;
		_camera->setPosition(_pos);
	}
	if (_inputManager->isKeyPressed(SDLK_s))
	{
		_pos.y -= _speed;
		_camera->setPosition(_pos);
	}
	if (_inputManager->isKeyPressed(SDLK_d))
	{
		_pos.x += _speed;
		_camera->setPosition(_pos);
	}
	if (_inputManager->isKeyPressed(SDLK_a))
	{
		_pos.x -= _speed;
		_camera->setPosition(_pos);
	}
	if (_inputManager->isKeyPressed(SDLK_1) && _guns.size() >= 0)
	{
		_currentGun = 0;
	}
	if (_inputManager->isKeyPressed(SDLK_2) && _guns.size() >= 1)
	{
		_currentGun = 1;
	}
	if (_inputManager->isKeyPressed(SDLK_3) && _guns.size() >= 2)
	{
		_currentGun = 2;
	}
	if (_currentGun >= 0)
	{
		if (_inputManager->isKeyPressed(SDL_BUTTON_LEFT))
		{
			glm::vec2 mousePos = _camera->screenToWorld(_inputManager->getMouseCoord());
			glm::vec2 direction = glm::normalize(mousePos - _pos);

			_guns[_currentGun]->update(true, _pos, direction, *_bullets);
		}
	}



	collideWithLevel(levelData);
}
