#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

namespace test_Engine
{
	TextureCache::TextureCache()
	{
	}

	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(const char* texturePath)
	{
		auto iter = _textureMap.find(texturePath);

		if (iter == _textureMap.end())
		{
			GLTexture newTexture = ImageLoader::loadImage(texturePath);
			_textureMap.insert(std::make_pair(texturePath, newTexture));

			//std::cout << "Used new texture! :" << texturePath << std::endl;
			return newTexture;
		}

		//std::cout << "Used cached texture!" << std::endl;
		return iter->second;
	}
}