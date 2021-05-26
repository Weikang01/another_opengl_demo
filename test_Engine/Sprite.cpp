#include "Sprite.h"
#include "Vertex.h"

#include <cstddef>
#include <iostream>

#include "ResourceManager.h"

namespace test_Engine
{
	Sprite::Sprite()
		:_x(0), _y(0), _width(0), _height(0), _texture()
	{
	}

	Sprite::~Sprite()
	{
		//if (_VBO)
		//	glDeleteBuffers(1, &_VBO);
	}

	void Sprite::init(float x, float y, float width, float height, const char* texturePath)
	{
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		_texture = ResourceManager::getTexture(texturePath);

		vertices = {
			// _position            // color             // texCoord
			Vertex{x + width, y + height, 255,   0,   0, 255,  1.f, 1.f},
			Vertex{x,         y + height,   0, 255, 255, 255,  0.f, 1.f},
			Vertex{x,         y,          255, 255, 255, 255,  0.f, 0.f},
			Vertex{x,         y,            0, 255, 255, 255,  0.f, 0.f},
			Vertex{x + width, y,          255,   0,   0, 255,  1.f, 0.f},
			Vertex{x + width, y + height,   0, 255, 255, 255,  1.f, 1.f}
		};


		//if (!_VBO)
		//	glGenBuffers(1, &_VBO);

		//glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}