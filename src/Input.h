/*
 *  Keys.h
 *  Mantra
 *
 *  Created by Chris O'Neill on 6/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INPUT_H
#define INPUT_H

// generic defines and functions
enum {
	INPUT_KEY = 1,
	INPUT_MOUSE_POSITION = 2,
	INPUT_MOUSE_BUTTONS = 4,
	INPUT_MOUSE_WHEEL = 8,
};

void initInput(void);

void waitForSpecificInput(int thingsToWaitFor);
void waitForAnyInput(void);

// keyboard routines
int pollKeys(void);
int keyDown(int key);
int keyUp(int key);

int keyPressed(int key);
int keyReleased(int key);
int anyKeyPressed(void);          // where is the any key?  I see esk, cutarl, and pigup...
int anyKeyReleased(void);         // oh well, I better order a tab...
int keyHeld(int key);         // wait, no time now, the computer's starting!

void waitForKeyPressed(void);
void waitForKeyTap(void);

// mouse routines
int pollMouseButtons(void);
int pollMousePosition(void);
int pollMouseScrollWheel(void);

int mouseButtonDown(int button);
int mouseButtonUp(int button);

int mouseButtonPressed(int button);
int mouseButtonReleased(int button);
int anyMouseButtonPressed(void);
int anyMouseButtonReleased(void);
int mouseButtonHeld(int button);
int mouseMoved(void);

int currentMouseX(void);
int currentMouseY(void);

int mousePositionInRect(int x1, int y1, int x2, int y2);

// Mantra specific routines
int checkKeys(void);

#endif

