#include "Agent.h"
#include "Level.h"

#include<iostream>
#include <test_Engine/ResourceManager.h>
#include <algorithm>

Agent::Agent(glm::vec2 pos, float speed, int health, const char* texturePath)
	:_pos(pos), _speed(speed), _health(health)
{
	_texture =
		test_Engine::ResourceManager::getTexture(texturePath);
}


bool Agent::collideWithLevel(const std::vector<std::string>& levelData)
{
	std::vector<glm::vec2> collideTilePosition;

	checkTilePositions(levelData, collideTilePosition, _pos.x, _pos.y);
	checkTilePositions(levelData, collideTilePosition, _pos.x + AGENT_WIDTH, _pos.y);
	checkTilePositions(levelData, collideTilePosition, _pos.x, _pos.y + AGENT_WIDTH);
	checkTilePositions(levelData, collideTilePosition, _pos.x + AGENT_WIDTH, _pos.y + AGENT_WIDTH);

	if (collideTilePosition.size() == 0)
		return false;
	for (int i = 0; i < collideTilePosition.size(); i++)
		collideWithTile(collideTilePosition[i]);
	return true;
}

bool Agent::collideWithAgent(Agent* agent)
{
	glm::vec2 diff = agent->_pos - this->_pos;
	if (diff.x * diff.x + diff.y * diff.y < AGENT_WIDTH * AGENT_WIDTH)
	{
		glm::vec2 collisionDepthVec = glm::normalize(diff) * (AGENT_WIDTH - glm::length(diff));
		this->_pos.x  -= .5f * collisionDepthVec.x;
		this->_pos.y  -= .5f * collisionDepthVec.y;
		agent->_pos.x += .5f * collisionDepthVec.x;
		agent->_pos.y += .5f * collisionDepthVec.y;
		return true;
	}
	return false;
}

void Agent::draw(SpriteBatch& spriteBatch) const
{
	glm::vec4 uv(0.f, 0.f, 1.f, 1.f);
	spriteBatch.draw(glm::vec4(_pos.x, _pos.y, AGENT_WIDTH, AGENT_WIDTH), uv,
		_texture.id, 0.f, glm::vec4(255, 255, 255, 255));
}

void Agent::checkTilePositions(const std::vector<std::string>& levelData,
	std::vector<glm::vec2>& collideTilePosition, float x, float y)
{
	unsigned int cornerX = floor(x / TILE_WIDTH);
	unsigned int cornerY = floor(y / TILE_WIDTH);

	glm::vec2 cornerPos = glm::vec2(static_cast<float>(cornerX * TILE_WIDTH), static_cast<float>(cornerY * TILE_WIDTH));

	glm::vec2 tilePos = cornerPos + glm::vec2(TILE_WIDTH * .5f);

	if (cornerY < 0 || cornerX < 0 || cornerY >= levelData.size() || cornerX >= levelData[0].size())
		return;

	for (size_t i = 0; i < collideTilePosition.size(); i++)
		if (collideTilePosition[i].x == tilePos.x && collideTilePosition[i].y == tilePos.y)
			return;
	if (levelData[cornerY][cornerX] != '.')
		collideTilePosition.push_back(tilePos);
}

// AABB collision
void Agent::collideWithTile(glm::vec2 tilePos)
{
	const float AGENT_RADIUS = AGENT_WIDTH / 2.f;
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPos = _pos + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPlayerPos - tilePos;

	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);


	if (xDepth > 0 || yDepth > 0)
	{
		if (std::max(xDepth, 0.f) < std::max(yDepth, 0.f))
			if (distVec.x < 0)
				_pos.x -= xDepth;
			else
				_pos.x += xDepth;
		else
			if (distVec.y < 0)
				_pos.y -= yDepth;
			else
				_pos.y += yDepth;
	}
}

