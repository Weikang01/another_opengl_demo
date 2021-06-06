#include "Level.h"
#include <test_Engine/engineErrors.h>
#include <fstream>
#include <iostream>
#include <test_Engine/ResourceManager.h>

using test_Engine::ResourceManager;


using test_Engine::fatalError;
Level::Level(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName);

	if (file.fail())
		fatalError("cannot open level file: " + fileName);
	std::string tmp;
	file >> tmp >> _nrHumans;

	std::getline(file, tmp);
	while (std::getline(file, tmp))
		_levelData.push_back(tmp);

	_spriteBatch.init();

	_spriteBatch.begin();

	glm::vec4 uv(0.f, 0.f, 1.f, 1.f);
	for (unsigned int col = 0; col < _levelData.size(); col++)
	{
		for (unsigned int row = 0; row < _levelData[col].size(); row++)
		{
			char value = _levelData[col][row];
			
			switch (value)
			{
			case 'R':
				_spriteBatch.draw(
					glm::vec4(row * TILE_WIDTH, col * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH),
					uv, ResourceManager::getTexture("Textures/boxes/box1.png").id,
					0.f, glm::vec4(255, 255, 255, 255));
				break;
			case 'L':
				_spriteBatch.draw(
					glm::vec4(row * TILE_WIDTH, col * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH),
					uv, ResourceManager::getTexture("Textures/boxes/box4.png").id,
					0.f, glm::vec4(255, 255, 255, 255));
				break;
			case 'G':
				_spriteBatch.draw(
					glm::vec4(row * TILE_WIDTH, col * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH),
					uv, ResourceManager::getTexture("Textures/boxes/box2.png").id,
					0.f, glm::vec4(255, 255, 255, 255));
				break;
			case '@':
				_levelData[col][row] = '.';
				_playerStartPosition.x = row * TILE_WIDTH;
				_playerStartPosition.y = col * TILE_WIDTH;
				break;
			case 'Z':
				_levelData[col][row] = '.';
				_zombieStartPosition.emplace_back(row * TILE_WIDTH, col * TILE_WIDTH);
				break;
			default:
				break;
			}
		}
	}

	_spriteBatch.end();
}

Level::~Level()
{
}

void Level::draw()
{
	_spriteBatch.renderBatch();
}
