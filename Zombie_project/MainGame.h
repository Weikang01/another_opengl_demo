#pragma once

#include <test_Engine/Window.h>
#include <test_Engine/Shader.h>
#include <test_Engine/Camera2D.h>
#include <test_Engine/InputManager.h>
#include <test_Engine/SpriteBatch.h>
#include <test_Engine/SpriteFont.h>
#include <test_Engine/Timing.h>
#include <test_Engine/AudioEngine.h>
#include <test_Engine/ParticleEngine2D.h>
#include <test_Engine/ParticleBatch2D.h>

#include "Zombie.h"
#include "Player.h"

#include "Bullet.h"
#include "Level.h"

using test_Engine::Window;
using test_Engine::Shader;
using test_Engine::Camera2D;
using test_Engine::InputManager;
using test_Engine::FPSLimiter;
using test_Engine::SpriteBatch;
using test_Engine::SpriteFont;
using test_Engine::AudioEngine;
using test_Engine::Music;
using test_Engine::SoundEffect;
using test_Engine::ParticleEngine2D;
using test_Engine::ParticleBatch2D;

enum class GameState { EXIT, PLAY };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
private:

	void initSystem();

	void initLevel();

	void gameLoop();

	void processInput();

	void updateBullets(float deltaTime);

	void updateAgents(float deltaTime);

	void drawGame();

	// draws the HUD
	void drawHud();

	void addBlood(const glm::vec2& position, int numParticles);

	int _screen_width = 1024;
	int _screen_height = 768;

	Window _window;
	Shader _shader;
	Camera2D _camera;
	Camera2D _hudCamera;
	InputManager _inputManager;
	GameState _gameState;
	SpriteBatch _spriteBatch;
	FPSLimiter _fpsLimiter;

	Player* _player = nullptr;
	std::vector<Bullet> _bullets;
	std::vector<Human*> _humans;
	std::vector<Zombie*> _zombies;
	std::vector<Level*> _levels;
	unsigned int _curLevel;

	AudioEngine m_audioEngine;
	ParticleEngine2D m_particleEngine;
	ParticleBatch2D* m_bloodParticleBatch = nullptr;

	// font display
	SpriteBatch _hudSpriteBatch;
	SpriteFont* _spriteFont = nullptr;

	int _nrHumansKilled; ///< Humans killed by player
	int _nrZombiesKilled; ///< Zombies killed by player
};

