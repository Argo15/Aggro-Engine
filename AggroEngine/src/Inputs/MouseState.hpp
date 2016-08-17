#ifndef MOUSESTATE_HPP
#define MOUSESTATE_HPP

#include <boost/unordered_set.hpp>

/**
 * Belief state of the mouse device.
 *
 * author: wcrane
 * since: 2013-12-21
**/
class MouseState
{
private:
	boost::unordered_set<int> m_sButtonPressed; // set of buttons pressed
	boost::unordered_set<int> m_sButtonClicked; // removes buttons after a get.

	int m_nPosX, m_nPosY;
	int m_nDeltaX, m_nDeltaY;
	int m_nScroll;

public:
	MouseState();

	void tick(); // Called once per frame

	void setButtonPressed(int button, bool bIsPressed);
	void setPosition(int nPosX, int nPosY);
	void setScroll(int nScroll);

	bool getButtonPressed(int button); // is button currently down?
	bool getButtonPressedOnce(int button); // will only return true once until the same button is pressed again
	int getPosX();
	int getPosY();
	int getDeltaX();
	int getDeltaY();
	int getScroll(); // sets scroll to 0 after
};

#endif