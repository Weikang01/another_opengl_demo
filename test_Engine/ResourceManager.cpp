#include "ResourceManager.h"

namespace test_Engine
{
	TextureCache ResourceManager::_textureCache;

	GLTexture ResourceManager::getTexture(const char* texturePath)
	{
		return _textureCache.getTexture(texturePath);
	}
}