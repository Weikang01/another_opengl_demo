#pragma once
#include <SDL.h>
#include <glew.h>

namespace test_Engine
{
	enum WindowFlags { WINDOW = 0x0, INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	public:
		Window();
		~Window();

		int create(const char* title, const int width, const int height, WindowFlags currentFlags);

		void swapBuffer();

		int getWidth() { return _width; }
		int getHeight() { return _height; }
	private:
		SDL_Window* _window;
		int _width, _height;
	};
}
