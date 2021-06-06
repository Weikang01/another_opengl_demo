#pragma once
#include <vector>
#include <memory>

#include "Ball.h"

#include <test_Engine/SpriteBatch.h>
#include <test_Engine/Shader.h>

using test_Engine::SpriteBatch;
using test_Engine::Shader;

class BallRenderer
{
public:
	virtual void renderBalls(SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix);
protected:
	std::unique_ptr<Shader> m_shader = nullptr;
};

