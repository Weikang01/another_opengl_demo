#include "Timing.h"
#include <SDL.h>
#include <utility>

namespace test_Engine
{
	FPSLimiter::FPSLimiter()
		:_maxFPS(0.f), _frameTime(0.f)
	{
	}

	FPSLimiter::~FPSLimiter()
	{
	}
	
	void FPSLimiter::init(float maxFPS)
	{
		setMaxFPS(maxFPS);
	}

	void FPSLimiter::setMaxFPS(float maxFPS)
	{
		_maxFPS = maxFPS;
	}
	
	void FPSLimiter::begin()
	{
		_startTicks = SDL_GetTicks();
	}
	
	void FPSLimiter::end()
	{
		float frameTicks = SDL_GetTicks() - _startTicks;
		if (1000.f / _maxFPS > frameTicks) {
			SDL_Delay(1000.f / _maxFPS - frameTicks);
		}
	}

	float FPSLimiter::calculateFPS()
	{
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static float prevTick = SDL_GetTicks();
		static int curFrame = 0;
		float curTick = SDL_GetTicks();

		_frameTime = curTick - prevTick;
		frameTimes[curFrame % NUM_SAMPLES] = _frameTime;

		curFrame++;
		prevTick = curTick;

		int count = std::min(curFrame, NUM_SAMPLES);
		float frameTimeAverage = 0;
		for (unsigned int i = 0; i < count; i++)
			frameTimeAverage += frameTimes[i];
		frameTimeAverage /= count;
		float fps = frameTimeAverage > 0.f ? 1000.f / frameTimeAverage : 60.f;
		return fps;
	}
}