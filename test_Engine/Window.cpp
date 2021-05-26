#include "Window.h"
#include "Errors.h"

namespace test_Engine
{
	Window::Window()
		:_window(nullptr), _width(0), _height(0)
	{
	}

	Window::~Window()
	{
		SDL_DestroyWindow(_window);
	}

	int Window::create(const char* title, const int width, const int height, WindowFlags currentFlags)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;
		if (currentFlags & INVISIBLE)
			flags |= SDL_WINDOW_HIDDEN;
		if (currentFlags & FULLSCREEN)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		if (currentFlags & BORDERLESS)
			flags |= SDL_WINDOW_BORDERLESS;

		_width = width;
		_height = height;
		_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, flags);
		if (_window == nullptr) fatalError("SDL window could not be created!");

		SDL_GLContext context = SDL_GL_CreateContext(_window);
		if (context == nullptr) fatalError("SDL context could not be created!");

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// glew
		glewExperimental = GL_TRUE;

		GLenum error = glewInit();
		glCheckError();
		if (error != GLEW_OK) fatalError("GLEW could not be initialized!");

		glClearColor(0.1f, 0.1f, 0.1f, 1.f);


		std::printf("***  OpenGL Version: %s  ***\n", glGetString(GL_VERSION));

		// set vsync
		SDL_GL_SetSwapInterval(0);

		// Enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swapBuffer()
	{
		SDL_GL_SwapWindow(_window);
	}
}