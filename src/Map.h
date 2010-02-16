/* This file is part of Mantra.
 *
 * Copyright (C) 2009, 2010 Chris O'Neill
 *           Originally by the Syzygy Cult
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 02110-1301, USA.
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

