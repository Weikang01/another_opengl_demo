#pragma once
#include "GLTexture.h"

namespace test_Engine
{
	class ImageLoader
	{
	public:
		static GLTexture loadImage(const GLchar* path);
	};
}

