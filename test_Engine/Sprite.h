#pragma once
#include "Shader.h"
#include <glew.h>
#include "GLTexture.h"
#include "Vertex.h"

namespace test_Engine
{
	class Sprite
	{
	public:
		vector<Vertex> vertices;

		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, const char* texturePath);
	private:
		GLTexture _texture;
		float _x, _y, _width, _height;
		//GLuint _VBO;
	};
}
