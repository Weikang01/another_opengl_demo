#pragma once
#include <map>
#include "GLTexture.h"

namespace test_Engine
{
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(const char* texturePath);

	private:
		std::map<const char*, GLTexture> _textureMap;
	};
}


