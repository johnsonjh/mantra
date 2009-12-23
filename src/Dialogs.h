/*
 *  Dialogs.h
 *  Mantra
 *
 *  Created by Chris O'Neill on 6/17/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
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

