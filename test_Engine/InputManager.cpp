#include "InputManager.h"

namespace test_Engine
{
	InputManager::InputManager()
		:_mouseCoords(0.f)
	{
	}
	InputManager::~InputManager()
	{
	}
	void InputManager::pressKey(const unsigned int keyID)
	{
		_keyMap[keyID] = true;
	}
	void InputManager::releaseKey(const unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}
	bool InputManager::isKeyPressed(const unsigned int keyID)
	{
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end())
			return it->second;
		return false;
	}
	void InputManager::setMouseCoord(const float x, const float y)
	{
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}
}