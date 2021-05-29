#pragma once

#include <string>
#include <vector>
#include <test_Engine/SpriteBatch.h>

using test_Engine::SpriteBatch;

const float TILE_WIDTH = 32.f;

class Level
{
public:
	// load the level
	Level(const std::string& fileName);
	~Level();

	void draw();

	unsigned int getNrHumans() { return _nrHumans; }

	// getters
	int getHeight() { return _levelData.size(); }
	int getWidth() { return _levelData[0].size(); }

	glm::vec2 getPlayerStartPos() const { return _playerStartPosition; }
	const SpriteBatch& getSpriteBatch() const { return _spriteBatch; }
	const std::vector<glm::vec2>& getZombieStartPos() const { return _zombieStartPosition; }

	std::vector<std::string> _levelData;
private:
	unsigned int _nrHumans;

	glm::vec2 _playerStartPosition;
	std::vector<glm::vec2> _zombieStartPosition;

	SpriteBatch _spriteBatch;
};

