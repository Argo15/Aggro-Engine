#include "MouseState.hpp"

MouseState::MouseState()
	: m_sButtonPressed()
	, m_sButtonClicked()
	, m_nPosX(-1)
	, m_nPosY(-1)
	, m_nDeltaX(0)
	, m_nDeltaY(0)
	, m_nScroll(0)
{

}

void MouseState::tick()
{
	//m_nDeltaX = 0;
	//m_nDeltaY = 0;
}

void MouseState::setButtonPressed(int button, bool bIsPressed)
{
	if (bIsPressed)
	{
		m_sButtonPressed.insert(button);
		m_sButtonClicked.insert(button);
	}
	else
	{
		m_sButtonPressed.erase(button);
		m_sButtonClicked.erase(button);
	}
}

void MouseState::setPosition(int nPosX, int nPosY)
{
	int nOldX = m_nPosX;
	int nOldY = m_nPosY;
	m_nPosX = nPosX;
	m_nPosY = nPosY;

	if (nOldX > -1 && nOldY > -1)
	{
		m_nDeltaX = m_nPosX-nOldX;
		m_nDeltaY = m_nPosY-nOldY;
	}
	else
	{
		m_nDeltaX = 0;
		m_nDeltaY = 0;
	}
}

void MouseState::setScroll(int nScroll)
{
	m_nScroll = nScroll;
}

bool MouseState::getButtonPressed(int button)
{
	return m_sButtonPressed.find(button) != m_sButtonPressed.end();
}

bool MouseState::getButtonPressedOnce(int button)
{
	bool bIsPressed = m_sButtonClicked.find(button) != m_sButtonClicked.end();
	m_sButtonClicked.erase(button);
	return bIsPressed;
}

int MouseState::getPosX()
{
	return m_nPosX;
}

int MouseState::getPosY()
{
	return m_nPosY;
}

int MouseState::getDeltaX()
{
	return m_nDeltaX;
}

int MouseState::getDeltaY()
{
	return m_nDeltaY;
}

int MouseState::getScroll()
{
	int nScroll = m_nScroll;
	m_nScroll = 0;
	return nScroll;
}