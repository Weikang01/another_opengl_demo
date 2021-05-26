#pragma once

#include <test_Engine/Window.h>
#include <test_Engine/Shader.h>
#include <test_Engine/Camera2D.h>
#include <test_Engine/InputManager.h>
#include <test_Engine/SpriteBatch.h>
#include <test_Engine/Timing.h>

#include "Unit.h"

using test_Engine::Window;
using test_Engine::Shader;
using test_Engine::Camera2D;
using test_Engine::InputManager;
using test_Engine::FPSLimiter;
using test_Engine::SpriteBatch;

enum class GameState { EXIT, PLAY };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
private:

	void initSystem();

	void gameLoop();

	void processInput();

	void drawGame();

	int _screen_width, _screen_height;

	Window _window;
	Shader _shader;
	Camera2D _camera;
	InputManager _inputManager;
	GameState _gameState;
	SpriteBatch _spriteBatch;
	FPSLimiter _fpsLimiter;

	Unit _player;
	std::vector<Human> _humans;
	std::vector<Zombie> _zombies;

	unsigned int _nrHumans, _nrZombies;
};

