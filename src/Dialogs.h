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

#ifndef DIALOGS_H
#define DIALOGS_H

enum
{	fire = 1,
	magic = 2,
	blunt = 4,
	sharp = 8,
	cold = 16,
	electricity = 32,
	silver = 64,
	poisonous = 128
};

enum
{	fireIcon = 132,
	magicIcon = 133,
	bluntIcon = 134,
	sharpIcon = 135,
	coldIcon = 136,
	electricityIcon = 137,
	silverIcon = 138,
	poisonousIcon = 139
};

int showAlertDialog(char *messagetxt, char *okbuttontxt, char *cancelbuttontxt);
void displayMessage(short resID, short index, short itemNum);
void displayItemMessage(short itemID);

short doStatsDialog(Boolean canDoStore);
short doItemsDialog(Boolean canDoStore);
short doStoreDialog(short param);
void gameDialog(short display, short param);

#endif

