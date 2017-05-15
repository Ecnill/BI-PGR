//---------------------------------------------------------------------------------------
/**
* \file       callbacks.h
* \author     Julia Ostrokomorets
* \date       2017/05/01
* \brief      All program callbacks for working with display, keyboard, mouse, etc. 
*
*/
//---------------------------------------------------------------------------------------
#ifndef __CALLBACKS_H
#define __CALLBACKS_H

#include "scene.h"

#define TICK 33

enum {
	MENU_LIGHTING = 1, MENU_EXIT
};

void manageCallbacks();		// calls all callbacks except finalize

void displayCallback();
void reshapeCallback(int newWidth, int newHeight);

void keyboardCallback(unsigned char keyPressed, int mouseX, int mouseY);
void keyboardUpCallback(unsigned char keyPressed, int mouseX, int mouseY);
void specialKeyboardCallback(int specKeyPressed, int mouseX, int mouseY);
void specialKeyboardUpCallback(int specKeyPressed, int mouseX, int mouseY);

void timerCallback(int);

void passiveMouseMotionCallback(int mouseX, int mouseY);
void mouseWheelCallback(int button, int dir, int x, int y);

void selectFromMenuCallback(int command);
int buildPopupMenu(void);

void finalizeCallback();

#endif // __CALLBACKS_H