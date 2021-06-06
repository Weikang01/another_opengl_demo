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
	void InputManager::update()
	{
		for (auto& it : _keyMap)
			_previousKeyMap[it.first] = it.second;
	}
	void InputManager::pressKey(const unsigned int keyID)
	{
		_keyMap[keyID] = true;
	}
	void InputManager::releaseKey(const unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}

	/// returns true if the key helds down
	bool InputManager::isKeyDown(const unsigned int keyID)
	{
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end())
			return it->second;
		return false;
	}

	/// returns true if the key was just pressed
	bool InputManager::isKeyPressed(const unsigned int keyID)
	{
		return isKeyDown(keyID) && !wasKeyDown(keyID);
	}
	void InputManager::setMouseCoord(const float x, const float y)
	{
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}
	bool InputManager::wasKeyDown(const unsigned int keyID)
	{
		auto it = _previousKeyMap.find(keyID);
		if (it != _previousKeyMap.end())
			return it->second;
		return false;
	}
}