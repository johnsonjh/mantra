/*
 *  OpeningScreen.c
 *  Mantra
 *
 *  Created by Chris O'Neill on 2/20/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#if defined(MANTRA_WINDOWS)
//#include "StdAfx.h"
#include <allegro.h>
#else
#include <Allegro/allegro.h>
#endif

#include "GameTypes.h"
#include "GameDefines.h"
#include "GameConstants.h"
#include "Utils.h"
#include "Input.h"
#include "LoadData.h"
#include "Dialogs.h"
#include "Map.h"

#include "OpeningScreen.h"


int showSavedGameChoiceDialog(char *selectbuttontxt, int warn)
{
	int insideSelect, insideCancel, insideList;
	int pressedButton, pressedMouse;
	BITMAP *previews[MAX_SAVED_GAMES];
	char *timebuffs[MAX_SAVED_GAMES];
	char *namebuffs[MAX_SAVED_GAMES];
	LONG_LONG temptime;
	int hours, minutes, seconds;
	int itemindex, flag, ret, i;
	
	const int blackColor = makecol(0, 0, 0);
	const int grayColor = makecol(210, 210, 210);
	const int buttonColor = makecol(180, 180, 180);
	const int buttonPressedColor = makecol(90, 90, 90);
//	const int grayedOutBackgroundColor = makecol(140, 140, 140);
//	const int grayedOutTextColor = makecol(90, 90, 90);
	const int selectedItemColor = grayColor;
	
	const int smalltextheight = alfont_get_font_height(g_Font);
	
	const int x = (SCREEN_WIDTH - DIALOG_WIDTH) / 2;
	const int y = (SCREEN_HEIGHT - STATS_HEIGHT - DIALOG_HEIGHT) / 2;
	
	const int textborder = 15;
	const int buttonborder = 5;
	
	const char *cancelbuttontxt = "Cancel";
	
	const int selectbuttontxtwidth = alfont_text_length(g_Font, selectbuttontxt);
	const int cancelbuttontxtwidth = alfont_text_length(g_Font, cancelbuttontxt);
	
	const int buttonheight = (buttonborder * 2) + smalltextheight;
	const int buttongap = 8;
	
	const int selectbuttonwidth = (buttonborder * 2) + selectbuttontxtwidth;
	const int cancelbuttonwidth = (buttonborder * 2) + cancelbuttontxtwidth;
	
	const int buttony = y + DIALOG_HEIGHT - (2 + textborder + buttonheight);
	const int selectbuttonx = x + DIALOG_WIDTH - (2 + textborder + selectbuttonwidth);
	const int cancelbuttonx = selectbuttonx - (buttongap + cancelbuttonwidth);
	
	const int liststartx = x + textborder;
	const int liststarty = y + textborder;
	const int liststartwidth = DIALOG_WIDTH - (textborder * 2);
	const int liststartheight = DIALOG_HEIGHT - ((textborder * 2) + buttonheight + textborder);
	
	const int listitemgap = 4;
	const int listitemprevwidth = 16 * 4;
	const int listitemprevheight = 10 * 4;
	const int listitemheight = MAX(smalltextheight, listitemprevheight);
	
	const int listitemprevx = liststartx + buttonborder;
	const int listitemnamex = listitemprevx + listitemprevwidth + buttongap;
	const int listitemtimex = listitemnamex;
	
	const int listitemy = liststarty + listitemgap;
	const int listitemprevy = listitemy;
	const int listitemnamey = listitemprevy + ((listitemprevheight - (2 * smalltextheight) - buttongap) / 2);
	const int listitemtimey = listitemnamey + smalltextheight + buttongap;
	
	for(i = 0;i < MAX_SAVED_GAMES;i++)
	{
		temptime = g_SavedGames[i].time;
		
		hours = (int)(temptime / 3600);
		minutes = (int)((temptime % 3600) / 60);
		seconds = (int)(temptime % 60);
		
		timebuffs[i] = (char *)malloc(10 * sizeof(char));
		sprintf(timebuffs[i], "%02d:%02d:%02d", hours, minutes, seconds);
		
	//	printf("About to write name!\n");
		
		namebuffs[i] = (char *)malloc(256 * sizeof(char));
		memcpy(namebuffs[i], &g_SavedGames[i].name[1], g_SavedGames[i].name[0] * sizeof(char));
		namebuffs[i][(int)g_SavedGames[i].name[0]] = 0;
		
	//	printf("Wrote name!\n");
		
		previews[i] = create_bitmap(listitemprevwidth, listitemprevheight);
		drawScreenPreview(i, previews[i]);
	}
	
	// draw border
	rectfill(g_ScreenBuffer, x - 2, y - 2, x + DIALOG_WIDTH + 2, y + DIALOG_HEIGHT + 2, blackColor);
	rectfill(g_ScreenBuffer, x, y, x + DIALOG_WIDTH, y + DIALOG_HEIGHT, grayColor);
	
	// draw game list area
	rectfill(g_ScreenBuffer, liststartx - 2, liststarty - 2, liststartx + liststartwidth + 2, liststarty + liststartheight + 2, blackColor);
	rectfill(g_ScreenBuffer, liststartx, liststarty, liststartx + liststartwidth, liststarty + liststartheight, buttonColor);
	
	set_mouse_sprite(g_CursorBitmap);
	set_mouse_sprite_focus(4, 2);
	show_mouse(screen);
	
	// wait for key input
	itemindex = 0;
	
	pressedButton = 0;
	pressedMouse = 0;
	flag = 1;
	ret = 0;
	
	fadeFromBlack();
	
	for(;;)
	{
		while(flag)
		{
			// detect mouse position
			insideSelect = mousePositionInRect(selectbuttonx, buttony, selectbuttonx + selectbuttonwidth, buttony + buttonheight);
			insideCancel = mousePositionInRect(cancelbuttonx, buttony, cancelbuttonx + cancelbuttonwidth, buttony + buttonheight);
			insideList = mousePositionInRect(liststartx, liststarty, liststartx + liststartwidth, liststarty + liststartheight);
			
			// check for item change
			if (keyReleased(MANTRA_KEY_UP))
			{
				itemindex--;
				
				if (itemindex < 0)
				{
					itemindex = MAX_SAVED_GAMES - 1;
				}
			}
			else if (keyReleased(MANTRA_KEY_DOWN))
			{
				itemindex++;
				
				if (itemindex >= MAX_SAVED_GAMES)
				{
					itemindex = 0;
				}
			}
			
			// check for buttons
			if (keyPressed(MANTRA_KEY_ESC) && !pressedButton)
			{
				pressedButton = 1;
			}
			else if (keyPressed(MANTRA_KEY_ENTER) && !pressedButton)
			{
				pressedButton = 2;
			}
			else if (keyReleased(MANTRA_KEY_ESC) && pressedButton == 1)
			{
				pressedButton = 0;
				flag = 0;
				ret = 0;
			}
			else if (keyReleased(MANTRA_KEY_ENTER) && pressedButton == 2)
			{
				pressedButton = 0;
				flag = 0;
				ret = 1;
			}
			
			// ugh... need to add mouse support
			// eh, turns out its not so bad after all...
			if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideCancel && !pressedMouse)
			{
				pressedMouse = 1;
			}
			else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideSelect && !pressedMouse)
			{
				pressedMouse = 2;
			}
			else if (mouseButtonPressed(MANTRA_MOUSE_LEFT) && insideList && !pressedMouse)
			{
				pressedMouse = 3;
				
				itemindex = (currentMouseY() - (liststarty + listitemgap)) / (listitemheight + listitemgap);
				
				if (itemindex >= MAX_SAVED_GAMES)
				{
					itemindex = MAX_SAVED_GAMES - 1;
				}
				else if (itemindex < 0)
				{
					itemindex = 0;
				}
			}
			else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideCancel && pressedMouse == 1)
			{
				pressedMouse = 0;
				flag = 0;
				ret = 0;
			}
			else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideSelect && pressedMouse == 2)
			{
				pressedMouse = 0;
				flag = 0;
				ret = 1;
			}
			else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideList && pressedMouse == 3)
			{
				itemindex = (currentMouseY() - (liststarty + listitemgap)) / (listitemheight + listitemgap);
				
				if (itemindex >= MAX_SAVED_GAMES)
				{
					itemindex = MAX_SAVED_GAMES - 1;
				}
				else if (itemindex < 0)
				{
					itemindex = 0;
				}
			}
			else if (mouseButtonUp(MANTRA_MOUSE_LEFT))
			{
				pressedMouse = 0;
			}
			
			// draw the list of junk
			rectfill(g_ScreenBuffer, liststartx, liststarty, liststartx + liststartwidth, liststarty + liststartheight, buttonColor);
			
			for(i = 0;i < MAX_SAVED_GAMES;i++)
			{
				// draw selected background
				if (i == itemindex)
				{
					rectfill(g_ScreenBuffer, liststartx, listitemy + (i * (listitemheight + listitemgap)) - (listitemgap / 2), liststartx + liststartwidth, listitemy + (i * (listitemheight + listitemgap)) + listitemheight + (listitemgap / 2), selectedItemColor);
				}
				
				// draw preview
				blit(previews[i], g_ScreenBuffer, 0, 0, listitemprevx, listitemprevy + (i * (listitemheight + listitemgap)), listitemprevwidth, listitemprevheight);
				
				// draw name
				alfont_textout_ex(g_ScreenBuffer, g_Font, namebuffs[i], listitemnamex, listitemnamey + (i * (listitemheight + listitemgap)) + TEXT_HEIGHT_EXTRA, blackColor, -1);
				
				// draw time
				alfont_textout_ex(g_ScreenBuffer, g_Font, timebuffs[i], listitemtimex, listitemtimey + (i * (listitemheight + listitemgap)) + TEXT_HEIGHT_EXTRA, blackColor, -1);
			}
			
			// draw button list at the bottom
			rectfill(g_ScreenBuffer, cancelbuttonx - 2, buttony - 2, cancelbuttonx + cancelbuttonwidth + 2, buttony + buttonheight + 2, blackColor);
			rectfill(g_ScreenBuffer, cancelbuttonx, buttony, cancelbuttonx + cancelbuttonwidth, buttony + buttonheight, (pressedButton == 1 || (pressedMouse == 1 && insideCancel)) ? buttonPressedColor : buttonColor);
			alfont_textout_ex(g_ScreenBuffer, g_Font, cancelbuttontxt, cancelbuttonx + buttonborder + ((cancelbuttonwidth - (2 * buttonborder) - cancelbuttontxtwidth) / 2), buttony + buttonborder + TEXT_HEIGHT_EXTRA, blackColor, -1);
			
			rectfill(g_ScreenBuffer, selectbuttonx - 2, buttony - 2, selectbuttonx + selectbuttonwidth + 2, buttony + buttonheight + 2, blackColor);
			rectfill(g_ScreenBuffer, selectbuttonx, buttony, selectbuttonx + selectbuttonwidth, buttony + buttonheight, (pressedButton == 2 || (pressedMouse == 2 && insideSelect)) ? buttonPressedColor : buttonColor);
			alfont_textout_ex(g_ScreenBuffer, g_Font, selectbuttontxt, selectbuttonx + buttonborder + ((selectbuttonwidth - (2 * buttonborder) - selectbuttontxtwidth) / 2), buttony + buttonborder + TEXT_HEIGHT_EXTRA, blackColor, -1);
			
			drawBufferToScreen();
			
			if (flag)
			{
				waitForSpecificInput(INPUT_KEY | INPUT_MOUSE_BUTTONS | INPUT_MOUSE_POSITION);
			}
			else
			{
				rest(NUM_MILLISECONDS_BETWEEN_REFRESH * 3);
			}
		}
		
		if (ret && warn == 1)
		{
			if (!showAlertDialog("Are you sure you want to load a new game?  Your current game will be lost.", "Sure!", "NOOO!"))
			{
				ret = 0;
				flag = 1;
				continue;
			}
		}
		else if (ret && warn == 2)
		{
			if (!showAlertDialog("Are you sure you\nwant to overwrite?", "Go for it!", "Wait, NO!"))
			{
				ret = 0;
				flag = 1;
				continue;
			}
		}
		
		break;
	}
	
	fadeToBlack();
	
	show_mouse(NULL);
	
	for(i = 0;i < MAX_SAVED_GAMES;i++)
	{
		destroy_bitmap(previews[i]);
		free(namebuffs[i]);
		free(timebuffs[i]);
	}
	
	if (ret)
	{
		return itemindex;
	}
	else
	{
		return -1;
	}
}

int showOpenGameScreen()
{
	int result = showSavedGameChoiceDialog("Load", g_GameDirty != 0);
	
	if (result >= 0)
	{
		importSavedGame(result);
		
		return 1;
	}
	else
	{
		return 0;
	}
	
//	return !openGame("/Users/chris/Documents/Development/Mantra/NewSource/Saved Games/End of the Game");
}

int showSaveGameScreen()
{
//	Str255 name;
//	int result;
	int game;
	
	game = showSavedGameChoiceDialog("Save", 2);
	
	if (game >= 0)
	{
		exportSavedGame(game, g_SavedGames[game].name);
		g_GameDirty = 0;
		
		return 1;
	}
	else
	{
		return 0;
	}
	
	return 0;
}

int showOpeningScreen(int first)
{
	PALETTE tempPalette;
	
	int insideNew, insideOpen, insideSave, insideResume, insideQuit;
	int pressedButton, pressedMouse;
	int swordIndex;
	int ret, flag, fade;
	
	const int blackColor = makecol(0, 0, 0);
	const int grayColor = makecol(210, 210, 210);
	const int buttonColor = makecol(180, 180, 180);
	const int buttonPressedColor = makecol(90, 90, 90);
	const int grayedOutBackgroundColor = makecol(140, 140, 140);
	const int grayedOutTextColor = makecol(90, 90, 90);
//	const int selectedItemColor = grayColor;
	
//	const int textheight = alfont_get_font_height(g_DialogFont);
	const int smalltextheight = alfont_get_font_height(g_Font);
	
	const int buttonborderx = 25;
	const int buttonbordery = 6;
	
	const int swordimagewidth = g_SwordAnimData[0]->w;
	const int swordimageheight = g_SwordAnimData[0]->h;
	const int swordimagex = (g_ScreenBuffer->h - swordimageheight) / 2;
	const int swordimagey = (g_ScreenBuffer->h - swordimageheight) / 2;
	
	const int mantraimagewidth = g_MantraBitmap->w;
	const int mantraimageheight = g_MantraBitmap->h;
	const int mantraimagex = swordimagex + swordimagewidth + ((SCREEN_WIDTH - (swordimagex + swordimagewidth + mantraimagewidth)) / 2);
	const int mantraimagey = swordimagey;
	
	const char *newbuttontxt = "New";
	const char *openbuttontxt = "Open";
	const char *savebuttontxt = "Save";
	const char *resumebuttontxt = "Resume";
	const char *quitbuttontxt = "Quit";
	
	const int newbuttontxtwidth = alfont_text_length(g_Font, newbuttontxt);
	const int openbuttontxtwidth = alfont_text_length(g_Font, openbuttontxt);
	const int savebuttontxtwidth = alfont_text_length(g_Font, savebuttontxt);
	const int resumebuttontxtwidth = alfont_text_length(g_Font, resumebuttontxt);
	const int quitbuttontxtwidth = alfont_text_length(g_Font, quitbuttontxt);
	
	const int buttonheight = (buttonbordery * 2) + smalltextheight;
	const int buttongap = (swordimageheight - (mantraimageheight + (4 * (2 + buttonheight + 2)))) / 5;
	
	const int newbuttonwidth = (buttonborderx * 2) + newbuttontxtwidth;
	const int openbuttonwidth = (buttonborderx * 2) + openbuttontxtwidth;
	const int savebuttonwidth = (buttonborderx * 2) + savebuttontxtwidth;
	const int resumebuttonwidth = (buttonborderx * 2) + resumebuttontxtwidth;
	const int quitbuttonwidth = (buttonborderx * 2) + quitbuttontxtwidth;
	const int buttonwidth = MAX(MAX(MAX(MAX(newbuttonwidth, openbuttonwidth), savebuttonwidth), resumebuttonwidth), quitbuttonwidth);
	
	const int buttonx = swordimagex + swordimagewidth + ((SCREEN_WIDTH - (swordimagex + swordimagewidth + 2 + buttonwidth + 2)) / 2);
	const int newbuttony = mantraimagey + mantraimageheight + buttongap;
	const int openbuttony = newbuttony + buttonheight + buttongap;
	const int savebuttony = openbuttony + buttonheight + buttongap;
	const int resumebuttony = savebuttony + buttonheight + buttongap;
	const int quitbuttony = resumebuttony + buttonheight + buttongap;
	
//	printf("Gameplay time: %lld\n", g_GameplayTime);
	
	for(;;)
	{
		set_mouse_sprite(g_CursorBitmap);
		set_mouse_sprite_focus(4, 2);
		show_mouse(screen);
		
		pressedButton = 0;
		pressedMouse = 0;
		swordIndex = 0;
		ret = 0;
		flag = 1;
		fade = FADE_LENGTH;
		
		while(flag)
		{
			// poll for input
			pollKeys();
			pollMouseButtons();
			pollMousePosition();
			
			// check location of the mouse
			insideNew = mousePositionInRect(buttonx, newbuttony, buttonx + buttonwidth, newbuttony + buttonheight);
			insideOpen = mousePositionInRect(buttonx, openbuttony, buttonx + buttonwidth, openbuttony + buttonheight);
			insideSave = mousePositionInRect(buttonx, savebuttony, buttonx + buttonwidth, savebuttony + buttonheight);
			insideResume = mousePositionInRect(buttonx, resumebuttony, buttonx + buttonwidth, resumebuttony + buttonheight);
			insideQuit = mousePositionInRect(buttonx, quitbuttony, buttonx + buttonwidth, quitbuttony + buttonheight);
			
			if (keyPressed(MANTRA_KEY_N) && !pressedButton)
			{
				pressedButton = 1;
			}
			else if (keyPressed(MANTRA_KEY_O) && !pressedButton)
			{
				pressedButton = 2;
			}
			else if (keyPressed(MANTRA_KEY_S) && !pressedButton && !first)
			{
				pressedButton = 3;
			}
			else if (keyPressed(MANTRA_KEY_R) && !pressedButton && !first)
			{
				pressedButton = 4;
			}
			else if (keyPressed(MANTRA_KEY_Q) && !pressedButton)
			{
				pressedButton = 5;
			}
			else if (keyReleased(MANTRA_KEY_N) && pressedButton == 1)
			{
				pressedButton = 0;
				flag = 0;
				ret = 1;
			}
			else if (keyReleased(MANTRA_KEY_O) && pressedButton == 2)
			{
				pressedButton = 0;
				flag = 0;
				ret = 2;
			}
			else if (keyReleased(MANTRA_KEY_S) && pressedButton == 3)
			{
				pressedButton = 0;
				flag = 0;
				ret = 3;
			}
			else if (keyReleased(MANTRA_KEY_R) && pressedButton == 4)
			{
				pressedButton = 0;
				flag = 0;
				ret = 4;
			}
			else if (keyReleased(MANTRA_KEY_Q) && pressedButton == 5)
			{
				pressedButton = 0;
				flag = 0;
				ret = 5;
			}
			
			// mouse support
			if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideNew && !pressedMouse)
			{
				pressedMouse = 1;
			}
			else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideOpen && !pressedMouse)
			{
				pressedMouse = 2;
			}
			else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideSave && !pressedMouse && !first)
			{
				pressedMouse = 3;
			}
			else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideResume && !pressedMouse && !first)
			{
				pressedMouse = 4;
			}
			else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideQuit && !pressedMouse)
			{
				pressedMouse = 5;
			}
			else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideNew && pressedMouse == 1)
			{
				pressedMouse = 0;
				flag = 0;
				ret = 1;
			}
			else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideOpen && pressedMouse == 2)
			{
				pressedMouse = 0;
				flag = 0;
				ret = 2;
			}
			else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideSave && pressedMouse == 3)
			{
				pressedMouse = 0;
				flag = 0;
				ret = 3;
			}
			else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideResume && pressedMouse == 4)
			{
				pressedMouse = 0;
				flag = 0;
				ret = 4;
			}
			else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideQuit && pressedMouse == 5)
			{
				pressedMouse = 0;
				flag = 0;
				ret = 5;
			}
			else if (mouseButtonUp(MANTRA_MOUSE_LEFT))
			{
				pressedMouse = 0;
			}
			
			// draw the rotating sword
			clear_to_color(g_ScreenBuffer, blackColor);
			blit(g_SwordAnimData[swordIndex], g_ScreenBuffer, 0, 0, swordimagex, swordimagey, swordimagewidth, swordimageheight);
			
			swordIndex++;
			
			while(!g_SwordAnimData[swordIndex] && swordIndex < NUM_SWORD_FRAMES) swordIndex++;
			
			if (swordIndex >= NUM_SWORD_FRAMES)
			{
				swordIndex = 0;
			}
			
			// draw the mantra logo
			blit(g_MantraBitmap, g_ScreenBuffer, 0, 0, mantraimagex, mantraimagey, mantraimagewidth, mantraimageheight);
			
			// draw the buttons
			rectfill(g_ScreenBuffer, buttonx - 2, newbuttony - 2, buttonx + buttonwidth + 2, newbuttony + buttonheight + 2, grayColor);
			rectfill(g_ScreenBuffer, buttonx, newbuttony, buttonx + buttonwidth, newbuttony + buttonheight, (pressedButton == 1 || (pressedMouse == 1 && insideNew)) ? buttonPressedColor : buttonColor);
			alfont_textout_ex(g_ScreenBuffer, g_Font, newbuttontxt, buttonx + buttonborderx + ((buttonwidth - (2 * buttonborderx) - newbuttontxtwidth) / 2), newbuttony + buttonbordery + TEXT_HEIGHT_EXTRA, blackColor, -1);
			
			rectfill(g_ScreenBuffer, buttonx - 2, openbuttony - 2, buttonx + buttonwidth + 2, openbuttony + buttonheight + 2, grayColor);
			rectfill(g_ScreenBuffer, buttonx, openbuttony, buttonx + buttonwidth, openbuttony + buttonheight, (pressedButton == 2 || (pressedMouse == 2 && insideOpen)) ? buttonPressedColor : buttonColor);
			alfont_textout_ex(g_ScreenBuffer, g_Font, openbuttontxt, buttonx + buttonborderx + ((buttonwidth - (2 * buttonborderx) - openbuttontxtwidth) / 2), openbuttony + buttonbordery + TEXT_HEIGHT_EXTRA, blackColor, -1);
			
			if (!first)
			{
				rectfill(g_ScreenBuffer, buttonx - 2, savebuttony - 2, buttonx + buttonwidth + 2, savebuttony + buttonheight + 2, grayColor);
				rectfill(g_ScreenBuffer, buttonx, savebuttony, buttonx + buttonwidth, savebuttony + buttonheight, (pressedButton == 3 || (pressedMouse == 3 && insideSave)) ? buttonPressedColor : buttonColor);
				alfont_textout_ex(g_ScreenBuffer, g_Font, savebuttontxt, buttonx + buttonborderx + ((buttonwidth - (2 * buttonborderx) - savebuttontxtwidth) / 2), savebuttony + buttonbordery + TEXT_HEIGHT_EXTRA, blackColor, -1);
				
				rectfill(g_ScreenBuffer, buttonx - 2, resumebuttony - 2, buttonx + buttonwidth + 2, resumebuttony + buttonheight + 2, grayColor);
				rectfill(g_ScreenBuffer, buttonx, resumebuttony, buttonx + buttonwidth, resumebuttony + buttonheight, (pressedButton == 4 || (pressedMouse == 4 && insideSave)) ? buttonPressedColor : buttonColor);
				alfont_textout_ex(g_ScreenBuffer, g_Font, resumebuttontxt, buttonx + buttonborderx + ((buttonwidth - (2 * buttonborderx) - resumebuttontxtwidth) / 2), resumebuttony + buttonbordery + TEXT_HEIGHT_EXTRA, blackColor, -1);
			}
			else
			{
				rectfill(g_ScreenBuffer, buttonx - 2, savebuttony - 2, buttonx + buttonwidth + 2, savebuttony + buttonheight + 2, grayedOutBackgroundColor);
				rectfill(g_ScreenBuffer, buttonx, savebuttony, buttonx + buttonwidth, savebuttony + buttonheight, grayedOutBackgroundColor);
				alfont_textout_ex(g_ScreenBuffer, g_Font, savebuttontxt, buttonx + buttonborderx + ((buttonwidth - (2 * buttonborderx) - savebuttontxtwidth) / 2), savebuttony + buttonbordery + TEXT_HEIGHT_EXTRA, grayedOutTextColor, -1);
				
				rectfill(g_ScreenBuffer, buttonx - 2, resumebuttony - 2, buttonx + buttonwidth + 2, resumebuttony + buttonheight + 2, grayedOutBackgroundColor);
				rectfill(g_ScreenBuffer, buttonx, resumebuttony, buttonx + buttonwidth, resumebuttony + buttonheight, grayedOutBackgroundColor);
				alfont_textout_ex(g_ScreenBuffer, g_Font, resumebuttontxt, buttonx + buttonborderx + ((buttonwidth - (2 * buttonborderx) - resumebuttontxtwidth) / 2), resumebuttony + buttonbordery + TEXT_HEIGHT_EXTRA, grayedOutTextColor, -1);
			}
			
			rectfill(g_ScreenBuffer, buttonx - 2, quitbuttony - 2, buttonx + buttonwidth + 2, quitbuttony + buttonheight + 2, grayColor);
			rectfill(g_ScreenBuffer, buttonx, quitbuttony, buttonx + buttonwidth, quitbuttony + buttonheight, (pressedButton == 5 || (pressedMouse == 5 && insideQuit)) ? buttonPressedColor : buttonColor);
			alfont_textout_ex(g_ScreenBuffer, g_Font, quitbuttontxt, buttonx + buttonborderx + ((buttonwidth - (2 * buttonborderx) - quitbuttontxtwidth) / 2), quitbuttony + buttonbordery + TEXT_HEIGHT_EXTRA, blackColor, -1);
			
			// blit to the screen (potentially fading)
			if (fade >= 0)
			{
				fade_interpolate(black_palette, g_SystemPalette, tempPalette, ((FADE_LENGTH - fade) * 64) / FADE_LENGTH, 0, 255);
				set_palette(tempPalette);
				
				drawBufferToScreen();
				
				fade--;
			}
			else
			{
				set_palette(g_SystemPalette);
				drawBufferToScreen();
			}
			
			rest(FADE_REST_MILLISECONDS);
			
			if (ret == 5 && g_GameDirty)
			{
				if (!showAlertDialog("Are you sure you want to quit?  Your current game will be lost.", "OK", "Cancel"))
				{
					ret = 0;
					flag = 1;
					continue;
				}
			}
		}
		/*
		for(i = fade;i <= FADE_LENGTH;i++)
		{
			fade_interpolate(black_palette, g_SystemPalette, tempPalette, ((FADE_LENGTH - i) * 64) / FADE_LENGTH, 0, 255);
			set_palette(tempPalette);
			
			rest(NUM_MILLISECONDS_BETWEEN_REFRESH / 2);
		}
		
		clear_to_color(g_ScreenBuffer, blackColor);
		drawBufferToScreen();
		set_palette(g_SystemPalette);
		*/
		fadeToBlack();
		
		show_mouse(NULL);
		
		if (ret == 1)
		{
			newGame();
			break;
		}
		else if (ret == 2)
		{
			if (showOpenGameScreen())
			{
				break;
			}
		}
		else if (ret == 3)
		{
			if (showSaveGameScreen())
			{
				break;
			}
		}
		else if (ret == 4)
		{
			break;
		}
		else if (ret == 5)
		{
			return 0;
		}
	}
	
	return 1;
}
