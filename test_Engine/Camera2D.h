#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

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

		void setPosition(const glm::vec2& newPos) { _needsMatUpdate = true; _position = newPos; }
		void setScale(const float newScale) { _needsMatUpdate = true; _scale = newScale; }

		glm::vec2 getPosition() { return _position; }
		float getScale() { return _scale; }
		glm::mat4 getCameraMat() { return _cameraMat; }

	private:
		int _screen_width, _screen_height;

		bool _needsMatUpdate;

		float _scale;

		glm::vec2 _position;
		glm::mat4 _cameraMat, _orthoMat;
	};
}