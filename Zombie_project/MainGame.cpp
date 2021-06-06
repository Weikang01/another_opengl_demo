#include "MainGame.h"
#include <gtx/rotate_vector.hpp>
#include <test_Engine/Engine.h>
#include <test_Engine/engineErrors.h>
#include <iostream>
#include <random>
#include "Gun.h"

using test_Engine::fatalError;

const bool DEBUG_MODE = true;
const float PLAYER_SPEED = 4.f;
const int PLAYER_HEALTH = 100;
const float HUMAN_SPEED = 1.f;
const int HUMAN_HEALTH = 40;
const float ZOMBIE_SPEED = .7f;
const int ZOMBIE_HEALTH = 100;
const float BULLET_SPEED = 20.f;

MainGame::MainGame()
	:
	_gameState(GameState::PLAY),
	_player(), _curLevel(0), _nrHumansKilled(0), _nrZombiesKilled(0)
{
	srand(time_t());
	_camera.init(_screen_width, _screen_height);
	_hudCamera.init(_screen_width, _screen_height);
	_hudCamera.setPosition(glm::vec2(.5f * _screen_width, .5f * _screen_height));
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

	Music music = m_audioEngine.loadMusic("Sound/boss_0.ogg");
	music.play(-1);

	_shader.compile("Shaders/basicColor_vert.glsl", "Shaders/basicColor_frag.glsl");

	gameLoop();
}

void MainGame::initSystem()
{
	test_Engine::init();

	m_audioEngine.init();

	_window.create("Zombie Rush", _screen_width, _screen_height, test_Engine::WindowFlags::WINDOW);

	_spriteBatch.init();
	_hudSpriteBatch.init();

	// initialize sprite font
	_spriteFont = new SpriteFont("Fonts/ArialCE.ttf", 64);

	_fpsLimiter.init(60.f);

	m_bloodParticleBatch = new ParticleBatch2D();
	m_bloodParticleBatch->init(500, .2f, "Textures/blood/particle.png", 
		[](test_Engine::Particle2D& p, float deltaTime)
		{
			p.m_position += p.m_velocity * deltaTime;
			p.m_color.a = (GLubyte)(p.m_life * 255.f);
		});


	m_particleEngine.addParticleBatch(m_bloodParticleBatch);
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

	int playerTile_x = _levels[_curLevel]->getPlayerStartPos().x / TILE_WIDTH;
	int playerTile_y = _levels[_curLevel]->getPlayerStartPos().y / TILE_WIDTH;
	int randTile_x{};
	int randTile_y{};
	for (size_t i = 1; i < _humans.size();)
	{
		randTile_x = _randX(randomEngine);
		randTile_y = _randY(randomEngine);
		if (randTile_x != playerTile_x && randTile_y != playerTile_y)
		{
			_humans[i] =
				new Human(glm::vec2(_randX(randomEngine) * TILE_WIDTH, _randY(randomEngine) * TILE_WIDTH),
					HUMAN_SPEED, HUMAN_HEALTH, "Textures/human/human.png");
			_humans[i]->init();
			i++;
		}
	}

	for (size_t i = 0; i < _zombies.size(); i++)
		_zombies[i] = new Zombie(_levels[_curLevel]->getZombieStartPos()[i], 
			ZOMBIE_SPEED, ZOMBIE_HEALTH, "Textures/zombie/zombie.png");

	// "Textures/bullet/Bullet.png"
	_player->init(&_inputManager, &_camera, &_bullets);
	
	//                      name, fireRate,
	_player->addGun(new Gun("Magnum", 30.f,
	// bulletsPerShot, spread
		1, glm::radians(5.f),
	//projectileTexturePath, projSize
		"Textures/bullet/Bullet.png", 10.f,
	//damage, projSpeed, projLifeTime, fireSoundFX
		50, BULLET_SPEED, 30, 
		m_audioEngine.loadSoundEffect("Sound/shots/Futuristic_SMG_Single_Shot.wav")));

	//                      name, fireRate,
	_player->addGun(new Gun("Shotgun", 60.f,
	// bulletsPerShot, spread
		20, glm::radians(40.f),
	//projectileTexturePath, projSize
		"Textures/bullet/Bullet.png", 10.f,
	//damage, projSpeed, projLifeTime, fireSoundFX
		20, BULLET_SPEED, 8, 
		m_audioEngine.loadSoundEffect("Sound/shots/Futuristic_Shotgun_Single_Shot.wav")));
	
	//                      name, fireRate,
	_player->addGun(new Gun("MP5", 10.f,
	// bulletsPerShot, spread
		1, glm::radians(10.f),
	//projectileTexturePath, projSize
		"Textures/bullet/Bullet.png", 10.f,
	//damage, projSpeed, projLifeTime, fireSoundFX
		30, BULLET_SPEED, 20, 
		m_audioEngine.loadSoundEffect("Sound/shots/Futuristic_Assault_Rifle_Single_Shot_01.wav")));

}

void MainGame::gameLoop()
{
	const float DESIRED_FPS = 60.0f;
	const float MAX_DELTA_TIME = 1.0f;
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const int MAX_PHYSICS_STEPS = 6;
	float totalDeltaTime = 0.f;
	Uint32 previousTicks = SDL_GetTicks();

	while (_gameState != GameState::EXIT)
	{
		_fpsLimiter.begin();

		Uint32 newTicks = SDL_GetTicks();

		Uint32 frameTime = newTicks - previousTicks;
		previousTicks = newTicks;

		totalDeltaTime = frameTime / DESIRED_FRAMETIME;
		//std::cout << totalDeltaTime << std::endl;
		
		if (!DEBUG_MODE && !_zombies.size())
		{
			std::cout<< "YOU WIN! " << "\nZOMBIE KILLED: " << _nrZombiesKilled << "\nHUMAN KILLED: " << _nrHumansKilled
				<< "\nHUMAN REMAINED: " << _humans.size() - 1 <<"\nFINAL SCORE: " << _humans.size() * 50 - _nrHumansKilled * 20 << std::endl;
			fatalError("=================================");
		}

		processInput();

		int i = 0;
		while (totalDeltaTime > 0.f && i++ < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			updateBullets(deltaTime);
			updateAgents(deltaTime);
			m_particleEngine.update(deltaTime);

			totalDeltaTime -= deltaTime;
		}
		
		_camera.setPosition(_player->getPos());
		_camera.update();
		_hudCamera.update();

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

	_inputManager.update();

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
	if (_inputManager.isKeyDown(SDLK_LCTRL) && _inputManager.isKeyDown(SDLK_LALT) && _inputManager.isKeyDown(SDLK_a))
		_camera.screenshot("Textures/screenshot.png");

	if (_inputManager.isKeyDown(SDLK_SPACE) || _inputManager.isKeyDown(SDLK_ESCAPE) || _inputManager.isKeyDown(SDLK_RETURN))
		_gameState = GameState::EXIT;

	
}

void MainGame::updateBullets(float deltaTime)
{
	static bool getHit = false;

	for (unsigned int i = 0; i < _bullets.size(); i++)
	{
		getHit = false;
		if (!_bullets[i].update(_levels[_curLevel]->_levelData, deltaTime))
		{
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
			continue;
		}

		for (unsigned int j = 1; j < _humans.size(); j++)
		{
			if (_bullets[i].collideWithAgent(_humans[j]))
			{
				// Add blood
				addBlood(_bullets[i].getPos(), 5);
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
				// Add blood
				addBlood(_bullets[i].getPos(), 5);

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
	}
}

void MainGame::updateAgents(float deltaTime)
{
	for (unsigned int i = 0; i < _zombies.size(); i++)
	{
		for (unsigned int j = i + 1; j < _zombies.size(); j++)
			_zombies[i]->collideWithAgent(_zombies[j]);

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
		_zombies[i]->update(_levels[_curLevel]->_levelData, _humans, _zombies, deltaTime);
	}

	for (unsigned int i = 0; i < _humans.size(); i++)
	{
		for (unsigned int j = i + 1; j < _humans.size(); j++)
			_humans[i]->collideWithAgent(_humans[j]);
		_humans[i]->update(_levels[_curLevel]->_levelData, _humans, _zombies, deltaTime);
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

	for (unsigned int i = 0; i < _bullets.size(); i++)
	{
		if (_camera.isBoxInView(_bullets[i].getPos(), _bullets[i].getSize()))
			_bullets[i].draw(_spriteBatch);
	}
	for (unsigned int i = 0; i < _zombies.size(); i++)
	{
		if (_camera.isBoxInView(_zombies[i]->getPos(), glm::vec2(AGENT_WIDTH)))
			_zombies[i]->draw(_spriteBatch);
	}
	for (unsigned int i = 0; i < _humans.size(); i++)
	{
		if (_camera.isBoxInView(_humans[i]->getPos(), glm::vec2(AGENT_WIDTH)))
			_humans[i]->draw(_spriteBatch);
	}


	_spriteBatch.end();
	_spriteBatch.renderBatch();

	m_particleEngine.draw(&_spriteBatch);

	_levels[_curLevel]->draw();


	drawHud();


	glBindTexture(GL_TEXTURE_2D, 0);
	_shader.unuse();
}

void MainGame::drawHud()
{
	char buffer[256];

	_hudSpriteBatch.begin();

	sprintf_s(buffer, "#Humans %d", _humans.size());
	_spriteFont->draw(_hudSpriteBatch, buffer,
		glm::vec2(0, 0), glm::vec2(.5f),
		0.f, test_Engine::ColorRGBA8(255, 255, 255, 255),
		test_Engine::Justification::LEFT);

	sprintf_s(buffer, "#Zombies %d", _zombies.size());
	_spriteFont->draw(_hudSpriteBatch, buffer,
		glm::vec2(0, 36), glm::vec2(.5f),
		0.f, test_Engine::ColorRGBA8(255, 255, 255, 255),
		test_Engine::Justification::LEFT);

	_hudSpriteBatch.end();

	_shader.setMat4fv("P", _hudCamera.getCameraMat());
	_hudSpriteBatch.renderBatch();
}

void MainGame::addBlood(const glm::vec2& position, int numParticles)
{
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.f, 2.f * M_PI);
	glm::vec2 velocity(5.f, 0.f);

	for (unsigned int i = 0; i < numParticles; i++)
	{
		m_bloodParticleBatch->addParticle(
			position, glm::rotate(velocity, randAngle(randEngine)),
			test_Engine::ColorRGBA8(255, 0, 0, 255), 10.f);
	}
}

