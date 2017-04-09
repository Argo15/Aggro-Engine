#pragma once

#include <boost/unordered_set.hpp>

typedef boost::unordered_set<std::string, int> map;

/**
 * Belief state of the keyboard device. 
 *
 * author: wcrane
 * since: 2013-12-21
**/
class KeyboardState
{
private:
	boost::unordered_set<int> m_sKeyPressed; // set of buttons pressed
	boost::unordered_set<int> m_sKeyTapped; // removes buttons after a get.

public:
	KeyboardState();

	void setKeyPressed(int key, bool bIsPressed);
	bool getKeyPressed(int key); // is button currently down?
	bool getKeyPressedOnce(int key); // will only return true once until the same button is pressed again
};