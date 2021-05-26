#pragma once
#include <unordered_map>
#include <glm.hpp>

namespace test_Engine
{
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void pressKey(const unsigned int keyID);
		void releaseKey(const unsigned int keyID);

		bool isKeyPressed(const unsigned int keyID);

		void setMouseCoord(const float x, const float y);
		glm::vec2 getMouseCoord() const { return _mouseCoords; }
	private:
		std::unordered_map<unsigned int, bool> _keyMap;
		glm::vec2 _mouseCoords;
	};
}
