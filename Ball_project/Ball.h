#pragma once

#include <glm.hpp>
#include <test_Engine/Vertex.h>

struct  Cell;

struct Ball
{
	Ball(float radiu, float mas, const glm::vec2& pos,
		const glm::vec2& vel, unsigned int texturId,
		const test_Engine::ColorRGBA8& colo);

	float radius;
	float mass;
	glm::vec2 velocity;
	glm::vec2 position;
	unsigned int textureID = 0;
	test_Engine::ColorRGBA8 color;
	Cell* ownerCell = nullptr; // nullptr violations are alot easier to debug than random violations memory
	int cellVectorIndex = -1;
};
