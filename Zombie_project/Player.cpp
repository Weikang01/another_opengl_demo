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
	std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime)
{
	glm::vec2 mousePos = _camera->screenToWorld(_inputManager->getMouseCoord());
	m_direction = glm::normalize(mousePos - _pos);

	if (_inputManager->isKeyDown(SDLK_w))
	{
		_pos.y += _speed * deltaTime;
		//_camera->setPosition(_pos);
	}
	if (_inputManager->isKeyDown(SDLK_s))
	{
		_pos.y -= _speed * deltaTime;
		//_camera->setPosition(_pos);
	}
	if (_inputManager->isKeyDown(SDLK_d))
	{
		_pos.x += _speed * deltaTime;
		//_camera->setPosition(_pos);
	}
	if (_inputManager->isKeyDown(SDLK_a))
	{
		_pos.x -= _speed * deltaTime;
		//_camera->setPosition(_pos);
	}
	if (_inputManager->isKeyDown(SDLK_1) && _guns.size() >= 0)
	{
		_currentGun = 0;
	}
	if (_inputManager->isKeyDown(SDLK_2) && _guns.size() >= 1)
	{
		_currentGun = 1;
	}
	if (_inputManager->isKeyDown(SDLK_3) && _guns.size() >= 2)
	{
		_currentGun = 2;
	}
	if (_currentGun != -1)
	{
		_guns[_currentGun]->update(_inputManager->isKeyDown(SDL_BUTTON_LEFT), _pos, m_direction, *_bullets, deltaTime);
	}
	collideWithLevel(levelData);
}
