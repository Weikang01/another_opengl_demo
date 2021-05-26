#include "Camera2D.h"

namespace test_Engine
{
	Camera2D::Camera2D()
		:_position(0.f, 0.f), _cameraMat(1.f), _orthoMat(1.f), _scale(1.f),
		_needsMatUpdate(true), _screen_width(0), _screen_height(0)
	{
	}

	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(const int screen_width, const int screen_height)
	{
		_screen_width = screen_width;
		_screen_height = screen_height;
		_orthoMat = glm::ortho(0.f, (float)_screen_width, 0.f, (float)_screen_height);
	}

	void Camera2D::update()
	{
		if (_needsMatUpdate)
		{
			glm::vec3 translate(_screen_width / 2.f - _position.x, _screen_height / 2.f - _position.y, 0.f);
			_cameraMat = glm::translate(_orthoMat, translate);
			
			glm::vec3 scale(_scale, _scale, 0.f);
			_cameraMat = glm::scale(glm::mat4(1.f), scale) * _cameraMat;

			_needsMatUpdate = false;
		}
	}
	glm::vec2 Camera2D::screenToWorld(glm::vec2 screenCoords)
	{
		screenCoords.y = _screen_height - screenCoords.y;
		screenCoords -= glm::vec2(_screen_width / 2.f, _screen_height / 2.f);
		// Scale the coord
		screenCoords /= _scale;
		// Translate with the camera position
		screenCoords += _position;
		return screenCoords;
	}
}