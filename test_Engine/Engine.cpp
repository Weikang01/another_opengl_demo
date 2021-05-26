#include <SDL.h>
#include <glew.h>
#include "Engine.h"
#include "stb_image.h"

namespace test_Engine
{
	int test_Engine::init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		stbi_set_flip_vertically_on_load(true);

		return 0;
	}
}