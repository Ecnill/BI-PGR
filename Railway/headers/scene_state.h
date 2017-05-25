//---------------------------------------------------------------------------------------
/**
* \file       scene_state.h
* \author     Julia Ostrokomorets <ostroiul@fit.cvut.cz>, <ecnill.j@gmail.com>
* \date       May 2017
* \brief	  Header file and implementation of Scene State class. 
*
*/
//---------------------------------------------------------------------------------------
#ifndef __SCENE_STATE_H
#define __SCENE_STATE_H
#include <string>
#include <sstream>   
#include <iomanip>
#include "pgr.h"

enum { KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_SPACE, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEYS_COUNT };

struct SceneState {
	
	int		windowWidth;				// set by reshape
	int		windowHeight;				// set by reshape
	bool	freeCameraMode;
	bool	keyMap[KEYS_COUNT];
	float	elapsedTime;
	int		frames;
	int		currentTime;
	int		timeBase;

	SceneState();
	void setKeysStateUnpressed();
	void setWindowTitleTime(std::string mainTitle, int time);
};

#endif // __SCENE_STATE_H