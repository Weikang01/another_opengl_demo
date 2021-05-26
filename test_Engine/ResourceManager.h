#pragma once
#include "TextureCache.h"

namespace test_Engine
{
	class ResourceManager
	{
	public:
		static GLTexture getTexture(const char* texturePath);
	private:
		static TextureCache _textureCache;
	};
}
