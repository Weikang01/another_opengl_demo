#pragma once

namespace test_Engine
{
	class FPSLimiter
	{
	public:
		FPSLimiter();
		~FPSLimiter();

		void init(float maxFPS);

		void setMaxFPS(float maxFPS);

		void begin();
		void end();

		float calculateFPS();
		float getDeltaTime();
	private:
		float _maxFPS, _frameTime;
		unsigned int _startTicks;
	};
}