#pragma once

#include <test_Engine/Window.h>
#include <test_Engine/Shader.h>
#include <test_Engine/Camera2D.h>
#include <test_Engine/InputManager.h>
#include <test_Engine/SpriteBatch.h>
#include <test_Engine/Timing.h>

#include "Agent.h"
#include "Zombie.h"
#include "Human.h"
#include "Wall.h"
#include "Bullet.h"
#include "Level.h"

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

	Agent _player;
	std::vector<Bullet> _bullets;
	std::vector<Wall> _walls;
	std::vector<Human> _humans;
	std::vector<Zombie> _zombies;
	std::vector<Level*> _levels;

	unsigned int _nrHumans, _nrZombies, _nrWalls;
};

