#pragma once

#include <vector>
#include <memory>

#include <test_Engine/Window.h>
#include <test_Engine/Shader.h>
#include <test_Engine/InputManager.h>
#include <test_Engine/Camera2D.h>
#include <test_Engine/SpriteBatch.h>
#include <test_Engine/SpriteFont.h>
#include <test_Engine/Timing.h>
#include <test_Engine/AudioEngine.h>

#include "Ball.h"
#include "BallController.h"
#include "BallRenderer.h"

using test_Engine::Window;
using test_Engine::Shader;
using test_Engine::InputManager;
using test_Engine::Camera2D;
using test_Engine::SpriteBatch;
using test_Engine::SpriteFont;
using test_Engine::FPSLimiter;
using test_Engine::AudioEngine;


enum class GameState {
	RUNNING,
	EXIT
};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
private:
	void init();
	void initBalls();
	void processInput();
	void update(float deltaTime);
	void draw();
	void drawHud();

	Window m_window;
	Shader m_shader;
	InputManager m_inputManager;
	Camera2D m_camera;
	SpriteBatch m_spriteBatch;
	FPSLimiter m_fpsLimiter;
	AudioEngine m_audioEngine;

	BallController m_ballController;
	std::vector<Ball> m_balls; ///< if balls were delete should be pointers, but this is more cache friendly if not
	std::vector<BallRenderer*> m_ballRenderers;
	int m_currentRenderer = 0;

	int m_screenWidth = 1024;
	int m_screenHeight = 768;
	float m_fps = 0.f;
	std::unique_ptr<SpriteFont> m_spriteFont;
	GameState m_gameState;
};


