#include "Camera2D.h"
#include <glew.h>
#include "stb_image_write.h"
#include <iostream>

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
		_frame++;
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


	bool Camera2D::isBoxInView(const glm::vec2& pos, const glm::vec2& size)
	{
		glm::vec2 scaledScreenSize = glm::vec2(_screen_width, _screen_height) / _scale;


		const float MIN_DISTANCE_X = size.x * .5f + scaledScreenSize.x * .5f;
		const float MIN_DISTANCE_Y = size.y * .5f + scaledScreenSize.y * .5f;

		glm::vec2 centerPos = pos + .5f * size;
		glm::vec2 distVec = centerPos - _position;

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		return xDepth > 0 && yDepth > 0;
	}


	bool Camera2D::screenshot(const char* filename)
	{
		if (_frame < _MIN_INTERVAL)
			return false;
		_frame = 0;

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		int x = viewport[0];
		int y = viewport[1];
		int width = viewport[2];
		int height = viewport[3];

		char* data = (char*)malloc((size_t)(width * height * 3)); // 3 components (R, G, B)

		if (!data)
			return 0;

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_flip_vertically_on_write(1);
		int saved = stbi_write_png(filename, width, height, 3, data, 0);

		free(data);

		return saved;
	}
}