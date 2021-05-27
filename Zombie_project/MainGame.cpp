#include "MainGame.h"
#include <test_Engine/Engine.h>
#include <iostream>

MainGame::MainGame()
	:_screen_width(800), _screen_height(600), _gameState(GameState::PLAY),
	_nrHumans(0), _nrZombies(0), _nrWalls(0) 
{
	srand(time_t());

	_levels.push_back(new Level("Levels/level1.lvl"));
	_camera.init(_screen_width, _screen_height);
	_nrHumans = _levels[0]->getNrHumans();
	_humans.resize(_nrHumans);
	_zombies.resize(_nrZombies);

}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystem();

	_shader.compile("Shaders/basicColor_vert.glsl", "Shaders/basicColor_frag.glsl");

	gameLoop();
}

void MainGame::initSystem()
{
	test_Engine::init();

	_window.create("Zombie Rush", _screen_width, _screen_height, test_Engine::WindowFlags::WINDOW);


	glm::vec2 _tileSize = glm::vec2(20.f);

	for (size_t i = 0; i < _nrHumans; i++)
	{
		float r_horizon = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r_vertical = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		r_horizon = (r_horizon - 0.5f) * _screen_width;
		r_vertical = (r_vertical - 0.5f) * _screen_height;

		_humans[i] = Human(glm::vec2(r_horizon, r_vertical), _tileSize, .75f, 100.f, "Textures/human/human.png");
	}

	float start_point_x = -_screen_width * 0.6f;
	float start_point_y = -_screen_height * 0.6f;
	float wall_width{0.f};
	float wall_height = _levels[0]->_levelData.size() * _tileSize.y;


	Wall::add_new_type('R', "Textures/boxes/box1.png");
	Wall::add_new_type('L', "Textures/boxes/box4.png");
	Wall::add_new_type('G', "Textures/boxes/box2.png");
	float _vertical_offset{ 0.f }, _horizon_offset{ 0.f };

	for (int col = _levels[0]->_levelData.size() - 1; col >= 0; col--)
	{
		//std::cout << _level._levelData[col];
		wall_width = _levels[0]->_levelData[col].size() * _tileSize.x;
		for (unsigned int row = 0; row < _levels[0]->_levelData[col].size(); row++)
		{
			char value = _levels[0]->_levelData[col][row];
			auto iter = Wall::_texture_map.find(value);
			if (iter != Wall::_texture_map.end())
			{
				_walls.push_back(Wall(glm::vec2(start_point_x + _horizon_offset, start_point_y + _vertical_offset), _tileSize, iter->first));
			}
			else if (value == 'Z')
			{
				_zombies.push_back(Zombie(glm::vec2(start_point_x + _horizon_offset, start_point_y + _vertical_offset), _tileSize, .7f, 500.f, "Textures/zombie/zombie.png"));
			}
			else if (value == '@')
			{
				_player = Agent(glm::vec2(start_point_x + _horizon_offset, start_point_y + _vertical_offset), _tileSize, 5.f, "Textures/player/player.png");
			}
			_horizon_offset += _tileSize.x;
		}
		_vertical_offset += _tileSize.y;
		_horizon_offset = 0.f;
	}


	_spriteBatch.init();
	_fpsLimiter.init(60.f);

}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		_fpsLimiter.begin();
		processInput();
		_camera.update();

		

		drawGame();

		//std::cout << SDL_GetTicks() << std::endl;

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
	if (_inputManager.isKeyPressed(SDLK_w))
	{
		_player.move(glm::vec2(0.f, 1.f));
		_camera.setPosition(_player.getPos());
	}
	if (_inputManager.isKeyPressed(SDLK_s))
	{
		_player.move(glm::vec2(0.f, -1.f));
		_camera.setPosition(_player.getPos());
	}
	if (_inputManager.isKeyPressed(SDLK_d))
	{
		_player.move(glm::vec2(1.f, 0.f));
		_camera.setPosition(_player.getPos());
	}
	if (_inputManager.isKeyPressed(SDLK_a))
	{
		_player.move(glm::vec2(-1.f, 0.f));
		_camera.setPosition(_player.getPos());
	}
	if (_inputManager.isKeyPressed(SDLK_SPACE) || _inputManager.isKeyPressed(SDLK_ESCAPE) || _inputManager.isKeyPressed(SDLK_RETURN))
		_gameState = GameState::EXIT;
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouseCoords = _camera.screenToWorld(_inputManager.getMouseCoord());
		glm::vec2 playerPosition = _player.getPos();
		glm::vec2 direction = glm::normalize(mouseCoords - playerPosition);

		_bullets.emplace_back(playerPosition, glm::vec2(8.f), direction, 10.f, 120, "Textures/bullet/Bullet.png");
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

	for (unsigned int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i].detect_humans(_humans);
		_zombies[i].update(_walls);
		if (Zombie::_ifSpawn)
		{
			_zombies.push_back(Zombie::newcommer);
			Zombie::_ifSpawn = false;
		}

		_zombies[i].draw(_spriteBatch);
	}

	for (unsigned int i = 0; i < _humans.size(); i++)
	{
		_humans[i].detect_zombies(_zombies);
		_humans[i].update(_walls);
		_humans[i].draw(_spriteBatch);
	}

	_player.draw(_spriteBatch);

	for (unsigned int i = 0; i < _walls.size(); i++)
	{
		_walls[i].draw(_spriteBatch);
	}

	for (size_t i = 0; i < _bullets.size();)
	{
		_bullets[i].detectHit(_walls, _zombies);
		if (!_bullets[i].update())
		{
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else
		{
			_bullets[i].draw(_spriteBatch);
			i++;
		}
	}



	_spriteBatch.end();
	_spriteBatch.renderBatch();
	glBindTexture(GL_TEXTURE_2D, 0);
	_shader.unuse();
}
