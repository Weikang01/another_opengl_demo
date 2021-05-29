#pragma once
#include <glm.hpp>
#include <test_Engine/GLTexture.h>
#include <test_Engine/SpriteBatch.h>
#include <test_Engine/GLTexture.h>

#include <SDL.h>

constexpr float AGENT_WIDTH = 30.f;
using test_Engine::SpriteBatch;
using test_Engine::GLTexture;

class Human;
class Zombie;

class Agent
{
public:
	Agent() = default;
	Agent(glm::vec2 pos, float speed, int health, const char* texturePath);
	virtual ~Agent() {}

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans, std::vector<Zombie*>& zombies) = 0;


	void draw(SpriteBatch& spriteBatch) const;

	bool collideWithLevel(const std::vector<std::string>& levelData);
	bool collideWithAgent(Agent* agent);

	glm::vec2 getPos() const { return _pos; }
	int _health;
protected:
	void checkTilePositions(const std::vector<std::string>& levelData,
		std::vector<glm::vec2>& collideTilePosition, float x, float y);

	void collideWithTile(glm::vec2 tilePos);

	glm::vec2 _pos;
	float _speed;
	GLTexture _texture;
};
