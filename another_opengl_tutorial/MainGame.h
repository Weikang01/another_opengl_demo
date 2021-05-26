#pragma once
#include <iostream>
#include <Windows.h>

#include <test_Engine/Window.h>
#include <test_Engine/Sprite.h>
#include <test_Engine/Shader.h>
#include <test_Engine/GLTexture.h>
#include <test_Engine/Camera2D.h>
#include <test_Engine/SpriteBatch.h>
#include <test_Engine/InputManager.h>
#include <test_Engine/Timing.h>

#include "Bullet.h"

enum class GameState {PLAY, EXIT};
using test_Engine::Window;
using test_Engine::GLTexture;
using test_Engine::Shader;
using test_Engine::Camera2D;
using test_Engine::SpriteBatch;
using test_Engine::InputManager;
using test_Engine::FPSLimiter;

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
private:
	void initSystems();
	void gameLoop();
	void drawGame();
	float _maxFPS, _fps;

	Window _window;
	void processInput();

	int _screen_width, _screen_height;

	std::vector<Bullet> _bullets;

	GameState _gameState;

	SpriteBatch _spriteBatch;

	InputManager _inputManager;

	FPSLimiter _fpsLimiter;

	Shader _shader;
	GLTexture _texture;
	Camera2D _camera;

	float _time;
};

