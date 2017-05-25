//---------------------------------------------------------------------------------------
/**
* \file       callbacks.h
* \author     Julia Ostrokomorets <ostroiul@fit.cvut.cz>, <ecnill.j@gmail.com>
* \date       May 2017
* \brief      All program callbacks for working with display, keyboard, mouse, etc.
*
*/
//---------------------------------------------------------------------------------------
#ifndef __CALLBACKS_H
#define __CALLBACKS_H
#include "pgr.h"
#include "config_holder.h"
#include "scene.h"
using namespace glm;

#define REFRESH_INTERVAL 33

enum { MENU_LIGHTING, MENU_EXPLOSION, MENU_CAMERA_TOP, MENU_CAMERA_TRAIN, MENU_CAMERA_FREE, MENU_CAMERA_HELICOPTER, MENU_EXIT };

/** Calls all callbacks except finalize */
void manageCallbacks();		

/** Callback for drawing */
void displayCallback();
/** Callback for window reshape*/
void reshapeCallback(int newWidth, int newHeight);
/** Callback for handling pressing normal keys */
void keyboardCallback(unsigned char keyPressed, int mouseX, int mouseY);
/** Callback for handling releasing normal key */
void keyboardUpCallback(unsigned char keyPressed, int mouseX, int mouseY);
/** Callback for handling pressing special key */
void specialKeyboardCallback(int specKeyPressed, int mouseX, int mouseY);
/** Callback for handling releasing special key */
void specialKeyboardUpCallback(int specKeyPressed, int mouseX, int mouseY);
/** Callback for handling time cycle */
void timerCallback(int);
/** Callback for handling mouse */
void passiveMouseMotionCallback(int mouseX, int mouseY);
/** Callback for handling mouse wheel */
void mouseWheelCallback(int button, int dir, int x, int y);
/** Callback for handling click */
void onMouseButton(int button, int state, int x, int y);
/** Callback for handling menu */
void selectFromMenuCallback(int command);
/** Create menu items */
int buildPopupMenu(void);
/** Calls destructor of all application */
void finalizeCallback();

#endif // __CALLBACKS_H