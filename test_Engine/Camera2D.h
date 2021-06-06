#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

static constexpr float _MIN_SCALE = .1f;
static constexpr int _MIN_INTERVAL = 120;  // prevent multiple calling from screenshot
static int _frame = 0;                     // prevent multiple calling from screenshot

namespace test_Engine
{
	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(const int screen_width, const int screen_height);

		void update();

		glm::vec2 screenToWorld(glm::vec2 coords);

		bool isBoxInView(const glm::vec2& pos, const glm::vec2& size);

		void setPosition(const glm::vec2& newPos) { _needsMatUpdate = true; _position = newPos; }
		void setScale(const float newScale) { if (newScale > _MIN_SCALE) { _needsMatUpdate = true; _scale = newScale; } }

		glm::vec2 getPosition() { return _position; }
		float getScale() { return _scale; }
		glm::mat4 getCameraMat() { return _cameraMat; }

		bool screenshot(const char* filename);

	private:
		int _screen_width, _screen_height;

		bool _needsMatUpdate;

		float _scale;

		
		glm::vec2 _position;
		glm::mat4 _cameraMat, _orthoMat;
	};
}