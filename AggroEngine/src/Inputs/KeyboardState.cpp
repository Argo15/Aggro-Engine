#include "KeyboardState.hpp"

KeyboardState::KeyboardState()
{

}

void KeyboardState::setKeyPressed(int key, bool bIsPressed)
{
	if (bIsPressed)
	{
		m_sKeyPressed.insert(key);
		m_sKeyTapped.insert(key);
	}
	else
	{
		m_sKeyPressed.erase(key);
		m_sKeyTapped.erase(key);
	}
}

bool KeyboardState::getKeyPressed(int key)
{
	return m_sKeyPressed.find(key) != m_sKeyPressed.end();
}

bool KeyboardState::getKeyPressedOnce(int key)
{
	bool bIsPressed = m_sKeyTapped.find(key) != m_sKeyTapped.end();
	m_sKeyTapped.erase(key);
	return bIsPressed;
}