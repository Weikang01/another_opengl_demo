#include "MainGame.h"
#include <test_Engine/Engine.h>
#include <iostream>

MainGame::MainGame()
	:_screen_width(800), _screen_height(600), _gameState(GameState::PLAY),
	_nrHumans(100), _nrZombies(1)
{
	srand(time_t());
	_camera.init(_screen_width, _screen_height);
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

	for (size_t i = 0; i < _nrHumans; i++)
	{
		float r_horizon = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r_vertical = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		r_horizon = (r_horizon - 0.5f) * _screen_width;
		r_vertical = (r_vertical - 0.5f) * _screen_height;

		_humans[i] = Human(glm::vec2(r_horizon, r_vertical), glm::vec2(20.f), .5f, 100.f, "Textures/human/human.png");
	}

	for (size_t i = 0; i < _nrZombies; i++)
	{
		float r_horizon = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r_vertical = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		r_horizon = (r_horizon - 0.5f) * _screen_width;
		r_vertical = (r_vertical - 0.5f) * _screen_height;

		_zombies[i] = Zombie(glm::vec2(r_horizon, r_vertical), glm::vec2(20.f), .7f, 500.f, "Textures/zombie/zombie.png");
	}

	_player = Unit(glm::vec2(0.f), glm::vec2(20.f), 5.f, "Textures/player/player.png");

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
		//_camera.setPosition(_camera.getPosition() + glm::vec2(0.f, CAM_SPEED));
		_player.move(glm::vec2(0.f, 1.f));
		_camera.setPosition(_player.getPos());
	}
	if (_inputManager.isKeyPressed(SDLK_s))
	{
		//_camera.setPosition(_camera.getPosition() - glm::vec2(0.f, CAM_SPEED));
		_player.move(glm::vec2(0.f, -1.f));
		_camera.setPosition(_player.getPos());
	}
	if (_inputManager.isKeyPressed(SDLK_d))
	{
		//_camera.setPosition(_camera.getPosition() + glm::vec2(CAM_SPEED, 0.f));
		_player.move(glm::vec2(1.f, 0.f));
		_camera.setPosition(_player.getPos());
	}
	if (_inputManager.isKeyPressed(SDLK_a))
	{
		//_camera.setPosition(_camera.getPosition() - glm::vec2(CAM_SPEED, 0.f));
		_player.move(glm::vec2(-1.f, 0.f));
		_camera.setPosition(_player.getPos());
	}
	if (_inputManager.isKeyPressed(SDLK_SPACE) || _inputManager.isKeyPressed(SDLK_ESCAPE) || _inputManager.isKeyPressed(SDLK_RETURN))
		_gameState = GameState::EXIT;
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
		_zombies[i].update();
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
		_humans[i].update();
		_humans[i].draw(_spriteBatch);
	}
	//system("pause");


	_player.draw(_spriteBatch);




	_spriteBatch.end();
	_spriteBatch.renderBatch();
	glBindTexture(GL_TEXTURE_2D, 0);
	_shader.unuse();
}
