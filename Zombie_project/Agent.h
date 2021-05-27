#pragma once
#include <glm.hpp>
#include <test_Engine/GLTexture.h>
#include <test_Engine/SpriteBatch.h>
#include <test_Engine/GLTexture.h>
#include <SDL.h>

#include "Wall.h"

using test_Engine::SpriteBatch;
using test_Engine::GLTexture;

class Human;
class Zombie;

class Agent
{
public:
	Agent() = default;
	Agent(glm::vec2 pos, glm::vec2 size, float speed, const char* texturePath);
	~Agent() {}

	void setPos(float x, float y);
	glm::vec2 getPos() const { return glm::vec2(_pos_and_size.x, _pos_and_size.y); }
	void move(const glm::vec2& dir);
    glm::vec2 getSize() const { return glm::vec2(_pos_and_size.z, _pos_and_size.w); }

	void draw(SpriteBatch& spriteBatch) const;

protected:
    Agent(const glm::vec4 pos_and_size, const GLTexture texture, const float speed)
        :_pos_and_size(pos_and_size), _texture(texture), _speed(speed)
    {}
    glm::vec4 getPos_and_size() const { return _pos_and_size; }
	glm::vec4 _pos_and_size;
	GLTexture _texture;
	float _speed;
};
