#include "MainGame.h"
#include <test_Engine/Engine.h>
#include <test_Engine/Errors.h>
#include <iostream>
#include <random>
#include "Gun.h"

using test_Engine::fatalError;

const bool DEBUG_MODE = false;
const float PLAYER_SPEED = 2.f;
const int PLAYER_HEALTH = 100;
const float HUMAN_SPEED = 1.f;
const int HUMAN_HEALTH = 40;
const float ZOMBIE_SPEED = .7f;
const int ZOMBIE_HEALTH = 100;
const float BULLET_SPEED = 20.f;

MainGame::MainGame()
	:_screen_width(800), _screen_height(600), _gameState(GameState::PLAY),
	_player(), _curLevel(0), _nrHumansKilled(0), _nrZombiesKilled(0)
{
	srand(time_t());
	_camera.init(_screen_width, _screen_height);
}

MainGame::~MainGame()
{
	for (size_t i = 0; i < _levels.size(); i++)
		delete _levels[i];
}

void MainGame::run()
{
	initSystem();

	initLevel();

	_shader.compile("Shaders/basicColor_vert.glsl", "Shaders/basicColor_frag.glsl");

	gameLoop();
}

void MainGame::initSystem()
{
	test_Engine::init();

	_window.create("Zombie Rush", _screen_width, _screen_height, test_Engine::WindowFlags::WINDOW);

	_spriteBatch.init();
	_fpsLimiter.init(60.f);

}

void MainGame::initLevel()
{
	_levels.push_back(new Level("Levels/level1.lvl"));

	_humans.resize(_levels[_curLevel]->getNrHumans() + 1);

	_zombies.resize(_levels[_curLevel]->getZombieStartPos().size());

	_player = new Player(_levels[_curLevel]->getPlayerStartPos(), PLAYER_SPEED, PLAYER_HEALTH, "Textures/player/player.png");
	_humans[0] = _player;

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> _randX(1, _levels[_curLevel]->getWidth() - 2);
	std::uniform_int_distribution<int> _randY(1, _levels[_curLevel]->getHeight() - 1);


	for (size_t i = 1; i < _humans.size(); i++)
	{
		_humans[i] =
			new Human(glm::vec2(_randX(randomEngine) * TILE_WIDTH, _randY(randomEngine) * TILE_WIDTH),
				HUMAN_SPEED, HUMAN_HEALTH, "Textures/human/human.png");
		_humans[i]->init();
	}

	for (size_t i = 0; i < _zombies.size(); i++)
		_zombies[i] = new Zombie(_levels[_curLevel]->getZombieStartPos()[i], ZOMBIE_SPEED, ZOMBIE_HEALTH, "Textures/zombie/zombie.png");

	// "Textures/bullet/Bullet.png"
	_player->init(&_inputManager, &_camera, &_bullets);
	
	//                      name, fireRate,
	_player->addGun(new Gun("Magnum", 30,
	// bulletsPerShot, spread
		1, glm::radians(5.f),
	//projectileTexturePath, projSize
		"Textures/bullet/Bullet.png", 10.f,
	//damage, projSpeed, projLifeTime
		50, BULLET_SPEED, 30));

	//                      name, fireRate,
	_player->addGun(new Gun("Shotgun", 60,
	// bulletsPerShot, spread
		20, glm::radians(40.f),
	//projectileTexturePath, projSize
		"Textures/bullet/Bullet.png", 10.f,
	//damage, projSpeed, projLifeTime
		20, BULLET_SPEED, 8));
	
	//                      name, fireRate,
	_player->addGun(new Gun("MP5", 10,
	// bulletsPerShot, spread
		1, glm::radians(10.f),
	//projectileTexturePath, projSize
		"Textures/bullet/Bullet.png", 10.f,
	//damage, projSpeed, projLifeTime
		30, BULLET_SPEED, 20));

}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		_fpsLimiter.begin();
		
		if (!DEBUG_MODE && !_zombies.size())
		{
			std::cout<< "YOU WIN! " << "\nZOMBIE KILLED: " << _nrZombiesKilled << "\nHUMAN KILLED: " << _nrHumansKilled
				<< "\nHUMAN REMAINED: " << _humans.size() - 1 <<"\nFINAL SCORE: " << _humans.size() * 50 - _nrHumansKilled * 20 << std::endl;
			fatalError("=================================");
		}
		processInput();
		_player->update(_levels[_curLevel]->_levelData, _humans, _zombies);
		_camera.update();

		drawGame();


		_fpsLimiter.end();
		_window.swapBuffer();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	const float CAM_SPEED = 5.f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEWHEEL:
			if (evnt.wheel.y > 0)
				_camera.setScale(_camera.getScale() + SCALE_SPEED);
			else if (evnt.wheel.y < 0)
				_camera.setScale(_camera.getScale() - SCALE_SPEED);
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoord(evnt.motion.x, evnt.motion.y);
			break;
		}
	}
	if (_inputManager.isKeyPressed(SDLK_LCTRL) && _inputManager.isKeyPressed(SDLK_LALT) && _inputManager.isKeyPressed(SDLK_a))
		_camera.screenshot("Textures/screenshot.png");

	if (_inputManager.isKeyPressed(SDLK_SPACE) || _inputManager.isKeyPressed(SDLK_ESCAPE) || _inputManager.isKeyPressed(SDLK_RETURN))
		_gameState = GameState::EXIT;
}

void MainGame::updateBullets()
{
	static bool getHit = false;

	for (unsigned int i = 0; i < _bullets.size(); i++)
	{
		getHit = false;
		if (!_bullets[i].update(_levels[_curLevel]->_levelData))
		{
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
			continue;
		}

		for (unsigned int j = 1; j < _humans.size(); j++)
		{
			if (_bullets[i].collideWithAgent(_humans[j]))
			{
				if (_humans[j]->_health <= 0)
				{
					_nrHumansKilled++;
					delete _humans[j];
					_humans[j] = _humans.back();
					_humans.pop_back();
				}
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				getHit = true;
				break;
			}
		}

		if (getHit)
			continue;

		for (unsigned int j = 0; j < _zombies.size(); j++)
		{
			if (_bullets[i].collideWithAgent(_zombies[j]))
			{
				if (_zombies[j]->_health <= 0)
				{
					_nrZombiesKilled++;
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
				}
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				getHit = true;
				break;
			}
		}

		if (getHit)
			continue;

		_bullets[i].draw(_spriteBatch);
	}
}

void MainGame::updateAgents()
{
	for (unsigned int i = 0; i < _zombies.size(); i++)
	{
		for (unsigned int j = i + 1; j < _zombies.size(); j++)
		{
			_zombies[i]->collideWithAgent(_zombies[j]);
		}

		_zombies[i]->setTarget(_player);
		if (_zombies[i]->collideWithAgent(_player) && !DEBUG_MODE)
		{
			std::cout << "YOU LOSE! " << "\nZOMBIE KILLED: " << _nrZombiesKilled << "\nHUMAN KILLED: " << _nrHumansKilled
				<<" \nZOMBIE REMAINED: " << _zombies.size() << "\nHUMAN REMAINED: " << _humans.size() - 1 << std::endl;
			fatalError("=================================");
		}
		for (unsigned int j = 1; j < _humans.size(); j++)
		{
			_zombies[i]->checkTarget(_humans[j]);
			if (_zombies[i]->collideWithAgent(_humans[j]))
			{
				_zombies.push_back(
					new Zombie(_humans[j]->getPos(), ZOMBIE_SPEED, ZOMBIE_HEALTH, "Textures/zombie/zombie.png"));
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
		}
		_zombies[i]->update(_levels[_curLevel]->_levelData, _humans, _zombies);
		_zombies[i]->draw(_spriteBatch);
	}


	for (unsigned int i = 0; i < _humans.size(); i++)
	{
		for (unsigned int j = i + 1; j < _humans.size(); j++)
		{
			_humans[i]->collideWithAgent(_humans[j]);
		}
		_humans[i]->update(_levels[_curLevel]->_levelData, _humans, _zombies);
		_humans[i]->draw(_spriteBatch);
	}
}

void MainGame::drawGame()
{
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shader.use();
	_shader.setInt("diffuse_texture", 0);
	_shader.setMat4fv("P", _camera.getCameraMat());
	_spriteBatch.begin();

	updateBullets();
	updateAgents();

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	_levels[_curLevel]->draw();



	glBindTexture(GL_TEXTURE_2D, 0);
	_shader.unuse();
}
