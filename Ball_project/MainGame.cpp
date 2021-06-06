#include "MainGame.h"
#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <algorithm> /* min(,) */
#include <test_Engine/Engine.h>

// Some helpful constant.
const float DESIRE_FPS = 60.0f; // FPS that game is designed to run at
const int MAX_PHYSICS_STEPS = 6; // Max number of physcis step per frame
const float MS_PER_SECOND = 1000; // Number of milliseconds in a second
const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRE_FPS; // The desired frame time per frame
const float MAX_DELTA_TIME = 1.0f; // Maximum size of deltaTime

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
	for (size_t i = 0; i < m_ballRenderers.size(); ++i) {
		delete m_ballRenderers[i];
	}
}

void MainGame::run()
{
	init();
	initBalls();

	// Start our previousTicks variable
	Uint32 previousTicks = SDL_GetTicks();

	while (m_gameState == GameState::RUNNING)
	{
		m_fpsLimiter.begin();
		processInput();

		Uint32 newTicks = SDL_GetTicks();
		Uint32 frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = (float)frameTime / DESIRED_FRAMETIME;

		int i = 0;

		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			update(deltaTime);
			totalDeltaTime -= deltaTime;

			++i;
		}

		m_camera.update();
		draw();
		m_fpsLimiter.end();
	}
}

void MainGame::init()
{
	test_Engine::init();
	m_audioEngine.init();
	test_Engine::Music music = m_audioEngine.loadMusic("");
	music.play();

	m_window.create("Ball",
		m_screenWidth, m_screenHeight, test_Engine::WindowFlags::WINDOW);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_camera.init(m_screenWidth, m_screenHeight);

	m_camera.setPosition(
		glm::vec2(m_screenWidth / 2.0f, m_screenHeight / 2.0f));

	m_spriteBatch.init();
	m_spriteFont = std::make_unique<test_Engine::SpriteFont>("", 40);

	m_shader.compile("Shaders/textureShading.vert", "Shaders/textureShading.frag");

	m_fpsLimiter.setMaxFPS(DESIRE_FPS);
}

struct BallSpawn {
	BallSpawn(const test_Engine::ColorRGBA8& colr,
		float rad, float m, float minSpeed,
		float maxSpeed, float prob) :
		color(colr),
		radius(rad),
		mass(m),
		randSpeed(minSpeed, maxSpeed),
		probability(prob) // probability that the ball spawns
	{} 
	
	test_Engine::ColorRGBA8 color;
	float radius;
	float mass;
	float probability;
	std::uniform_real_distribution<float> randSpeed;
};

void MainGame::initBalls()
{
}

void MainGame::processInput()
{
	m_inputManager.update();

	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			m_ballController.onMouseMove(m_balls, (float)evnt.motion.x, (float)m_screenHeight - (float)evnt.motion.y);
			m_inputManager.setMouseCoord((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_ballController.onMouseDown(m_balls, (float)evnt.motion.x, (float)m_screenHeight - (float)evnt.motion.y);
			m_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_ballController.onMouseUp(m_balls);
			m_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}

	if (m_inputManager.isKeyPressed(SDLK_ESCAPE)) {
		m_gameState = GameState::EXIT;
	}
	// Handle gravity changes
	if (m_inputManager.isKeyPressed(SDLK_LEFT)) {
		m_ballController.setGravityDirection(GravityDirection::LEFT);
	}
	else if (m_inputManager.isKeyPressed(SDLK_RIGHT)) {
		m_ballController.setGravityDirection(GravityDirection::RIGHT);
	}
	else if (m_inputManager.isKeyPressed(SDLK_UP)) {
		m_ballController.setGravityDirection(GravityDirection::UP);
	}
	else if (m_inputManager.isKeyPressed(SDLK_DOWN)) {
		m_ballController.setGravityDirection(GravityDirection::DOWN);
	}
	else if (m_inputManager.isKeyPressed(SDLK_SPACE)) {
		m_ballController.setGravityDirection(GravityDirection::NONE);
	}

	if (m_inputManager.isKeyPressed(SDLK_1)) {
		++m_currentRenderer;
		if (m_currentRenderer >= (int)m_ballRenderers.size()) {
			m_currentRenderer = 0;
		}
	}
	if (m_inputManager.isKeyPressed(SDLK_2)) {
		if (m_ballController.getIsColorChangingOn_Collision()) {
			m_ballController.setIsColorChangingOn_Collision(false);
		}
		else {
			m_ballController.setIsColorChangingOn_Collision(true);
		}
	}
}

void MainGame::update(float deltaTime)
{

#define ADD_BALL(p, ...)\
	totalProbability += p;\
	possibleBalls.emplace_back(__VA_ARGS__);

	std::mt19937 randomEngine((unsigned int)time(nullptr));
	std::uniform_real_distribution<float> randX(0.0f, (float)m_screenWidth);
	std::uniform_real_distribution<float> randY(0.0f, (float)m_screenHeight);
	std::uniform_real_distribution<float> randDir(-1.0f, 1.0);

	// Add all possible balls
	std::vector<BallSpawn> possibleBalls;
	float totalProbability = 0.0f;

	std::uniform_real_distribution<float> r1(2.0f, 6.0f);
	std::uniform_int_distribution<int> r2(0, 255);

	const int NUM_BALLS = 20000;

	ADD_BALL(1.0f, test_Engine::ColorRGBA8(255, 255, 255, 255),
		2.0f, 1.0f, 0.1f, 7.0f, totalProbability);
	ADD_BALL(1.0f, test_Engine::ColorRGBA8(1, 254, 145, 255),
		2.0f, 2.0f, 0.1f, 3.0f, totalProbability);
	ADD_BALL(1.0f, test_Engine::ColorRGBA8(177, 0, 254, 255),
		3.0f, 4.0f, 0.0f, 0.0f, totalProbability);
	ADD_BALL(1.0f, test_Engine::ColorRGBA8(254, 0, 0, 255),
		3.0f, 4.0f, 0.0f, 0.0f, totalProbability);
	ADD_BALL(1.0f, test_Engine::ColorRGBA8(0, 255, 255, 255),
		3.0f, 4.0f, 0.0f, 0.0f, totalProbability);
	ADD_BALL(1.0f, test_Engine::ColorRGBA8(255, 255, 0, 255),
		3.0f, 4.0f, 0.0f, 0.0f, totalProbability);

	//// Make a bunch of random ball types
	for (int i = 0; i < 10000; ++i) {
		ADD_BALL(1.0f, test_Engine::ColorRGBA8(r2(randomEngine), r2(randomEngine), r2(randomEngine), 255), r1(randomEngine), r1(randomEngine), 0.0f, 0.0f, totalProbability);
	}
}

void MainGame::draw()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);

	glm::mat4 projectionMatrix = m_camera.getCameraMat();

	m_ballRenderers[m_currentRenderer]->renderBalls(m_spriteBatch, m_balls, projectionMatrix);

	m_shader.use();
	m_shader.setInt("mySampler", 0);
	m_shader.setMat4fv("P", projectionMatrix);

	drawHud();

	m_shader.unuse();
	m_window.swapBuffer();
}

void MainGame::drawHud()
{
	const test_Engine::ColorRGBA8 fontColor(255, 0, 0, 255);
	char buffer[64];
	sprintf(buffer, "%.1f", m_fps);

	m_spriteBatch.begin();
	m_spriteFont->draw(m_spriteBatch, buffer, glm::vec2(0.0f, m_screenHeight - 50.0f),
		glm::vec2(1.0), 0.0f, fontColor);
	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
}