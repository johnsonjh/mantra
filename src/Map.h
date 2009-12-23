/*
 *  Map.h
 *  Mantra
 *
 *  Created by Chris O'Neill on 1/19/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MAP_H
#define MAP_H

#include "GameConstants.h"

void initMapStuff();
void loadScreen(int index);

void drawScreenPreview(int game, BITMAP *bitmap);
void displayCurrentMapScreen();
void displayCurrentMapScreenSaric(Boolean drawSaric);
void displayCurrentMapScreenBlit(Boolean drawSaricOnScreen, Boolean drawOnScreen);

void clearWindowInwards();
void wipeWindowOutwards();

void checkForRoomChange();
void handleTheRoomChange(char direction);

#endif

