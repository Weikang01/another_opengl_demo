#include "MainGame.h"
#include <test_Engine/Engine.h>
#include <test_Engine/Errors.h>
#include <test_Engine/ResourceManager.h>


MainGame::MainGame()
	:_window(), _screen_width(800), _screen_height(600),
	_gameState(GameState::PLAY), _shader(), _time(0),
	_texture(), _maxFPS(60.f), _fps(0.f)
{
	_camera.init(_screen_width, _screen_height);
}

MainGame::~MainGame()
{
	SDL_Quit();
}

void MainGame::run()
{
	initSystems();

	_shader.compile("Shaders/basicColor_vert.glsl", "Shaders/basicColor_frag.glsl");

	gameLoop();
}

void MainGame::initSystems()
{
	test_Engine::init();

	_window.create("another opengl tutorial", _screen_width, _screen_height, test_Engine::WINDOW);

	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		// used for frame time measuring

		_fpsLimiter.begin();


		processInput();
		
		//_fps = _fpsLimiter.calculateFPS();
		//static int frameCounter = 0;
		//if (!(frameCounter++ % 10))
		//	std::cout << _fps << std::endl;

		_camera.update();
		for (size_t i = 0; i < _bullets.size();)
		{
			if (!_bullets[i].update())
			{
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			else  i++;
		}

		drawGame();
		
		
		
		
		// limit fps
		_fpsLimiter.end();

		_window.swapBuffer();
	}
}

void MainGame::drawGame()
{
	//glClearDepth(1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shader.use();

	glActiveTexture(GL_TEXTURE0);
	_shader.setInt("diffuse_texture", 0);
	
	_shader.setMat4fv("P", _camera.getCameraMat());

	_spriteBatch.begin();
	
	glm::vec4 pos(0.f, 0.f, 50.f, 50.f);
	glm::vec4 uv(0.f, 0.f, 1.f, 1.f);
	static GLTexture texture = 
		test_Engine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterLeft_Jump.png");

	_spriteBatch.draw(pos, uv, texture.id, 0.f, glm::vec4(255, 255, 255, 255));

	for (unsigned int i = 0; i < _bullets.size(); i++)
		_bullets[i].draw(_spriteBatch);


	_spriteBatch.end();

	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_shader.unuse();
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
			break;
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
	if (_inputManager.isKeyDown(SDLK_w))
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.f, CAM_SPEED));
	if (_inputManager.isKeyDown(SDLK_s))
		_camera.setPosition(_camera.getPosition() - glm::vec2(0.f, CAM_SPEED));
	if (_inputManager.isKeyDown(SDLK_d))
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAM_SPEED, 0.f));
	if (_inputManager.isKeyDown(SDLK_a))
		_camera.setPosition(_camera.getPosition() - glm::vec2(CAM_SPEED, 0.f));
	if (_inputManager.isKeyDown(SDLK_SPACE) || _inputManager.isKeyDown(SDLK_ESCAPE) || _inputManager.isKeyDown(SDLK_RETURN))
		_gameState = GameState::EXIT;
	if (_inputManager.isKeyDown(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouseCoords = _camera.screenToWorld(_inputManager.getMouseCoord());

		glm::vec2 playerPosition(0.f);
		glm::vec2 direction = glm::normalize(mouseCoords - playerPosition);

		_bullets.emplace_back(playerPosition, glm::vec2(10.f), direction, 5.f, 120);
	}
}
