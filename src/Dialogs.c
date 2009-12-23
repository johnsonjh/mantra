/*
 *  Dialogs.c
 *  Mantra
 *
 *  Created by Chris O'Neill on 6/17/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#if defined(MANTRA_WINDOWS)
//#include "StdAfx.h"
#include <allegro.h>
#else
#include <Allegro/allegro.h>
#endif

#include <string.h>
 
#include "GameConstants.h"
#include "Utils.h"
#include "Input.h"
#include "Sound.h"
#include "Map.h"
#include "Dialogs.h"


int showAlertDialog(char *messagetxt, char *okbuttontxt, char *cancelbuttontxt)
{
	short ret = 0;
	int insideOk, insideCancel;
	int pressedButton, pressedMouse, flag;
	
	const int blackColor = makecol(0, 0, 0);
	const int grayColor = makecol(210, 210, 210);
	const int buttonColor = makecol(180, 180, 180);
	const int buttonPressedColor = makecol(90, 90, 90);
//	const int grayedOutBackgroundColor = makecol(140, 140, 140);
//	const int grayedOutTextColor = makecol(90, 90, 90);
	
	const int textborder = 15;
	const int buttonborder = 5;
	
//	const int textheight = alfont_get_font_height(g_DialogFont) * 2;
	const int smalltextheight = alfont_get_font_height(g_Font);
	
//	const int buttongap = 5;
	
	const int width = SCREEN_WIDTH / 2;
	
	const int okbuttontextwidth = alfont_text_length(g_Font, okbuttontxt);
	const int cancelbuttontextwidth = alfont_text_length(g_Font, cancelbuttontxt);
	const int buttontextwidth = MAX(okbuttontextwidth, cancelbuttontextwidth);
	const int okbuttonwidth = (buttonborder * 2) + buttontextwidth;
	const int cancelbuttonwidth = (buttonborder * 2) + buttontextwidth;
	const int buttonheight = (buttonborder * 2) + smalltextheight;
	
	const int messagewidth = width - (2 * textborder);
	const int messageheight = smalltextheight * textout_paragraph_ex(g_ScreenBuffer, g_Font, messagetxt, -100, -100, 0, 0, messagewidth);
	
	const int height = messageheight + buttonheight + (3 * textborder);
	
	const int x = (SCREEN_WIDTH - width) / 2;
	const int y = (SCREEN_HEIGHT - STATS_HEIGHT - height) / 2;
	
	const int messagex = x + textborder;
	const int messagey = y + textborder;
	
	const int okbuttonx = x + width - textborder - 2 - okbuttonwidth;
	const int cancelbuttonx = okbuttonx - textborder - cancelbuttonwidth;
	const int buttony = y + height - textborder - 2 - buttonheight;
	
	// draw border
	rectfill(g_ScreenBuffer, x - 2, y - 2, x + width + 2, y + height + 2, blackColor);
	rectfill(g_ScreenBuffer, x, y, x + width, y + height, grayColor);
	
	textout_paragraph_ex(g_ScreenBuffer, g_Font, messagetxt, messagex, messagey, blackColor, -1, messagewidth);
	
	// wait for key input
	pressedButton = 0;
	pressedMouse = 0;
	flag = 1;
	
	while(flag)
	{
		// detect moues position
		insideOk = mousePositionInRect(okbuttonx, buttony, okbuttonx + okbuttonwidth, buttony + buttonheight);
		insideCancel = mousePositionInRect(cancelbuttonx, buttony, cancelbuttonx + cancelbuttonwidth, buttony + buttonheight);
		
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
		
		// mouse support
		if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideCancel && !pressedMouse)
		{
			pressedMouse = 1;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideOk && !pressedMouse)
		{
			pressedMouse = 2;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideCancel && pressedMouse == 1)
		{
			pressedMouse = 0;
			flag = 0;
			ret = 0;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideOk && pressedMouse == 2)
		{
			pressedMouse = 0;
			flag = 0;
			ret = 1;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT))
		{
			pressedMouse = 0;
		}
		
		// draw button list at the bottom
		rectfill(g_ScreenBuffer, okbuttonx - 2, buttony - 2, okbuttonx + okbuttonwidth + 2, buttony + buttonheight + 2, blackColor);
		rectfill(g_ScreenBuffer, okbuttonx, buttony, okbuttonx + okbuttonwidth, buttony + buttonheight, (pressedButton == 2 || (pressedMouse == 2 && insideOk)) ? buttonPressedColor : buttonColor);
		alfont_textout_ex(g_ScreenBuffer, g_Font, okbuttontxt, okbuttonx + ((okbuttonwidth - okbuttontextwidth) / 2), buttony + buttonborder + TEXT_HEIGHT_EXTRA, blackColor, -1);
		
		rectfill(g_ScreenBuffer, cancelbuttonx - 2, buttony - 2, cancelbuttonx + cancelbuttonwidth + 2, buttony + buttonheight + 2, blackColor);
		rectfill(g_ScreenBuffer, cancelbuttonx, buttony, cancelbuttonx + cancelbuttonwidth, buttony + buttonheight, (pressedButton == 1 || (pressedMouse == 1 && insideCancel)) ? buttonPressedColor : buttonColor);
		alfont_textout_ex(g_ScreenBuffer, g_Font, cancelbuttontxt, cancelbuttonx + ((cancelbuttonwidth - cancelbuttontextwidth) / 2), buttony + buttonborder + TEXT_HEIGHT_EXTRA, blackColor, -1);
		
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
	
	return ret;
}

void displayMessage(short resID, short index, short itemNum)
{
	char message[2048];
	Str255 lines[10];
	
	int blackColor = makecol(0, 0, 0);
	int grayColor = makecol(210, 210, 210);
	
	int maxlen = 0;
	int w, h;
	const int textborder = 10;
	int i = 0, j = 0, line = 0, temp;
	
	memset(message, 0, 2048 * sizeof(char));
	sprintf(message, "%s%s", g_Messages[index - 1], (index == 5) ? g_ItemTemplates[itemNum].name : "");
	
	for(;message[i] != 0;i++, j++)
	{
		if (message[i] == '\n')
		{
			lines[line][j] = 0;
			
			line++;
			j = -1;
			
			continue;
		}
		
		if (j >= 40)
		{
			lines[line][j] = 0;
			temp = j;
			
			while(lines[line][j] != ' ' && j > 0)
			{
				i--;
				j--;
			}
			
			if (j > 0)
			{
				lines[line][j] = 0;
			}
			else
			{
				i += temp;
			}
			
			line++;
			j = -1;
		}
		
		lines[line][j] = message[i];
	}
	
	lines[line][j] = 0;
	line++;
		
	printf("Displaying message: \n");
	
	for(i = 0;i < line;i++)
	{
		printf("%s (%d)\n", lines[i], alfont_text_length(g_Font, lines[i]));
	}
	
	for(i = 0;i < line;i++)
	{
		temp = alfont_text_length(g_Font, lines[i]);
		
		if (temp > maxlen)
		{
			maxlen = temp;
		}
	}
	
	w = (2 * textborder) + maxlen;
	h = (2 * textborder) + ((alfont_text_height(g_Font) * 2) * line) - TEXT_HEIGHT_EXTRA;
	
	displayCurrentMapScreenBlit(true, false);
	
	rectfill(g_ScreenBuffer, (16*32 - w) / 2, (10*32 - h) / 2, (16*32 + w) / 2, (10*32 + h) / 2, blackColor);
	rectfill(g_ScreenBuffer, ((16*32 - w) / 2) + 2, ((10*32 - h) / 2) + 2, ((16*32 + w) / 2) - 2, ((10*32 + h) / 2) - 2, grayColor);
	
	for(i = 0;i < line;i++)
	{
		alfont_textout_ex(g_ScreenBuffer, g_Font, lines[i], ((16*32 - w) / 2) + textborder, ((10*32 - h) / 2) + textborder + ((alfont_text_height(g_Font) * 2) * i), blackColor, -1);
	}
	
	drawBufferToScreen();
	waitForKeyPressed();
	displayCurrentMapScreen();
}

void displayItemMessage(short itemID)
{
	Str255 lines[10];
	
	const int blackColor = makecol(0, 0, 0);
	const int grayColor = makecol(210, 210, 210);
	
	int maxlen = 0;
	int w, h;
	const int textborder = 10;
	int i = 0, j = 0, line = 0, temp;
	/*
	memset(message, 0, 2048 * sizeof(char));
	sprintf(message, "%s%s", g_Messages[index - 1], (index == 5) ? g_ItemTemplates[itemNum].name : "");
	*/
	for(;g_ItemTemplates[itemID].description[i] != 0;i++, j++)
	{
		if (g_ItemTemplates[itemID].description[i] == '\n')
		{
			lines[line][j] = 0;
			
			line++;
			j = -1;
			
			continue;
		}
		
		if (j >= 40)
		{
			lines[line][j] = 0;
			temp = j;
			
			while(lines[line][j] != ' ' && j > 0)
			{
				i--;
				j--;
			}
			
			if (j > 0)
			{
				lines[line][j] = 0;
			}
			else
			{
				i += temp;
			}
			
			line++;
			j = -1;
		}
		
		lines[line][j] = g_ItemTemplates[itemID].description[i];
	}
	
	lines[line][j] = 0;
	line++;
	
	printf("Displaying message: \n");
	
	for(i = 0;i < line;i++)
	{
		printf("%s\n", lines[i]);
	}
	
	for(i = 0;i < line;i++)
	{
		temp = alfont_text_length(g_Font, lines[i]);
		
		if (temp > maxlen)
		{
			maxlen = temp;
		}
	}
	
	w = (2 * textborder) + maxlen;
	h = (2 * textborder) + ((alfont_text_height(g_Font) * 2) * line) - TEXT_HEIGHT_EXTRA;
	
	displayCurrentMapScreenBlit(true, false);
	
	rectfill(g_ScreenBuffer, (16*32 - w) / 2, (10*32 - h) / 2, (16*32 + w) / 2, (10*32 + h) / 2, blackColor);
	rectfill(g_ScreenBuffer, ((16*32 - w) / 2) + 2, ((10*32 - h) / 2) + 2, ((16*32 + w) / 2) - 2, ((10*32 + h) / 2) - 2, grayColor);
	
	for(i = 0;i < line;i++)
	{
		alfont_textout_ex(g_ScreenBuffer, g_Font, lines[i], ((16*32 - w) / 2) + textborder, ((10*32 - h) / 2) + textborder + ((alfont_text_height(g_Font) * 2) * i), blackColor, -1);
	}
	
	drawBufferToScreen();
	waitForKeyPressed();
	displayCurrentMapScreen();
}

short doStatsDialog(Boolean canDoStore)
{
	// not implemented yet...
	// lots of custom gui stuff to come...
	// YAY! look at all the custom gui stuff...
	
	char buff[1024];
	short ret = dialogClose;
	int insideDone, insideItem, insideStore;
	int i, inset, pressedButton, pressedMouse, flag;
	
	const int blackColor = makecol(0, 0, 0);
	const int grayColor = makecol(210, 210, 210);
	const int buttonColor = makecol(180, 180, 180);
	const int buttonPressedColor = makecol(90, 90, 90);
	const int grayedOutBackgroundColor = makecol(140, 140, 140);
	const int grayedOutTextColor = makecol(90, 90, 90);
	
	const int x = (SCREEN_WIDTH - DIALOG_WIDTH) / 2;
	const int y = (SCREEN_HEIGHT - STATS_HEIGHT - DIALOG_HEIGHT) / 2;
	
	const int textborder = 15;
	const int buttonborder = 5;
	
	const int textheight = alfont_get_font_height(g_DialogFont) * 2;
	const int smalltextheight = alfont_get_font_height(g_Font);
	
	const int statnamex = x + textborder;
	const int statnamey = y + textborder;
	const int statvaluex = statnamex + alfont_text_length(g_DialogFont, "Experience    ");
	
	const int damagetypey = statnamey + (10 * textheight) + (2 * textheight);
	
	const int weaponstartx = x + (DIALOG_WIDTH / 2);
	const int weaponstarty = y + textborder;
	
	const int weaponiconx = weaponstartx + alfont_text_length(g_DialogFont, "Sword:    ");
	const int weaponnamex = weaponiconx + 32 + textborder;
	
	const int weaponincy = 32 + textheight;
	
	const int immunitystarty = damagetypey;
	
	const char *donebuttontxt = "(D)one";
	const char *itemsbuttontxt = "(I)tems";
	const char *storebuttontxt = "S(t)ore";
	
	const int buttongap = 5;
	
	const int donebuttonwidth = (buttonborder * 2) + alfont_text_length(g_Font, donebuttontxt);
	const int itemsbuttonwidth = (buttonborder * 2) + alfont_text_length(g_Font, itemsbuttontxt);
	const int storebuttonwidth = (buttonborder * 2) + alfont_text_length(g_Font, storebuttontxt);
	const int buttonheight = (buttonborder * 2) + smalltextheight;
	
	const int donebuttonx = x + DIALOG_WIDTH - textborder - 2 - donebuttonwidth;
	const int itemsbuttonx = x + textborder + 2;
	const int storebuttonx = itemsbuttonx + itemsbuttonwidth + 2 + buttongap + 2;
	const int buttony = y + DIALOG_HEIGHT - textborder - 2 - buttonheight;
	
	// draw border
	rectfill(g_ScreenBuffer, x - 2, y - 2, x + DIALOG_WIDTH + 2, y + DIALOG_HEIGHT + 2, blackColor);
	rectfill(g_ScreenBuffer, x, y, x + DIALOG_WIDTH, y + DIALOG_HEIGHT, grayColor);
	
	// draw vital stats
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, "Health", statnamex, statnamey, blackColor, -1);
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, "Stamina", statnamex, statnamey + (1 * textheight), blackColor, -1);
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, "Experience", statnamex, statnamey + (2 * textheight), blackColor, -1);
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, "Armor", statnamex, statnamey + (3 * textheight), blackColor, -1);
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, "Level", statnamex, statnamey + (4 * textheight), blackColor, -1);
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, "Money", statnamex, statnamey + (5 * textheight), blackColor, -1);
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, "Damage", statnamex, statnamey + (6 * textheight), blackColor, -1);
	
	memset(buff, 0, 1024 * sizeof(char));
	sprintf(buff, "%d/%d", g_Saric.health, g_Saric.maxHealth);
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, buff, statvaluex, statnamey, blackColor, -1);
	
	memset(buff, 0, 1024 * sizeof(char));
	sprintf(buff, "%d/%d", g_Saric.stamina, g_Saric.maxStamina);
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, buff, statvaluex, statnamey + (1 * textheight), blackColor, -1);
	
	memset(buff, 0, 1024 * sizeof(char));
	sprintf(buff, "%ld/%ld", g_Saric.experience, g_Saric.nextLevel);
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, buff, statvaluex, statnamey + (2 * textheight), blackColor, -1);
	
	memset(buff, 0, 1024 * sizeof(char));
	sprintf(buff, "%d", g_Saric.armorValue + g_Saric.itemEffects[0].armor + g_Saric.itemEffects[1].armor + g_Saric.itemEffects[2].armor);
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, buff, statvaluex, statnamey + (3 * textheight), blackColor, -1);
	
	memset(buff, 0, 1024 * sizeof(char));
	sprintf(buff, "%d", g_Saric.level);
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, buff, statvaluex, statnamey + (4 * textheight), blackColor, -1);
	
	memset(buff, 0, 1024 * sizeof(char));
	sprintf(buff, "%ld", g_Saric.money);
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, buff, statvaluex, statnamey + (5 * textheight), blackColor, -1);
	
	memset(buff, 0, 1024 * sizeof(char));
	sprintf(buff, "%d", g_Saric.damage + g_Saric.itemEffects[0].damage + g_Saric.itemEffects[2].damage);
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, buff, statvaluex, statnamey + (6 * textheight), blackColor, -1);
	
	// draw damage icons
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, "Your Damage Is:", statnamex, damagetypey, blackColor, -1);
	
	inset = 0;
	
	for(i = 0;i < NUM_IMMUNITY_ICONS;i++)
	{
		if (g_Saric.damageType & (1 << i))
		{
			draw_sprite(g_ScreenBuffer, g_ImmunityIcons[i], statnamex + inset, damagetypey + textheight);
			
			inset += 16 + 2;
		}
	}
	
	// draw current weapons
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, "Sword:", weaponstartx, weaponstarty + (32 / 2), blackColor, -1);
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, "Other:", weaponstartx, weaponstarty + (32 / 2) + (1 * weaponincy), blackColor, -1);
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, "Armor:", weaponstartx, weaponstarty + (32 / 2) + (2 * weaponincy), blackColor, -1);
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		if (g_Saric.itemEquipped[i])
		{
			if (g_ItemTemplates[i].attributes & isSword)
			{
				draw_sprite(g_ScreenBuffer, g_LargeIcons[i], weaponiconx, weaponstarty);
				
				alfont_textout_ex(g_ScreenBuffer, g_DialogFont, g_ItemTemplates[i].name, weaponnamex, weaponstarty + (32 / 2), blackColor, -1);
			}
			else if (g_ItemTemplates[i].attributes & isSpecialItem)
			{
				draw_sprite(g_ScreenBuffer, g_LargeIcons[i], weaponiconx, weaponstarty + (1 * weaponincy));
				
				alfont_textout_ex(g_ScreenBuffer, g_DialogFont, g_ItemTemplates[i].name, weaponnamex, weaponstarty + (32 / 2) + (1 * weaponincy), blackColor, -1);
			}
			else if (g_ItemTemplates[i].attributes & isArmor)
			{
				draw_sprite(g_ScreenBuffer, g_LargeIcons[i], weaponiconx, weaponstarty + (2 * weaponincy));
				
				alfont_textout_ex(g_ScreenBuffer, g_DialogFont, g_ItemTemplates[i].name, weaponnamex, weaponstarty + (32 / 2) + (2 * weaponincy), blackColor, -1);
			}
			else
			{
				// printf("Strange Item!!!\n");
				// not actually so strange...
				// turns out not all "other" items 
				// are marked as a special item...
			}
		}
	}
	
	// draw immunity icons
	alfont_textout_ex(g_ScreenBuffer, g_DialogFont, "You Are Immune To:", weaponstartx, immunitystarty, blackColor, -1);
	
	inset = 0;
	
	for(i = 0;i < NUM_IMMUNITY_ICONS;i++)
	{
		if (g_Saric.immunities & (1 << i))
		{
			draw_sprite(g_ScreenBuffer, g_ImmunityIcons[i], weaponstartx + inset, immunitystarty + textheight);
			
			inset += 16 + 2;
		}
	}
	
	// wait for key input
	pressedButton = 0;
	pressedMouse = 0;
	flag = 1;
	
	while(flag)
	{
		// detect mouse position
		insideDone = mousePositionInRect(donebuttonx, buttony, donebuttonx + donebuttonwidth, buttony + buttonheight);
		insideItem = mousePositionInRect(itemsbuttonx, buttony, itemsbuttonx + itemsbuttonwidth, buttony + buttonheight);
		insideStore = mousePositionInRect(storebuttonx, buttony, storebuttonx + storebuttonwidth, buttony + buttonheight);
		
		if (keyPressed(MANTRA_KEY_D) && !pressedButton)
		{
			pressedButton = 3;
		}
		else if (keyPressed(MANTRA_KEY_Q) && !pressedButton)
		{
			pressedButton = 3;
		}
		else if (keyPressed(MANTRA_KEY_I) && !pressedButton)
		{
			pressedButton = 1;
		}
		else if (keyPressed(MANTRA_KEY_T) && !pressedButton && canDoStore)
		{
			pressedButton = 2;
		}
		else if (keyReleased(MANTRA_KEY_D) && pressedButton == 3)
		{
			pressedButton = 0;
			flag = 0;
			ret = dialogClose;
			
		}
		else if (keyReleased(MANTRA_KEY_Q) && pressedButton == 3)
		{
			pressedButton = 0;
			flag = 0;
			ret = dialogClose;
			
		}
		else if (keyReleased(MANTRA_KEY_I) && pressedButton == 1)
		{
			pressedButton = 0;
			flag = 0;
			ret = dialogItems;
		}
		else if (keyReleased(MANTRA_KEY_T) && pressedButton == 2)
		{
			pressedButton = 0;
			flag = 0;
			ret = dialogStore;
		}
		
		// mouse support
		if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideDone && !pressedMouse)
		{
			pressedMouse = 3;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideItem && !pressedMouse)
		{
			pressedMouse = 1;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideStore && !pressedMouse && canDoStore)
		{
			pressedMouse = 2;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideDone && pressedMouse == 3)
		{
			pressedMouse = 0;
			flag = 0;
			ret = dialogClose;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideItem && pressedMouse == 1)
		{
			pressedMouse = 0;
			flag = 0;
			ret = dialogItems;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideStore && pressedMouse == 2)
		{
			pressedMouse = 0;
			flag = 0;
			ret = dialogStore;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT))
		{
			pressedMouse = 0;
		}
		
		// draw button list at the bottom
		rectfill(g_ScreenBuffer, donebuttonx - 2, buttony - 2, donebuttonx + donebuttonwidth + 2, buttony + buttonheight + 2, blackColor);
		rectfill(g_ScreenBuffer, donebuttonx, buttony, donebuttonx + donebuttonwidth, buttony + buttonheight, (pressedButton == 3 || (pressedMouse == 3 && insideDone)) ? buttonPressedColor : buttonColor);
		alfont_textout_ex(g_ScreenBuffer, g_Font, donebuttontxt, donebuttonx + buttonborder, buttony + buttonborder + TEXT_HEIGHT_EXTRA, blackColor, -1);
		
		rectfill(g_ScreenBuffer, itemsbuttonx - 2, buttony - 2, itemsbuttonx + itemsbuttonwidth + 2, buttony + buttonheight + 2, blackColor);
		rectfill(g_ScreenBuffer, itemsbuttonx, buttony, itemsbuttonx + itemsbuttonwidth, buttony + buttonheight, (pressedButton == 1 || (pressedMouse == 1 && insideItem)) ? buttonPressedColor : buttonColor);
		alfont_textout_ex(g_ScreenBuffer, g_Font, itemsbuttontxt, itemsbuttonx + buttonborder, buttony + buttonborder + TEXT_HEIGHT_EXTRA, blackColor, -1);
		
		if (canDoStore)
		{
			rectfill(g_ScreenBuffer, storebuttonx - 2, buttony - 2, storebuttonx + storebuttonwidth + 2, buttony + buttonheight + 2, blackColor);
			rectfill(g_ScreenBuffer, storebuttonx, buttony, storebuttonx + storebuttonwidth, buttony + buttonheight, (pressedButton == 2 || (pressedMouse == 2 && insideStore)) ? buttonPressedColor : buttonColor);
			alfont_textout_ex(g_ScreenBuffer, g_Font, storebuttontxt, storebuttonx + buttonborder, buttony + buttonborder + TEXT_HEIGHT_EXTRA, blackColor, -1);
		}
		else
		{
			rectfill(g_ScreenBuffer, storebuttonx - 2, buttony - 2, storebuttonx + storebuttonwidth + 2, buttony + buttonheight + 2, grayedOutBackgroundColor);
			rectfill(g_ScreenBuffer, storebuttonx, buttony, storebuttonx + storebuttonwidth, buttony + buttonheight, buttonColor);
			alfont_textout_ex(g_ScreenBuffer, g_Font, storebuttontxt, storebuttonx + buttonborder, buttony + buttonborder + TEXT_HEIGHT_EXTRA, grayedOutTextColor, -1);
		}
		
		drawBufferToScreen();
		
		if (flag)
		{
			waitForSpecificInput(INPUT_KEY | INPUT_MOUSE_BUTTONS | INPUT_MOUSE_POSITION);
		}
		else
		{
			updateSound();
			rest(NUM_MILLISECONDS_BETWEEN_REFRESH * 3);
		}
	}
	
	return ret;
}

short doItemsDialog(Boolean canDoStore)
{
	// not implemented yet...
	// lots of custom gui stuff to come...
	
	// observe!  lots of custom gui stuff:
	
	int *swordslist, *otherslist, *armorslist, *messageslist;
	int numswords, numothers, numarmors, nummessages;
	int swordsindex, othersindex, armorsindex, messagesindex;
	short ret = dialogClose;
	Item item;
	int currentitemcount, *currentitemlist;
	static int selectedTab = 0;
	int itemindex, topitem;
	int insideDone, insideStats, insideSelect, insideStore;
	int insideSword, insideOther, insideArmor, insideMessage;
	int insideList, insideScrollTop, insideScrollBottom;
	int pressedButton, pressedMouse;
	long itemAttribs;
	int i, attribMask, flag;
	/*
	const int blackColor = makecol(0, 0, 0);
	const int grayColor = makecol(210, 210, 210);
	const int outlineColor = makecol(140, 140, 140);
	const int textColor = outlineColor;
	const int buttonColor = makecol(180, 180, 180);
	const int buttonPressedColor = makecol(90, 90, 90);
	const int grayedOutTextColor = makecol(90, 90, 90);
	const int grayedOutOutlineColor = outlineColor;
	const int selectedItemColor = grayColor;
	*/
	const int blackColor = makecol(0, 0, 0);
	const int grayColor = makecol(210, 210, 210);
	const int outlineColor = blackColor;
	const int textColor = blackColor;
	const int buttonColor = makecol(180, 180, 180);
	const int buttonPressedColor = makecol(90, 90, 90);
	const int grayedOutOutlineColor = outlineColor;
	const int grayedOutTextColor = makecol(90, 90, 90);
	const int selectedItemColor = makecol(210, 210, 210);
	
	const int x = (SCREEN_WIDTH - DIALOG_WIDTH) / 2;
	const int y = (SCREEN_HEIGHT - STATS_HEIGHT - DIALOG_HEIGHT) / 2;
	
	const int textborder = 15;
	const int buttonborder = 5;
	const int tabborder = 8;
	
	const int textheight = alfont_get_font_height(g_DialogFont);
	const int smalltextheight = alfont_get_font_height(g_Font);
	
	const char *swordtabtxt = "Sword";
	const char *othertabtxt = "Other";
	const char *armortabtxt = "Armor";
	const char *messagetabtxt = "Messages";
	
	const int swordtextlen = alfont_text_length(g_DialogFont, swordtabtxt);
	const int othertextlen = alfont_text_length(g_DialogFont, othertabtxt);
	const int armortextlen = alfont_text_length(g_DialogFont, armortabtxt);
	const int messagetextlen = alfont_text_length(g_DialogFont, messagetabtxt);
	
	const int swordtabx = x + textborder + 2;
	const int swordtabwidth = ((8 * buttonborder) / 3) + swordtextlen;
	
	const int othertabx = swordtabx + swordtabwidth + 2 + tabborder + 2;
	const int othertabwidth = ((8 * buttonborder) / 3) + othertextlen;
	
	const int armortabx = othertabx + othertabwidth + 2 + tabborder + 2;
	const int armortabwidth = ((8 * buttonborder) / 3) + armortextlen;
	
	const int messagetabx = armortabx + armortabwidth + 2 + tabborder + 2;
	const int messagetabwidth = ((8 * buttonborder) / 3) + messagetextlen;
	
	const int taby = y + textborder + 2;
	const int tabheight = (2 * buttonborder) + textheight;
	
	const int liststartx = swordtabx;
	const int liststarty = taby + tabheight + 2 + 1;
	const int liststartwidth = messagetabx + messagetabwidth - liststartx;
	const int liststartheight = DIALOG_HEIGHT - textborder - 2 - 2 - tabheight - 2 - textborder;
	
	const char *donebuttontxt = "(D)one";
	const char *statsbuttontxt = "(S)tats";
	const char *storebuttontxt = "S(t)ore";
	const char *selectbuttontxt = "S(e)lect";
	
	const int donebuttontxtwidth = alfont_text_length(g_Font, donebuttontxt);
	const int statsbuttontxtwidth = alfont_text_length(g_Font, statsbuttontxt);
	const int storebuttontxtwidth = alfont_text_length(g_Font, storebuttontxt);
	const int selectbuttontxtwidth = alfont_text_length(g_Font, selectbuttontxt);
	
	const int buttongap = 5;
	
	const int donebuttonwidthtemp = (buttonborder * 2) + donebuttontxtwidth;
	const int selectbuttonwidthtemp = (buttonborder * 2) + selectbuttontxtwidth;
	const int donebuttonwidth = MAX(donebuttonwidthtemp, selectbuttonwidthtemp);
	const int statsbuttonwidth = (buttonborder * 2) + statsbuttontxtwidth;
	const int storebuttonwidth = (buttonborder * 2) + storebuttontxtwidth;
	const int selectbuttonwidth = donebuttonwidth;
	const int buttonheight = (buttonborder * 2) + smalltextheight;
	
	const int statsbuttonx = liststartx + liststartwidth + 2 + textborder + 2;
	const int statsbuttony = y + DIALOG_HEIGHT - textborder - 2 - buttonheight;
	const int storebuttonx = statsbuttonx;
	const int storebuttony = statsbuttony - 2 - buttongap - 2 - buttonheight;
	const int donebuttonx = x + DIALOG_WIDTH - textborder - 2 - donebuttonwidth - 2;
	const int donebuttony = statsbuttony;
	const int selectbuttonx = x + DIALOG_WIDTH - textborder - 2 - selectbuttonwidth - 2;
	const int selectbuttony = storebuttony;
	
	const int iteminfox = liststartx + liststartwidth + 2 + textborder + 2;
	const int iteminfoy = taby;
	const int iteminfowidth = x + DIALOG_WIDTH - textborder - 2 - iteminfox - 2;
	const int iteminfoheight = selectbuttony - iteminfoy - 2 - textborder - 2;
	
	const int iteminfoiconx = iteminfox + 2;
	const int iteminfoicony = iteminfoy + 2;
	const int iteminfonamex = iteminfoiconx + 32 + 2;
	const int iteminfonamey = iteminfoicony + (32 / 2);
	const int iteminfodescx = iteminfoiconx;
	const int iteminfodescy = iteminfoicony + 32 + buttongap;
	const int iteminfodescwidth = iteminfowidth - 2 - 2;
	
	const int iteminfospeedx = iteminfox + 2;
	const int iteminfospeedy = iteminfoy + iteminfoheight - 2 - (2 * textheight);
	const int iteminfodamagex = iteminfospeedx;
	const int iteminfodamagey = iteminfospeedy - 2 - (2 * textheight);
	const int iteminfoarmorx = iteminfodamagex;
	const int iteminfoarmory = iteminfodamagey - 2 - (2 * textheight);
	
	const int iteminfochargesx = iteminfox + (iteminfowidth / 2);
	const int iteminfochargesy = iteminfoarmory;
	const int iteminfostaminax = iteminfochargesx;
	const int iteminfostaminay = iteminfodamagey;
	const int iteminfodamagehealedx = iteminfostaminax;
	const int iteminfodamagehealedy = iteminfospeedy;
	
	const char *iteminfoarmortxt = "Armor: ";
	const char *iteminfodamagetxt = "Damage: ";
	const char *iteminfospeedtxt = "Speed: ";
	const char *iteminfochargestxt = "Charges: ";
	const char *iteminfostaminatxt = "Stamina: ";
	const char *iteminfodamagehealedtxt = "Dmg Healed: ";
	
	const int iteminfoarmortxtlen = alfont_text_length(g_DialogFont, iteminfoarmortxt);
	const int iteminfodamagetxtlen = alfont_text_length(g_DialogFont, iteminfodamagetxt);
	const int iteminfospeedtxtlen = alfont_text_length(g_DialogFont, iteminfospeedtxt);
	const int iteminfochargestxtlen = alfont_text_length(g_DialogFont, iteminfochargestxt);
	const int iteminfostaminatxtlen = alfont_text_length(g_DialogFont, iteminfostaminatxt);
	const int iteminfodamagehealedtxtlen = alfont_text_length(g_DialogFont, iteminfodamagehealedtxt);
	
	const int iteminfospeedvaluex = iteminfospeedx + iteminfospeedtxtlen;
	const int iteminfospeedvaluey = iteminfospeedy;
	const int iteminfodamagevaluex = iteminfodamagex + iteminfodamagetxtlen;
	const int iteminfodamagevaluey = iteminfodamagey;
	const int iteminfoarmorvaluex = iteminfoarmorx + iteminfoarmortxtlen;
	const int iteminfoarmorvaluey = iteminfoarmory;
	
	const int iteminfochargesvaluex = iteminfochargesx + iteminfochargestxtlen;
	const int iteminfochargesvaluey = iteminfochargesy;
	const int iteminfostaminavaluex = iteminfostaminax + iteminfostaminatxtlen;
	const int iteminfostaminavaluey = iteminfostaminay;
	const int iteminfodamagehealedvaluex = iteminfodamagehealedx + iteminfodamagehealedtxtlen;
	const int iteminfodamagehealedvaluey = iteminfodamagehealedy;
	
	const int listitemgap = 4;
	const int scrollitemgap = 0;
	
	const int scrollsidegap = 2;
	const int scrollstartwidth = 10;
	const int scrollstartheight = liststartheight - (2 * scrollsidegap);
	const int scrollstartx = liststartx + liststartwidth - scrollsidegap - scrollstartwidth;
	const int scrollstarty = liststarty + scrollsidegap;
	const int scrollstartarrowheight = scrollstartwidth;
//	const int scrollbarwidth = scrollstartwidth - 4;
	const int scrollbargap = 2;
	
	const int listitemiconheight = 16;
	const int listitemcheckwidth = 8;
	const int listitemcheckheight = 8;
	const int listitemy = liststarty + listitemgap;
	const int listitemheight = MAX(smalltextheight, listitemiconheight);
	
	const int listitemiconx = liststartx + buttonborder;
	const int listitemcheckx = listitemiconx + 16 + buttongap;
	const int listitemnamex = listitemcheckx + listitemcheckwidth + 2;
	const int listitemcountx = liststartx + liststartwidth - 2 - scrollstartwidth - buttongap;
	
	const int maxitemsinlist = (liststartheight / (listitemheight + listitemgap));
	
	printf("liststartwidth: %d, dialog width: %d\n", liststartwidth, DIALOG_WIDTH);
	
	numswords = numothers = numarmors = nummessages = 0;
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		if (g_Saric.itemQuantities[i] > 0)
		{
			if (g_ItemTemplates[i].attributes & isSword)
			{
				numswords++;
			//	numswords++;
			}
			else if (g_ItemTemplates[i].attributes & isArmor)
			{
				numarmors++;
			//	numarmors++;
			}
			else if (g_ItemTemplates[i].attributes & isMessage)
			{
				nummessages++;
			//	nummessages++;
			}
			else if (g_ItemTemplates[i].attributes & (isNotSelectable | isSelectable | isSpecialItem))
			{
				numothers++;
			//	numothers++;
			}
		}
	}
	
	swordslist = numswords ? (int *)malloc(numswords * sizeof(int)) : NULL;
	otherslist = numothers ? (int *)malloc(numothers * sizeof(int)) : NULL;
	armorslist = numarmors ? (int *)malloc(numarmors * sizeof(int)) : NULL;
	messageslist = nummessages ? (int *)malloc(nummessages * sizeof(int)) : NULL;
	
	swordsindex = othersindex = armorsindex = messagesindex = 0;
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		if (g_Saric.itemQuantities[i] > 0)
		{
			printf("Item detected: %s, num: %d\n", g_ItemTemplates[i].name, i);
			if (g_ItemTemplates[i].attributes & isSword)
			{
				swordslist[swordsindex] = i;
				swordsindex++;
			//	swordslist[swordsindex] = i;
			//	swordsindex++;
			}
			else if (g_ItemTemplates[i].attributes & isArmor)
			{
				armorslist[armorsindex] = i;
				armorsindex++;
			//	armorslist[armorsindex] = i;
			//	armorsindex++;
			}
			else if (g_ItemTemplates[i].attributes & isMessage)
			{
				messageslist[messagesindex] = i;
				messagesindex++;
			//	messageslist[messagesindex] = i;
			//	messagesindex++;
			}
			else if (g_ItemTemplates[i].attributes & (isNotSelectable | isSelectable | isSpecialItem))
			{
				otherslist[othersindex] = i;
				othersindex++;
			//	otherslist[othersindex] = i;
			//	othersindex++;
			}
		}
	}
	
	// draw border
	rectfill(g_ScreenBuffer, x - 2, y - 2, x + DIALOG_WIDTH + 2, y + DIALOG_HEIGHT + 2, blackColor);
	rectfill(g_ScreenBuffer, x, y, x + DIALOG_WIDTH, y + DIALOG_HEIGHT, grayColor);
	
	// draw item list area
	rectfill(g_ScreenBuffer, liststartx - 2, liststarty - 2, liststartx + liststartwidth + 2, liststarty + liststartheight + 2, outlineColor);
	rectfill(g_ScreenBuffer, liststartx, liststarty, liststartx + liststartwidth, liststarty + liststartheight, buttonColor);
	
	// wait for key input
//	selectedTab = 0;
	itemindex = 0;
	
	pressedButton = 0;
	pressedMouse = 0;
	flag = 1;
	
	currentitemlist = swordslist;
	currentitemcount = numswords;
	topitem = 0;
	
	while(flag)
	{
		// detect mouse position
		insideDone = mousePositionInRect(donebuttonx, donebuttony, donebuttonx + donebuttonwidth, donebuttony + buttonheight);
		insideStats = mousePositionInRect(statsbuttonx, statsbuttony, statsbuttonx + statsbuttonwidth, statsbuttony + buttonheight);
		insideSelect = mousePositionInRect(selectbuttonx, selectbuttony, selectbuttonx + selectbuttonwidth, selectbuttony + buttonheight);
		insideStore = mousePositionInRect(storebuttonx, storebuttony, storebuttonx + storebuttonwidth, storebuttony + buttonheight);
		insideSword = mousePositionInRect(swordtabx, taby, swordtabx + swordtabwidth, taby + tabheight);
		insideOther = mousePositionInRect(othertabx, taby, othertabx + othertabwidth, taby + tabheight);
		insideArmor = mousePositionInRect(armortabx, taby, armortabx + armortabwidth, taby + tabheight);
		insideMessage = mousePositionInRect(messagetabx, taby, messagetabx + messagetabwidth, taby + tabheight);
		
		if (currentitemcount > maxitemsinlist)
		{
			insideList = mousePositionInRect(liststartx, liststarty, scrollstartx, liststarty + liststartheight);
			insideScrollTop = mousePositionInRect(scrollstartx, scrollstarty, scrollstartx + scrollstartwidth, scrollstarty + scrollstartarrowheight);
			insideScrollBottom = mousePositionInRect(scrollstartx, scrollstarty + scrollstartheight - scrollstartarrowheight, scrollstartx + scrollstartwidth, scrollstarty + scrollstartheight);
		}
		else
		{
			insideList = mousePositionInRect(liststartx, liststarty, liststartx + liststartwidth, liststarty + liststartheight);
			insideScrollTop = 0;
			insideScrollBottom = 0;
		}
		
		// check for tab change
		if (keyReleased(MANTRA_KEY_LEFT))
		{
			selectedTab--;
			topitem = 0;
			
			if (selectedTab < 0)
			{
				selectedTab = 3;
			}
			
			itemindex = 0;
		}
		else if (keyReleased(MANTRA_KEY_RIGHT))
		{
			selectedTab++;
			topitem = 0;
			
			if (selectedTab > 3)
			{
				selectedTab = 0;
			}
			
			itemindex = 0;
		}
		
		// check for item change
		if (keyReleased(MANTRA_KEY_UP) && currentitemcount)
		{
			itemindex--;
			
			if (itemindex < 0)
			{
				itemindex = currentitemcount - 1;
				topitem = currentitemcount - maxitemsinlist;
				
				if (topitem < 0)
				{
					topitem = 0;
				}
			}
			else if (topitem > 0 && itemindex < topitem + scrollitemgap)
			{
				topitem = itemindex - scrollitemgap;
			}
			else if (topitem < currentitemcount - maxitemsinlist && itemindex > topitem + maxitemsinlist - (scrollitemgap + 1))
			{
				topitem = itemindex - maxitemsinlist + (scrollitemgap + 1);
			}
		}
		else if (keyReleased(MANTRA_KEY_DOWN) && currentitemcount)
		{
			itemindex++;
			
			if (itemindex >= currentitemcount)
			{
				itemindex = 0;
				topitem = 0;
			}
			else if (topitem > 0 && itemindex < topitem + scrollitemgap)
			{
				topitem = itemindex - scrollitemgap;
			}
			else if (topitem < currentitemcount - maxitemsinlist && itemindex > topitem + maxitemsinlist - (scrollitemgap + 1))
			{
				topitem = itemindex - maxitemsinlist + (scrollitemgap + 1);
			}
		}
		
		// check for buttons
		if (keyPressed(MANTRA_KEY_D) && !pressedButton)
		{
			pressedButton = 3;
		}
		else if (keyPressed(MANTRA_KEY_Q) && !pressedButton)
		{
			pressedButton = 3;
		}
		else if (keyPressed(MANTRA_KEY_ESC) && !pressedButton)
		{
			pressedButton = 3;
		}
		else if (keyPressed(MANTRA_KEY_S) && !pressedButton)
		{
			pressedButton = 1;
		}
		else if (keyPressed(MANTRA_KEY_T) && !pressedButton && canDoStore)
		{
			pressedButton = 2;
		}
		else if (keyPressed(MANTRA_KEY_E) && !pressedButton && currentitemcount)
		{
			pressedButton = 4;
		}
		else if (keyPressed(MANTRA_KEY_ENTER) && !pressedButton && currentitemcount)
		{
			pressedButton = 4;
		}
		else if (keyReleased(MANTRA_KEY_D) && pressedButton == 3)
		{
			pressedButton = 0;
			flag = 0;
			ret = dialogClose;
		}
		else if (keyReleased(MANTRA_KEY_Q) && pressedButton == 3)
		{
			pressedButton = 0;
			flag = 0;
			ret = dialogClose;
		}
		else if (keyReleased(MANTRA_KEY_ESC) && pressedButton == 3)
		{
			pressedButton = 0;
			flag = 0;
			ret = dialogClose;
		}
		else if (keyReleased(MANTRA_KEY_S) && pressedButton == 1)
		{
			pressedButton = 0;
			flag = 0;
			ret = dialogStats;
		}
		else if (keyReleased(MANTRA_KEY_T) && pressedButton == 2)
		{
			pressedButton = 0;
			flag = 0;
			ret = dialogStore;
		}
		else if ((keyReleased(MANTRA_KEY_E) && pressedButton == 4) || 
			 (keyReleased(MANTRA_KEY_ENTER) && pressedButton == 4))
		{
			pressedButton = 0;
			
			if (itemindex < topitem || itemindex >= topitem + maxitemsinlist)
			{
				topitem = itemindex;
				
				if (topitem > currentitemcount - maxitemsinlist)
				{
					topitem = currentitemcount - maxitemsinlist;
				}
			}
			
			itemAttribs = g_ItemTemplates[currentitemlist[itemindex]].attributes;	// get its attribs
			
			if (itemAttribs & (isMessage | isMoney | isNotSelectable))		// these don't check
			{
				// do nothing							// so do nothing
			}
			else if (itemAttribs & isSelectable)					// these just toggle
			{
				g_Saric.itemEquipped[currentitemlist[itemindex]] = !g_Saric.itemEquipped[currentitemlist[itemindex]];
			}
			else if (itemAttribs & (isSword | isArmor | isSpecialItem))	// these are exclusive
			{
				attribMask = itemAttribs & (isSword | isArmor | isSpecialItem);
				
				for (i = 0;i < currentitemcount;i++)
				{
					// do we have to deselect this?  are the attributes right?
					if (g_Saric.itemEquipped[currentitemlist[i]] && (g_ItemTemplates[currentitemlist[i]].attributes & attribMask))
					{
						g_Saric.itemEquipped[currentitemlist[i]] = false;	// deselect it
					}
				}
				
				g_Saric.itemEquipped[currentitemlist[itemindex]] = true;		// set this one to true
			}
		}
		
		// ugh... need to add mouse support
		if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideDone && !pressedMouse)
		{
			pressedMouse = 3;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideStats && !pressedMouse)
		{
			pressedMouse = 1;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideStore && !pressedMouse && canDoStore)
		{
			pressedMouse = 2;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideSelect && !pressedMouse && currentitemcount)
		{
			pressedMouse = 4;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideSword && selectedTab != 0 && !pressedMouse)
		{
			pressedMouse = 5;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideOther && selectedTab != 1 && !pressedMouse)
		{
			pressedMouse = 6;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideArmor && selectedTab != 2 && !pressedMouse)
		{
			pressedMouse = 7;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideMessage && selectedTab != 3 && !pressedMouse)
		{
			pressedMouse = 8;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideScrollTop && !pressedMouse)
		{
			pressedMouse = 9;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideScrollBottom && !pressedMouse)
		{
			pressedMouse = 10;
		}
		else if (mouseButtonPressed(MANTRA_MOUSE_LEFT) && insideList && !pressedMouse)
		{
			pressedMouse = 11;
			
			itemindex = (currentMouseY() - (liststarty + listitemgap)) / (listitemheight + listitemgap);
			
			if (itemindex >= currentitemcount)
			{
				itemindex = currentitemcount - 1;
			}
			else if (itemindex < 0)
			{
				itemindex = 0;
			}
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideStats && pressedMouse == 1)
		{
			pressedMouse = 0;
			flag = 0;
			ret = dialogStats;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideStore && pressedMouse == 2)
		{
			pressedMouse = 0;
			flag = 0;
			ret = dialogStore;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideDone && pressedMouse == 3)
		{
			pressedMouse = 0;
			flag = 0;
			ret = dialogClose;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideSelect && pressedMouse == 4)
		{
			pressedMouse = 0;
			
			if (itemindex < topitem || itemindex >= topitem + maxitemsinlist)
			{
				topitem = itemindex;
				
				if (topitem > currentitemcount - maxitemsinlist)
				{
					topitem = currentitemcount - maxitemsinlist;
				}
			}
			
			itemAttribs = g_ItemTemplates[currentitemlist[itemindex]].attributes;	// get its attribs
			
			if (itemAttribs & (isMessage | isMoney | isNotSelectable))		// these don't check
			{
				// do nothing							// so do nothing
			}
			else if (itemAttribs & isSelectable)					// these just toggle
			{
				g_Saric.itemEquipped[currentitemlist[itemindex]] = !g_Saric.itemEquipped[currentitemlist[itemindex]];
			}
			else if (itemAttribs & (isSword | isArmor | isSpecialItem))	// these are exclusive
			{
				attribMask = itemAttribs & (isSword | isArmor | isSpecialItem);
				
				for (i = 0;i < currentitemcount;i++)
				{
					// do we have to deselect this?  are the attributes right?
					if (g_Saric.itemEquipped[currentitemlist[i]] && (g_ItemTemplates[currentitemlist[i]].attributes & attribMask))
					{
						g_Saric.itemEquipped[currentitemlist[i]] = false;	// deselect it
					}
				}
				
				g_Saric.itemEquipped[currentitemlist[itemindex]] = true;		// set this one to true
			}
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideSword && selectedTab != 0 && pressedMouse == 5)
		{
			pressedMouse = 0;
			topitem = 0;
			itemindex = 0;
			selectedTab = 0;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideOther && selectedTab != 1 && pressedMouse == 6)
		{
			pressedMouse = 0;
			topitem = 0;
			itemindex = 0;
			selectedTab = 1;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideArmor && selectedTab != 2 && pressedMouse == 7)
		{
			pressedMouse = 0;
			topitem = 0;
			itemindex = 0;
			selectedTab = 2;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideMessage && selectedTab != 3 && pressedMouse == 8)
		{
			pressedMouse = 0;
			topitem = 0;
			itemindex = 0;
			selectedTab = 3;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideScrollTop && pressedMouse == 9)
		{
			pressedMouse = 0;
			
			if (topitem > 0)
			{
				topitem--;
			}
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideScrollBottom && pressedMouse == 10)
		{
			pressedMouse = 0;
			
			if (topitem < currentitemcount - maxitemsinlist)
			{
				topitem++;
			}
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideList && pressedMouse == 11)
		{
			itemindex = (currentMouseY() - (liststarty + listitemgap)) / (listitemheight + listitemgap);
			
			if (itemindex >= currentitemcount)
			{
				itemindex = currentitemcount - 1;
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
		
		// readjust for new tab
		if (selectedTab == 0)
		{
			currentitemlist = swordslist;
			currentitemcount = numswords;
		}
		else if (selectedTab == 1)
		{
			currentitemlist = otherslist;
			currentitemcount = numothers;
		}
		else if (selectedTab == 2)
		{
			currentitemlist = armorslist;
			currentitemcount = numarmors;
		}
		else if (selectedTab == 3)
		{
			currentitemlist = messageslist;
			currentitemcount = nummessages;
		}
		
		// draw tabs
		rectfill(g_ScreenBuffer, swordtabx - 2, taby - 2, swordtabx + swordtabwidth + 2, taby + tabheight + 2, outlineColor);
		rectfill(g_ScreenBuffer, swordtabx, taby, swordtabx + swordtabwidth, taby + tabheight, (pressedMouse == 5 && insideSword) ? buttonPressedColor : buttonColor);
		alfont_textout_ex(g_ScreenBuffer, g_DialogFont, swordtabtxt, swordtabx + ((swordtabwidth - swordtextlen) / 2), taby + buttonborder + TEXT_HEIGHT_EXTRA, textColor, -1);
		
		rectfill(g_ScreenBuffer, othertabx - 2, taby - 2, othertabx + othertabwidth + 2, taby + tabheight + 2, outlineColor);
		rectfill(g_ScreenBuffer, othertabx, taby, othertabx + othertabwidth, taby + tabheight, (pressedMouse == 6 && insideOther) ? buttonPressedColor : buttonColor);
		alfont_textout_ex(g_ScreenBuffer, g_DialogFont, othertabtxt, othertabx + ((othertabwidth - othertextlen) / 2), taby + buttonborder + TEXT_HEIGHT_EXTRA, textColor, -1);
		
		rectfill(g_ScreenBuffer, armortabx - 2, taby - 2, armortabx + armortabwidth + 2, taby + tabheight + 2, outlineColor);
		rectfill(g_ScreenBuffer, armortabx, taby, armortabx + armortabwidth, taby + tabheight, (pressedMouse == 7 && insideArmor) ? buttonPressedColor : buttonColor);
		alfont_textout_ex(g_ScreenBuffer, g_DialogFont, armortabtxt, armortabx +  + ((armortabwidth - armortextlen) / 2), taby + buttonborder + TEXT_HEIGHT_EXTRA, textColor, -1);
		
		rectfill(g_ScreenBuffer, messagetabx - 2, taby - 2, messagetabx + messagetabwidth + 2, taby + tabheight + 2, outlineColor);
		rectfill(g_ScreenBuffer, messagetabx, taby, messagetabx + messagetabwidth, taby + tabheight, (pressedMouse == 8 && insideMessage) ? buttonPressedColor : buttonColor);
		alfont_textout_ex(g_ScreenBuffer, g_DialogFont, messagetabtxt, messagetabx +  + ((messagetabwidth - messagetextlen) / 2), taby + buttonborder + TEXT_HEIGHT_EXTRA, textColor, -1);
		
		rectfill(g_ScreenBuffer, swordtabx, liststarty - 2, swordtabx + swordtabwidth, liststarty - 1, (selectedTab == 0) ? buttonColor : outlineColor);
		rectfill(g_ScreenBuffer, othertabx, liststarty - 2, othertabx + othertabwidth, liststarty - 1, (selectedTab == 1) ? buttonColor : outlineColor);
		rectfill(g_ScreenBuffer, armortabx, liststarty - 2, armortabx + armortabwidth, liststarty - 1, (selectedTab == 2) ? buttonColor : outlineColor);
		rectfill(g_ScreenBuffer, messagetabx, liststarty - 2, messagetabx + messagetabwidth, liststarty - 1, (selectedTab == 3) ? buttonColor : outlineColor);
		
		// draw the list of junk
		rectfill(g_ScreenBuffer, liststartx, liststarty, liststartx + liststartwidth, liststarty + liststartheight, buttonColor);
		
		for(i = topitem;i < currentitemcount && i < topitem + maxitemsinlist;i++)
		{
			// draw selected background
			if (i == itemindex)
			{
				rectfill(g_ScreenBuffer, liststartx, listitemy + ((i - topitem) * (listitemheight + listitemgap)) - (listitemgap / 2), (currentitemcount > maxitemsinlist) ? scrollstartx - scrollsidegap : liststartx + liststartwidth, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + listitemheight + (listitemgap / 2), selectedItemColor);
			}
			
		//	printf("About to draw an image: %d!\n", currentitemlist[i]);
		//	fflush(stdout);
			
			// draw icon
			if (g_SmallIcons[currentitemlist[i]])
			{
				draw_sprite(g_ScreenBuffer, g_SmallIcons[currentitemlist[i]], listitemiconx, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + ((listitemheight - 16) / 2));
			}
			else
			{
				stretch_sprite(g_ScreenBuffer, g_LargeIcons[currentitemlist[i]], listitemiconx, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + ((listitemheight - 16) / 2), 16, 16);
			}
			
		//	printf("Drew an image!\n");
		//	fflush(stdout);
			
			// draw check mark
			if (g_Saric.itemEquipped[currentitemlist[i]])
			{
				line(g_ScreenBuffer, listitemcheckx, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + ((listitemheight - listitemcheckheight) / 2) + (listitemcheckheight / 2), listitemcheckx + (listitemcheckwidth / 3), listitemy + ((i - topitem) * (listitemheight + listitemgap)) + (listitemheight + listitemcheckheight) / 2, blackColor);
				line(g_ScreenBuffer, listitemcheckx + (listitemcheckwidth / 3), listitemy + ((i - topitem) * (listitemheight + listitemgap)) + ((listitemheight + listitemcheckheight) / 2), listitemcheckx + listitemcheckwidth, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + ((listitemheight - listitemcheckheight) / 3), blackColor);
			}
			
			// draw name
			alfont_textout_ex(g_ScreenBuffer, g_Font, g_ItemTemplates[currentitemlist[i]].name, listitemnamex, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + ((listitemheight - smalltextheight) / 2) + TEXT_HEIGHT_EXTRA, textColor, -1);
			
			// draw count
			alfont_textprintf_right_ex(g_ScreenBuffer, g_Font, listitemcountx, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + ((listitemheight - smalltextheight) / 2) + TEXT_HEIGHT_EXTRA, textColor, -1, "%d", g_Saric.itemQuantities[currentitemlist[i]]);
		}
		
		// draw scroll bar
		if (currentitemcount > maxitemsinlist)
		{
			// draw the arrows
			rect(g_ScreenBuffer, scrollstartx, scrollstarty, scrollstartx + scrollstartwidth, scrollstarty + scrollstartarrowheight, outlineColor);
			rectfill(g_ScreenBuffer, scrollstartx + 1, scrollstarty + 1, scrollstartx + scrollstartwidth - 1, scrollstarty + scrollstartarrowheight - 1, (pressedMouse == 9 && insideScrollTop) ? buttonPressedColor : buttonColor);
			line(g_ScreenBuffer, scrollstartx + (scrollstartwidth / 2), scrollstarty + scrollstartarrowheight - 2, scrollstartx + (scrollstartwidth / 2), scrollstarty + 2, outlineColor);
			line(g_ScreenBuffer, scrollstartx + (scrollstartwidth / 2), scrollstarty + 2, scrollstartx + 2, scrollstarty + (scrollstartarrowheight / 2), outlineColor);
			line(g_ScreenBuffer, scrollstartx + (scrollstartwidth / 2), scrollstarty + 2, scrollstartx + scrollstartwidth - 2, scrollstarty + (scrollstartarrowheight / 2), outlineColor);
			
			rect(g_ScreenBuffer, scrollstartx, scrollstarty + scrollstartheight - scrollstartarrowheight, scrollstartx + scrollstartwidth, scrollstarty + scrollstartheight, outlineColor);
			rectfill(g_ScreenBuffer, scrollstartx + 1, scrollstarty + scrollstartheight - scrollstartarrowheight + 1, scrollstartx + scrollstartwidth - 1, scrollstarty + scrollstartheight - 1, (pressedMouse == 10 && insideScrollBottom) ? buttonPressedColor : buttonColor);
			line(g_ScreenBuffer, scrollstartx + (scrollstartwidth / 2), scrollstarty + scrollstartheight - 2, scrollstartx + (scrollstartwidth / 2), scrollstarty + scrollstartheight - scrollstartarrowheight + 2, outlineColor);
			line(g_ScreenBuffer, scrollstartx + (scrollstartwidth / 2), scrollstarty + scrollstartheight - 2, scrollstartx + 2, scrollstarty + scrollstartheight - (scrollstartarrowheight / 2), outlineColor);
			line(g_ScreenBuffer, scrollstartx + (scrollstartwidth / 2), scrollstarty + scrollstartheight - 2, scrollstartx + scrollstartwidth - 2, scrollstarty + scrollstartheight - (scrollstartarrowheight / 2), outlineColor);
			
			// draw the bar
			line(g_ScreenBuffer, scrollstartx, scrollstarty, scrollstartx, scrollstarty + scrollstartheight, outlineColor);
			line(g_ScreenBuffer, scrollstartx + scrollstartwidth, scrollstarty, scrollstartx + scrollstartwidth, scrollstarty + scrollstartheight, outlineColor);
			rectfill(g_ScreenBuffer, scrollstartx + scrollbargap, scrollstarty + scrollstartarrowheight + scrollbargap + ((topitem * (scrollstartheight - (2 * (scrollstartarrowheight + scrollbargap)))) / currentitemcount), scrollstartx + scrollstartwidth - scrollbargap, scrollstarty + scrollstartarrowheight - scrollbargap + (((topitem + maxitemsinlist) * (scrollstartheight - (2 * (scrollstartarrowheight + scrollbargap)))) / currentitemcount), outlineColor);
		}
		else
		{
			// no scroll bar if it is not needed...
		}
		
		// draw item info area
		rectfill(g_ScreenBuffer, iteminfox - 2, iteminfoy - 2, iteminfox + iteminfowidth + 2, iteminfoy + iteminfoheight + 2, outlineColor);
		rectfill(g_ScreenBuffer, iteminfox, iteminfoy, iteminfox + iteminfowidth, iteminfoy + iteminfoheight, buttonColor);
		
		if (currentitemcount > 0)
		{
			if (!(g_ItemTemplates[currentitemlist[itemindex]].attributes & isMessage))
			{
				// draw static item info txt
				alfont_textout_ex(g_ScreenBuffer, g_DialogFont, iteminfoarmortxt, iteminfoarmorx, iteminfoarmory, textColor, -1);
				alfont_textout_ex(g_ScreenBuffer, g_DialogFont, iteminfodamagetxt, iteminfodamagex, iteminfodamagey, textColor, -1);
				alfont_textout_ex(g_ScreenBuffer, g_DialogFont, iteminfospeedtxt, iteminfospeedx, iteminfospeedy, textColor, -1);
				alfont_textout_ex(g_ScreenBuffer, g_DialogFont, iteminfochargestxt, iteminfochargesx, iteminfochargesy, textColor, -1);
				alfont_textout_ex(g_ScreenBuffer, g_DialogFont, iteminfostaminatxt, iteminfostaminax, iteminfostaminay, textColor, -1);
				alfont_textout_ex(g_ScreenBuffer, g_DialogFont, iteminfodamagehealedtxt, iteminfodamagehealedx, iteminfodamagehealedy, textColor, -1);
				
				alfont_textprintf_ex(g_ScreenBuffer, g_DialogFont, iteminfoarmorvaluex, iteminfoarmorvaluey, textColor, -1, "%d", g_ItemTemplates[currentitemlist[itemindex]].armor);
				alfont_textprintf_ex(g_ScreenBuffer, g_DialogFont, iteminfodamagevaluex, iteminfodamagevaluey, textColor, -1, "%d", g_ItemTemplates[currentitemlist[itemindex]].damage);
				alfont_textprintf_ex(g_ScreenBuffer, g_DialogFont, iteminfospeedvaluex, iteminfospeedvaluey, textColor, -1, "%d", g_ItemTemplates[currentitemlist[itemindex]].speed);
				alfont_textprintf_ex(g_ScreenBuffer, g_DialogFont, iteminfochargesvaluex, iteminfochargesvaluey, textColor, -1, "%d", g_ItemTemplates[currentitemlist[itemindex]].charges);
				alfont_textprintf_ex(g_ScreenBuffer, g_DialogFont, iteminfostaminavaluex, iteminfostaminavaluey, textColor, -1, "%d", g_ItemTemplates[currentitemlist[itemindex]].stamina);
				alfont_textprintf_ex(g_ScreenBuffer, g_DialogFont, iteminfodamagehealedvaluex, iteminfodamagehealedvaluey, textColor, -1, "%d", g_ItemTemplates[currentitemlist[itemindex]].damageHealed);
			}
			
			// draw selected item's info
			draw_sprite(g_ScreenBuffer, g_LargeIcons[currentitemlist[itemindex]], iteminfoiconx, iteminfoicony);
			alfont_textout_ex(g_ScreenBuffer, g_DialogFont, g_ItemTemplates[currentitemlist[itemindex]].name, iteminfonamex, iteminfonamey, textColor, -1);
			textout_paragraph_ex(g_ScreenBuffer, g_DialogFont, g_ItemTemplates[currentitemlist[itemindex]].description, iteminfodescx, iteminfodescy, textColor, -1, iteminfodescwidth);
		}
		
		// draw button list at the bottom
		rectfill(g_ScreenBuffer, donebuttonx - 2, donebuttony - 2, donebuttonx + donebuttonwidth + 2, donebuttony + buttonheight + 2, outlineColor);
		rectfill(g_ScreenBuffer, donebuttonx, donebuttony, donebuttonx + donebuttonwidth, donebuttony + buttonheight, (pressedButton == 3 || (pressedMouse == 3 && insideDone)) ? buttonPressedColor : buttonColor);
		alfont_textout_ex(g_ScreenBuffer, g_Font, donebuttontxt, donebuttonx + buttonborder + ((donebuttonwidth - (2 * buttonborder) - donebuttontxtwidth) / 2), donebuttony + buttonborder + TEXT_HEIGHT_EXTRA, textColor, -1);
		
		if (currentitemcount)
		{
			rectfill(g_ScreenBuffer, selectbuttonx - 2, selectbuttony - 2, selectbuttonx + selectbuttonwidth + 2, selectbuttony + buttonheight + 2, outlineColor);
			rectfill(g_ScreenBuffer, selectbuttonx, selectbuttony, selectbuttonx + selectbuttonwidth, selectbuttony + buttonheight, (pressedButton == 4 || (pressedMouse == 4 && insideSelect)) ? buttonPressedColor : buttonColor);
			alfont_textout_ex(g_ScreenBuffer, g_Font, selectbuttontxt, selectbuttonx + buttonborder + ((selectbuttonwidth - (2 * buttonborder) - selectbuttontxtwidth) / 2), selectbuttony + buttonborder + TEXT_HEIGHT_EXTRA, textColor, -1);
		}
		else
		{
			rectfill(g_ScreenBuffer, selectbuttonx - 2, selectbuttony - 2, selectbuttonx + selectbuttonwidth + 2, selectbuttony + buttonheight + 2, grayedOutOutlineColor);
			rectfill(g_ScreenBuffer, selectbuttonx, selectbuttony, selectbuttonx + selectbuttonwidth, selectbuttony + buttonheight, buttonColor);
			alfont_textout_ex(g_ScreenBuffer, g_Font, selectbuttontxt, selectbuttonx + buttonborder + ((selectbuttonwidth - (2 * buttonborder) - selectbuttontxtwidth) / 2), selectbuttony + buttonborder + TEXT_HEIGHT_EXTRA, grayedOutTextColor, -1);
		}
		
		rectfill(g_ScreenBuffer, statsbuttonx - 2, statsbuttony - 2, statsbuttonx + statsbuttonwidth + 2, statsbuttony + buttonheight + 2, outlineColor);
		rectfill(g_ScreenBuffer, statsbuttonx, statsbuttony, statsbuttonx + statsbuttonwidth, statsbuttony + buttonheight, (pressedButton == 1 || (pressedMouse == 1 && insideStats)) ? buttonPressedColor : buttonColor);
		alfont_textout_ex(g_ScreenBuffer, g_Font, statsbuttontxt, statsbuttonx + buttonborder + ((statsbuttonwidth - (2 * buttonborder) - statsbuttontxtwidth) / 2), statsbuttony + buttonborder + TEXT_HEIGHT_EXTRA, textColor, -1);
		
		if (canDoStore)
		{
			rectfill(g_ScreenBuffer, storebuttonx - 2, storebuttony - 2, storebuttonx + storebuttonwidth + 2, storebuttony + buttonheight + 2, outlineColor);
			rectfill(g_ScreenBuffer, storebuttonx, storebuttony, storebuttonx + storebuttonwidth, storebuttony + buttonheight, (pressedButton == 2 || (pressedMouse == 2 && insideStore)) ? buttonPressedColor : buttonColor);
			alfont_textout_ex(g_ScreenBuffer, g_Font, storebuttontxt, storebuttonx + buttonborder + ((statsbuttonwidth - (2 * buttonborder) - storebuttontxtwidth) / 2), storebuttony + buttonborder + TEXT_HEIGHT_EXTRA, textColor, -1);
		}
		else
		{
			rectfill(g_ScreenBuffer, storebuttonx - 2, storebuttony - 2, storebuttonx + storebuttonwidth + 2, storebuttony + buttonheight + 2, grayedOutOutlineColor);
			rectfill(g_ScreenBuffer, storebuttonx, storebuttony, storebuttonx + storebuttonwidth, storebuttony + buttonheight, buttonColor);
			alfont_textout_ex(g_ScreenBuffer, g_Font, storebuttontxt, storebuttonx + buttonborder + ((storebuttonwidth - (2 * buttonborder) - storebuttontxtwidth) / 2), storebuttony + buttonborder + TEXT_HEIGHT_EXTRA, grayedOutTextColor, -1);
		}
		
		drawBufferToScreen();
		
		if (flag)
		{
			waitForSpecificInput(INPUT_KEY | INPUT_MOUSE_BUTTONS | INPUT_MOUSE_POSITION);
		}
		else
		{
			updateSound();
			rest(NUM_MILLISECONDS_BETWEEN_REFRESH * 3);
		}
	}
	
	if (swordslist)
	{
		free(swordslist);
	}
	
	if (otherslist)
	{
		free(otherslist);
	}
	
	if (armorslist)
	{
		free(armorslist);
	}
	
	if (messageslist)
	{
		free(messageslist);
	}
	
	// set g_Saric.itemEffects
	// (good thing I found this in their code!)
	
	// the selected sword goes into itemEffects[0]
	for (i = 1;i < 250;i++)
	{
		if ((g_ItemTemplates[i].attributes & isSword) && g_Saric.itemEquipped[i])
		{
			g_Saric.itemEffects[0] = g_ItemTemplates[i];
			break;									// there SHOULD be only one
		}
	}
	
	// the selected special item goes into itemEffects[1]
	for (i = 1;i < 250;i++)
	{
		if ((g_ItemTemplates[i].attributes & isSpecialItem) && g_Saric.itemEquipped[i])
		{
			g_Saric.itemEffects[1] = g_ItemTemplates[i];
			break;									// only one!
		}
	}
	
	/* from here on out I need to talk to you:  the armors are uniselected too: we'll have full body
		armors of different types, rather than pieces */
	
	item.name = 0;
	item.description = 0;
	item.attributes = isSelectable + isArmor;
	item.armor = 0;
	item.damage = 0;
	item.speed = 0;
	item.rateOfFire = 0;
	item.fireCounter = 0;
	item.firedMonsterID = 0;
	item.spriteRef = 0;
	item.quantity = 0;
	item.charges = 0;
	item.stamina = 0;
	item.damageHealed = 0;
	item.immunities = 0;
	item.damageType = 0;
	
	for (i = 1;i < 250;i++)
	{
		if (g_Saric.itemEquipped[i] && (g_ItemTemplates[i].attributes & (isSelectable + isArmor)))	//select armor+selectables
		{
			item.armor += g_ItemTemplates[i].armor;
			item.damage += g_ItemTemplates[i].damage;
			item.speed += g_ItemTemplates[i].speed;
			item.rateOfFire += g_ItemTemplates[i].rateOfFire;
			item.fireCounter += g_ItemTemplates[i].fireCounter;
			item.charges += g_ItemTemplates[i].charges;
			item.stamina += g_ItemTemplates[i].stamina;
			item.damageHealed += g_ItemTemplates[i].damageHealed;
			item.immunities |= g_ItemTemplates[i].immunities;
			item.damageType |= g_ItemTemplates[i].damageType;
		}
	}
	
	// itemEffects[2] is the accumulation of all the selected selectable items in the other list
	g_Saric.itemEffects[2] = item;
	
	// or together the immunities and damageTypes of the three things here, to get saric's total
	g_Saric.immunities = g_Saric.itemEffects[0].immunities | g_Saric.itemEffects[1].immunities | g_Saric.itemEffects[2].immunities;
	g_Saric.damageType = g_Saric.itemEffects[0].damageType | g_Saric.itemEffects[1].damageType | g_Saric.itemEffects[2].damageType;
	
	return ret;
}

#define STORE_SHOULD_BE_RUN

short doStoreDialog(short param)
{
	// not implemented yet...
	// lots of custom gui stuff to come...
	// custom gui stuff finally here!!!

#if defined(STORE_SHOULD_BE_RUN)
	
	int *storelist, *mylist, *templist;
	int numitems;
	int storeindex, myindex;
	short ret = dialogClose;
	int currentitemcount, *currentitemlist;
	static int selectedTab = 0;
	int itemindex, topitem;
	int insideDone, insideStats, insideBuy, insideItems;
	int insideStore, insideMy;
	int insideList, insideScrollTop, insideScrollBottom;
	int pressedButton, pressedMouse;
	int i, flag;
	
	const int blackColor = makecol(0, 0, 0);
	const int grayColor = makecol(210, 210, 210);
	const int buttonColor = makecol(180, 180, 180);
	const int buttonPressedColor = makecol(90, 90, 90);
	const int grayedOutBackgroundColor = makecol(140, 140, 140);
	const int grayedOutTextColor = makecol(90, 90, 90);
	const int selectedItemColor = grayColor;
	
	const int x = (SCREEN_WIDTH - DIALOG_WIDTH) / 2;
	const int y = (SCREEN_HEIGHT - STATS_HEIGHT - DIALOG_HEIGHT) / 2;
	
	const int textborder = 15;
	const int buttonborder = 5;
	const int tabborder = 8;
	
	const int textheight = alfont_get_font_height(g_DialogFont);
	const int smalltextheight = alfont_get_font_height(g_Font);
	
	const char *storeitemstabtxt = "Store Items";
	const char *myitemstabtxt = "Your Items";
	
	const int storeitemstextlen = alfont_text_length(g_DialogFont, storeitemstabtxt);
	const int myitemstextlen = alfont_text_length(g_DialogFont, myitemstabtxt);
	
	const int storeitemstabx = x + textborder + 2;
	const int storeitemstabwidth = ((8 * buttonborder) / 3) + storeitemstextlen;
	
	const int myitemstabx = storeitemstabx + storeitemstabwidth + 2 + tabborder + 2;
	const int myitemstabwidth = ((8 * buttonborder) / 3) + myitemstextlen;
	
	const int taby = y + textborder + 2;
	const int tabheight = (2 * buttonborder) + textheight;
	
	const int liststartx = storeitemstabx;
	const int liststarty = taby + tabheight + 2 + 1;
	const int liststartwidth = (7 * DIALOG_WIDTH / 15);
	const int liststartheight = DIALOG_HEIGHT - textborder - 2 - 2 - tabheight - 2 - textborder;
	
	const char *donebuttontxt = "(D)one";
	const char *itemsbuttontxt = "(I)tems";
	const char *statsbuttontxt = "(S)tats";
	const char *buybuttontxt = "(B)uy";
	
	const int donebuttontxtwidth = alfont_text_length(g_Font, donebuttontxt);
	const int itemsbuttontxtwidth = alfont_text_length(g_Font, itemsbuttontxt);
	const int statsbuttontxtwidth = alfont_text_length(g_Font, statsbuttontxt);
	const int buybuttontxtwidth = alfont_text_length(g_Font, buybuttontxt);
	
	const int buttongap = 5;
	
	const int donebuttonwidthtemp = (buttonborder * 2) + donebuttontxtwidth;
	const int buybuttonwidthtemp = (buttonborder * 2) + buybuttontxtwidth;
	const int donebuttonwidth = MAX(donebuttonwidthtemp, buybuttonwidthtemp);
	const int itemsbuttonwidth = (buttonborder * 2) + itemsbuttontxtwidth;
	const int statsbuttonwidth = (buttonborder * 2) + statsbuttontxtwidth;
	const int buybuttonwidth = donebuttonwidth;
	const int buttonheight = (buttonborder * 2) + smalltextheight;
	
	const int itemsbuttonx = liststartx + liststartwidth + 2 + textborder + 2;
	const int itemsbuttony = y + DIALOG_HEIGHT - textborder - 2 - buttonheight;
	const int statsbuttonx = itemsbuttonx;
	const int statsbuttony = itemsbuttony - 2 - buttongap - 2 - buttonheight;
	const int donebuttonx = x + DIALOG_WIDTH - textborder - 2 - donebuttonwidth - 2;
	const int donebuttony = itemsbuttony;
	const int buybuttonx = x + DIALOG_WIDTH - textborder - 2 - buybuttonwidth - 2;
	const int buybuttony = statsbuttony;
	
	const int moneyamountx = liststartx + liststartwidth + 2 + textborder + 2;
	const int moneyamounty = buybuttony - 2 - textborder - 2;
	
	const int iteminfox = liststartx + liststartwidth + 2 + textborder + 2;
	const int iteminfoy = taby;
	const int iteminfowidth = x + DIALOG_WIDTH - textborder - 2 - iteminfox - 2;
	const int iteminfoheight = buybuttony - iteminfoy - 2 - textborder - 2 - textheight - 2 - textborder - 2;
	
	const int iteminfoiconx = iteminfox + 2;
	const int iteminfoicony = iteminfoy + 2;
	const int iteminfonamex = iteminfoiconx + 32 + 2;
	const int iteminfonamey = iteminfoicony + (32 / 2);
	const int iteminfodescx = iteminfoiconx;
	const int iteminfodescy = iteminfoicony + 32 + buttongap;
	const int iteminfodescwidth = iteminfowidth - 2 - 2;
	
	const int iteminfospeedx = iteminfox + 2;
	const int iteminfospeedy = iteminfoy + iteminfoheight - 2 - (2 * textheight);
	const int iteminfodamagex = iteminfospeedx;
	const int iteminfodamagey = iteminfospeedy - 2 - (2 * textheight);
	const int iteminfoarmorx = iteminfodamagex;
	const int iteminfoarmory = iteminfodamagey - 2 - (2 * textheight);
	
	const int iteminfochargesx = iteminfox + (iteminfowidth / 2);
	const int iteminfochargesy = iteminfoarmory;
	const int iteminfostaminax = iteminfochargesx;
	const int iteminfostaminay = iteminfodamagey;
	const int iteminfodamagehealedx = iteminfostaminax;
	const int iteminfodamagehealedy = iteminfospeedy;
	
	const char *iteminfoarmortxt = "Armor: ";
	const char *iteminfodamagetxt = "Damage: ";
	const char *iteminfospeedtxt = "Speed: ";
	const char *iteminfochargestxt = "Charges: ";
	const char *iteminfostaminatxt = "Stamina: ";
	const char *iteminfodamagehealedtxt = "Dmg Healed: ";
	
	const int iteminfoarmortxtlen = alfont_text_length(g_DialogFont, iteminfoarmortxt);
	const int iteminfodamagetxtlen = alfont_text_length(g_DialogFont, iteminfodamagetxt);
	const int iteminfospeedtxtlen = alfont_text_length(g_DialogFont, iteminfospeedtxt);
	const int iteminfochargestxtlen = alfont_text_length(g_DialogFont, iteminfochargestxt);
	const int iteminfostaminatxtlen = alfont_text_length(g_DialogFont, iteminfostaminatxt);
	const int iteminfodamagehealedtxtlen = alfont_text_length(g_DialogFont, iteminfodamagehealedtxt);
	
	const int iteminfospeedvaluex = iteminfospeedx + iteminfospeedtxtlen;
	const int iteminfospeedvaluey = iteminfospeedy;
	const int iteminfodamagevaluex = iteminfodamagex + iteminfodamagetxtlen;
	const int iteminfodamagevaluey = iteminfodamagey;
	const int iteminfoarmorvaluex = iteminfoarmorx + iteminfoarmortxtlen;
	const int iteminfoarmorvaluey = iteminfoarmory;
	
	const int iteminfochargesvaluex = iteminfochargesx + iteminfochargestxtlen;
	const int iteminfochargesvaluey = iteminfochargesy;
	const int iteminfostaminavaluex = iteminfostaminax + iteminfostaminatxtlen;
	const int iteminfostaminavaluey = iteminfostaminay;
	const int iteminfodamagehealedvaluex = iteminfodamagehealedx + iteminfodamagehealedtxtlen;
	const int iteminfodamagehealedvaluey = iteminfodamagehealedy;
	
	const int listitemgap = 4;
	const int scrollitemgap = 0;
	
	const int scrollsidegap = 2;
	const int scrollstartwidth = 10;
	const int scrollstartheight = liststartheight - (2 * scrollsidegap);
	const int scrollstartx = liststartx + liststartwidth - scrollsidegap - scrollstartwidth;
	const int scrollstarty = liststarty + scrollsidegap;
	const int scrollstartarrowheight = scrollstartwidth;
//	const int scrollbarwidth = scrollstartwidth - 4;
	const int scrollbargap = 2;
	
	const int listitemiconheight = 16;
	const int listitemcheckwidth = 8;
	const int listitemcheckheight = 8;
	const int listitemy = liststarty + listitemgap;
	const int listitemheight = MAX(smalltextheight, listitemiconheight);
	const int listitemiconx = liststartx + buttonborder;
	const int listitemcheckx = listitemiconx + 16 + buttongap;
	const int listitemnamex = listitemcheckx + listitemcheckwidth + 2;
	const int listitemcountx = liststartx + liststartwidth - 2 - scrollstartwidth - buttongap;
	
	const int maxitemsinlist = (liststartheight / (listitemheight + listitemgap));
	
	param = -(param + 1);
	
//	printf("liststartwidth: %d, dialog width: %d\n", liststartwidth, DIALOG_WIDTH);
//	printf("param: %d, count: %d\n", param, g_Stores[param].count);
	
	numitems = 0;
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		if (g_Saric.itemQuantities[i] > 0)
		{
			if (g_ItemTemplates[i].attributes & (isNotSelectable | isSelectable | isSpecialItem))
			{
				numitems++;
			//	numitems++;
			}
		}
	}
	
	storelist = (int *)malloc(g_Stores[param].count * sizeof(int));
	mylist = numitems ? (int *)malloc(numitems * sizeof(int)) : NULL;
	
	storeindex = myindex = 0;
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		if (g_Saric.itemQuantities[i] > 0)
		{
		//	printf("Item detected: %s, num: %d\n", g_ItemTemplates[i].name, i);
			if (g_ItemTemplates[i].attributes & (isNotSelectable | isSelectable | isSpecialItem))
			{
				mylist[myindex] = i;
				myindex++;
			//	mylist[myindex] = i;
			//	myindex++;
			}
		}
	}
	
	for(i = 0;i < g_Stores[param].count;i++)
	{
		storelist[i] = g_Stores[param].item[i].index;
	//	printf("Selling item: %d, price: %d\n", storelist[i], g_Stores[param].item[i].price);
	}
	
	// draw border
	rectfill(g_ScreenBuffer, x - 2, y - 2, x + DIALOG_WIDTH + 2, y + DIALOG_HEIGHT + 2, blackColor);
	rectfill(g_ScreenBuffer, x, y, x + DIALOG_WIDTH, y + DIALOG_HEIGHT, grayColor);
	
	// draw item list area
	rectfill(g_ScreenBuffer, liststartx - 2, liststarty - 2, liststartx + liststartwidth + 2, liststarty + liststartheight + 2, blackColor);
	rectfill(g_ScreenBuffer, liststartx, liststarty, liststartx + liststartwidth, liststarty + liststartheight, buttonColor);
	
	// wait for key input
//	selectedTab = 0;
	itemindex = 0;
	
	pressedButton = 0;
	pressedMouse = 0;
	flag = 1;
	
	currentitemlist = storelist;
	currentitemcount = g_Stores[param].count;
	topitem = 0;
	
	while(flag)
	{
		// detect mouse position
		insideDone = mousePositionInRect(donebuttonx, donebuttony, donebuttonx + donebuttonwidth, donebuttony + buttonheight);
		insideStats = mousePositionInRect(statsbuttonx, statsbuttony, statsbuttonx + statsbuttonwidth, statsbuttony + buttonheight);
		insideBuy = mousePositionInRect(buybuttonx, buybuttony, buybuttonx + buybuttonwidth, buybuttony + buttonheight);
		insideItems = mousePositionInRect(itemsbuttonx, itemsbuttony, itemsbuttonx + itemsbuttonwidth, itemsbuttony + buttonheight);
		insideStore = mousePositionInRect(storeitemstabx, taby, storeitemstabx + storeitemstabwidth, taby + tabheight);
		insideMy = mousePositionInRect(myitemstabx, taby, myitemstabx + myitemstabwidth, taby + tabheight);
		
		if (currentitemcount > maxitemsinlist)
		{
			insideList = mousePositionInRect(liststartx, liststarty, scrollstartx, liststarty + liststartheight);
			insideScrollTop = mousePositionInRect(scrollstartx, scrollstarty, scrollstartx + scrollstartwidth, scrollstarty + scrollstartarrowheight);
			insideScrollBottom = mousePositionInRect(scrollstartx, scrollstarty + scrollstartheight - scrollstartarrowheight, scrollstartx + scrollstartwidth, scrollstarty + scrollstartheight);
		}
		else
		{
			insideList = mousePositionInRect(liststartx, liststarty, liststartx + liststartwidth, liststarty + liststartheight);
			insideScrollTop = 0;
			insideScrollBottom = 0;
		}
		
		// check for tab change
		if (keyReleased(MANTRA_KEY_LEFT))
		{
			selectedTab--;
			topitem = 0;
			
			if (selectedTab < 0)
			{
				selectedTab = 1;
			}
			
			itemindex = 0;
		}
		else if (keyReleased(MANTRA_KEY_RIGHT))
		{
			selectedTab++;
			topitem = 0;
			
			if (selectedTab > 1)
			{
				selectedTab = 0;
			}
			
			itemindex = 0;
		}
		
		// check for item change
		if (keyReleased(MANTRA_KEY_UP) && currentitemcount)
		{
			itemindex--;
			
			if (itemindex < 0)
			{
				itemindex = currentitemcount - 1;
				topitem = currentitemcount - maxitemsinlist;
				
				if (topitem < 0)
				{
					topitem = 0;
				}
			}
			else if (topitem > 0 && itemindex < topitem + scrollitemgap)
			{
				topitem = itemindex - scrollitemgap;
			}
			else if (topitem < currentitemcount - maxitemsinlist && itemindex > topitem + maxitemsinlist - (scrollitemgap + 1))
			{
				topitem = itemindex - maxitemsinlist + (scrollitemgap + 1);
			}
		}
		else if (keyReleased(MANTRA_KEY_DOWN) && currentitemcount)
		{
			itemindex++;
			
			if (itemindex >= currentitemcount)
			{
				itemindex = 0;
				topitem = 0;
			}
			else if (topitem > 0 && itemindex < topitem + scrollitemgap)
			{
				topitem = itemindex - scrollitemgap;
			}
			else if (topitem < currentitemcount - maxitemsinlist && itemindex > topitem + maxitemsinlist - (scrollitemgap + 1))
			{
				topitem = itemindex - maxitemsinlist + (scrollitemgap + 1);
			}
		}
		
		// check for buttons
		if (keyPressed(MANTRA_KEY_D) && !pressedButton)
		{
			pressedButton = 3;
		}
		else if (keyPressed(MANTRA_KEY_Q) && !pressedButton)
		{
			pressedButton = 3;
		}
		else if (keyPressed(MANTRA_KEY_I) && !pressedButton)
		{
			pressedButton = 1;
		}
		else if (keyPressed(MANTRA_KEY_S) && !pressedButton)
		{
			pressedButton = 2;
		}
		else if (keyPressed(MANTRA_KEY_B) && !pressedButton && currentitemlist == storelist)
		{
			pressedButton = 4;
		}
		else if (keyPressed(MANTRA_KEY_ENTER) && !pressedButton && currentitemlist == storelist)
		{
			pressedButton = 4;
		}
		else if (keyReleased(MANTRA_KEY_D) && pressedButton == 3)
		{
			pressedButton = 0;
			flag = 0;
			ret = dialogClose;
			
		}
		else if (keyReleased(MANTRA_KEY_Q) && pressedButton == 3)
		{
			pressedButton = 0;
			flag = 0;
			ret = dialogClose;
			
		}
		else if (keyReleased(MANTRA_KEY_I) && pressedButton == 1)
		{
			pressedButton = 0;
			flag = 0;
			ret = dialogItems;
		}
		else if (keyReleased(MANTRA_KEY_S) && pressedButton == 2)
		{
			pressedButton = 0;
			flag = 0;
			ret = dialogStats;
		}
		else if ((keyReleased(MANTRA_KEY_B) || keyReleased(MANTRA_KEY_ENTER)) && 
			 pressedButton == 4 && currentitemlist == storelist)
		{
			pressedButton = 0;
			
			if (g_Saric.money >= g_Stores[param].item[itemindex].price)
			{
				if (g_Saric.itemQuantities[g_Stores[param].item[itemindex].index] == 0)
				{
					templist = (int *)malloc((numitems + 1) * sizeof(int));
					
					for(i = 0;mylist[i] < g_Stores[param].item[itemindex].index;i++)
					{
						templist[i] = mylist[i];
					}
					
					templist[i] = g_Stores[param].item[itemindex].index;
					
					for(;i < numitems;i++)
					{
						templist[i + 1] = mylist[i];
					}
					
					free(mylist);
					mylist = templist;
					numitems++;
				}
				
				g_Saric.money -= g_Stores[param].item[itemindex].price;
				
				g_Saric.itemQuantities[g_Stores[param].item[itemindex].index]++;
				
				// play sound...
			}
			else
			{
				// possibly play sound...
			}
			/*
			itemAttribs = g_ItemTemplates[currentitemlist[itemindex]].attributes;	// get its attribs
			
			if (itemAttribs & (isMessage | isMoney | isNotSelectable))		// these don't check
			{
				// do nothing							// so do nothing
			}
			else if (itemAttribs & isSelectable)					// these just toggle
			{
				g_Saric.itemEquipped[currentitemlist[itemindex]] = !g_Saric.itemEquipped[currentitemlist[itemindex]];
			}
			else if (itemAttribs & (isSword | isArmor | isSpecialItem))	// these are exclusive
			{
				attribMask = itemAttribs & (isSword | isArmor | isSpecialItem);
				
				for (i = 0;i < currentitemcount;i++)
				{
					// do we have to deselect this?  are the attributes right?
					if (g_Saric.itemEquipped[currentitemlist[i]] && (g_ItemTemplates[currentitemlist[i]].attributes & attribMask))
					{
						g_Saric.itemEquipped[currentitemlist[i]] = false;	// deselect it
					}
				}
				
				g_Saric.itemEquipped[currentitemlist[itemindex]] = true;		// set this one to true
			}
			*/
		}
		
		// ugh... need to add mouse support
		if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideDone && !pressedMouse)
		{
			pressedMouse = 3;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideItems && !pressedMouse)
		{
			pressedMouse = 1;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideStats && !pressedMouse)
		{
			pressedMouse = 2;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideBuy && !pressedMouse)
		{
			pressedMouse = 4;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideStore && selectedTab != 0 && !pressedMouse)
		{
			pressedMouse = 5;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideMy && selectedTab != 1 && !pressedMouse)
		{
			pressedMouse = 6;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideScrollTop && !pressedMouse)
		{
			pressedMouse = 9;
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideScrollBottom && !pressedMouse)
		{
			pressedMouse = 10;
		}
		else if (mouseButtonPressed(MANTRA_MOUSE_LEFT) && insideList && !pressedMouse)
		{
			pressedMouse = 11;
			
			itemindex = (currentMouseY() - (liststarty + listitemgap)) / (listitemheight + listitemgap);
			
			if (itemindex >= currentitemcount)
			{
				itemindex = currentitemcount - 1;
			}
			else if (itemindex < 0)
			{
				itemindex = 0;
			}
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideDone && pressedMouse == 3)
		{
			pressedMouse = 0;
			flag = 0;
			ret = dialogClose;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideItems && pressedMouse == 1)
		{
			pressedMouse = 0;
			flag = 0;
			ret = dialogItems;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideStats && pressedMouse == 2)
		{
			pressedMouse = 0;
			flag = 0;
			ret = dialogStats;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideBuy && pressedMouse == 4 && currentitemlist == storelist)
		{
			pressedMouse = 0;
			
			if (g_Saric.money >= g_Stores[param].item[itemindex].price)
			{
				if (g_Saric.itemQuantities[g_Stores[param].item[itemindex].index] == 0)
				{
					templist = (int *)malloc((numitems + 1) * sizeof(int));
					
					for(i = 0;mylist[i] < g_Stores[param].item[itemindex].index;i++)
					{
						templist[i] = mylist[i];
					}
					
					templist[i] = g_Stores[param].item[itemindex].index;
					
					for(;i < numitems;i++)
					{
						templist[i + 1] = mylist[i];
					}
					
					free(mylist);
					mylist = templist;
					numitems++;
				}
				
				g_Saric.money -= g_Stores[param].item[itemindex].price;
				
				g_Saric.itemQuantities[g_Stores[param].item[itemindex].index]++;
				
				// play sound...
			}
			else
			{
				// possibly play sound...
			}
			/*
			itemAttribs = g_ItemTemplates[currentitemlist[itemindex]].attributes;	// get its attribs
			
			if (itemAttribs & (isMessage | isMoney | isNotSelectable))		// these don't check
			{
				// do nothing							// so do nothing
			}
			else if (itemAttribs & isSelectable)					// these just toggle
			{
				g_Saric.itemEquipped[currentitemlist[itemindex]] = !g_Saric.itemEquipped[currentitemlist[itemindex]];
			}
			else if (itemAttribs & (isSword | isArmor | isSpecialItem))	// these are exclusive
			{
				attribMask = itemAttribs & (isSword | isArmor | isSpecialItem);
				
				for (i = 0;i < currentitemcount;i++)
				{
					// do we have to deselect this?  are the attributes right?
					if (g_Saric.itemEquipped[currentitemlist[i]] && (g_ItemTemplates[currentitemlist[i]].attributes & attribMask))
					{
						g_Saric.itemEquipped[currentitemlist[i]] = false;	// deselect it
					}
				}
				
				g_Saric.itemEquipped[currentitemlist[itemindex]] = true;		// set this one to true
			}
			*/
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideStore && selectedTab != 0 && pressedMouse == 5)
		{
			pressedMouse = 0;
			topitem = 0;
			itemindex = 0;
			selectedTab = 0;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideMy && selectedTab != 1 && pressedMouse == 6)
		{
			pressedMouse = 0;
			topitem = 0;
			itemindex = 0;
			selectedTab = 1;
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideScrollTop && pressedMouse == 9)
		{
			pressedMouse = 0;
			
			if (topitem > 0)
			{
				topitem--;
			}
		}
		else if (mouseButtonUp(MANTRA_MOUSE_LEFT) && insideScrollBottom && pressedMouse == 10)
		{
			pressedMouse = 0;
			
			if (topitem < currentitemcount - maxitemsinlist)
			{
				topitem++;
			}
		}
		else if (mouseButtonDown(MANTRA_MOUSE_LEFT) && insideList && pressedMouse == 11)
		{
			itemindex = (currentMouseY() - (liststarty + listitemgap)) / (listitemheight + listitemgap);
			
			if (itemindex >= currentitemcount)
			{
				itemindex = currentitemcount - 1;
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
		
		// readjust for new tab
		if (selectedTab == 0)
		{
			currentitemlist = storelist;
			currentitemcount = g_Stores[param].count;
		}
		else if (selectedTab == 1)
		{
			currentitemlist = mylist;
			currentitemcount = numitems;
		}
		
		// draw tabs
		rectfill(g_ScreenBuffer, storeitemstabx - 2, taby - 2, storeitemstabx + storeitemstabwidth + 2, taby + tabheight + 2, blackColor);
		rectfill(g_ScreenBuffer, storeitemstabx, taby, storeitemstabx + storeitemstabwidth, taby + tabheight, (pressedMouse == 5 && insideStore) ? buttonPressedColor : buttonColor);
		alfont_textout_ex(g_ScreenBuffer, g_DialogFont, storeitemstabtxt, storeitemstabx + ((storeitemstabwidth - storeitemstextlen) / 2), taby + buttonborder + TEXT_HEIGHT_EXTRA, blackColor, -1);
		
		rectfill(g_ScreenBuffer, myitemstabx - 2, taby - 2, myitemstabx + myitemstabwidth + 2, taby + tabheight + 2, blackColor);
		rectfill(g_ScreenBuffer, myitemstabx, taby, myitemstabx + myitemstabwidth, taby + tabheight, (pressedMouse == 6 && insideMy) ? buttonPressedColor : buttonColor);
		alfont_textout_ex(g_ScreenBuffer, g_DialogFont, myitemstabtxt, myitemstabx + ((myitemstabwidth - myitemstextlen) / 2), taby + buttonborder + TEXT_HEIGHT_EXTRA, blackColor, -1);
		
		rectfill(g_ScreenBuffer, storeitemstabx, liststarty - 2, storeitemstabx + storeitemstabwidth, liststarty - 1, (selectedTab == 0) ? buttonColor : blackColor);
		rectfill(g_ScreenBuffer, myitemstabx, liststarty - 2, myitemstabx + myitemstabwidth, liststarty - 1, (selectedTab == 1) ? buttonColor : blackColor);
		
		// draw the list of junk
		rectfill(g_ScreenBuffer, liststartx, liststarty, liststartx + liststartwidth, liststarty + liststartheight, buttonColor);
		
		for(i = topitem;i < currentitemcount && i < topitem + maxitemsinlist;i++)
		{
			// draw selected background
			if (i == itemindex)
			{
			//	rectfill(g_ScreenBuffer, liststartx, listitemy + ((i - topitem) * (listitemheight + listitemgap)) - (listitemgap / 2), liststartx + liststartwidth, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + listitemheight + (listitemgap / 2), selectedItemColor);
				rectfill(g_ScreenBuffer, liststartx, listitemy + ((i - topitem) * (listitemheight + listitemgap)) - (listitemgap / 2), (currentitemcount > maxitemsinlist) ? scrollstartx - scrollsidegap : liststartx + liststartwidth, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + listitemheight + (listitemgap / 2), selectedItemColor);
			}
			
			// draw icon
			if (g_SmallIcons[currentitemlist[i]])
			{
				draw_sprite(g_ScreenBuffer, g_SmallIcons[currentitemlist[i]], listitemiconx, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + ((listitemheight - 16) / 2));
			}
			else
			{
				stretch_sprite(g_ScreenBuffer, g_LargeIcons[currentitemlist[i]], listitemiconx, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + ((listitemheight - 16) / 2), 16, 16);
			}
			
			// draw check mark
			if (g_Saric.itemEquipped[currentitemlist[i]])
			{
				line(g_ScreenBuffer, listitemcheckx, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + ((listitemheight - listitemcheckheight) / 2) + (listitemcheckheight / 2), listitemcheckx + (listitemcheckwidth / 3), listitemy + ((i - topitem) * (listitemheight + listitemgap)) + (listitemheight + listitemcheckheight) / 2, blackColor);
				line(g_ScreenBuffer, listitemcheckx + (listitemcheckwidth / 3), listitemy + ((i - topitem) * (listitemheight + listitemgap)) + ((listitemheight + listitemcheckheight) / 2), listitemcheckx + listitemcheckwidth, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + ((listitemheight - listitemcheckheight) / 3), blackColor);
			}
			
			// draw name
			alfont_textout_ex(g_ScreenBuffer, g_Font, g_ItemTemplates[currentitemlist[i]].name, listitemnamex, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + ((listitemheight - smalltextheight) / 2) + TEXT_HEIGHT_EXTRA, blackColor, -1);
			
			// draw count (or cost)
			if (currentitemlist == mylist)
			{
				alfont_textprintf_right_ex(g_ScreenBuffer, g_Font, listitemcountx, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + ((listitemheight - smalltextheight) / 2) + TEXT_HEIGHT_EXTRA, blackColor, -1, "%d", g_Saric.itemQuantities[currentitemlist[i]]);
			}
			else {
				alfont_textprintf_right_ex(g_ScreenBuffer, g_Font, listitemcountx, listitemy + ((i - topitem) * (listitemheight + listitemgap)) + ((listitemheight - smalltextheight) / 2) + TEXT_HEIGHT_EXTRA, blackColor, -1, "%d", g_Stores[param].item[i].price);
			}
		}
		
		// draw scroll bar
		if (currentitemcount > maxitemsinlist)
		{
			// draw the arrows
			rect(g_ScreenBuffer, scrollstartx, scrollstarty, scrollstartx + scrollstartwidth, scrollstarty + scrollstartarrowheight, blackColor);
			rectfill(g_ScreenBuffer, scrollstartx + 1, scrollstarty + 1, scrollstartx + scrollstartwidth - 1, scrollstarty + scrollstartarrowheight - 1, (pressedMouse == 9 && insideScrollTop) ? buttonPressedColor : buttonColor);
			line(g_ScreenBuffer, scrollstartx + (scrollstartwidth / 2), scrollstarty + scrollstartarrowheight - 2, scrollstartx + (scrollstartwidth / 2), scrollstarty + 2, blackColor);
			line(g_ScreenBuffer, scrollstartx + (scrollstartwidth / 2), scrollstarty + 2, scrollstartx + 2, scrollstarty + (scrollstartarrowheight / 2), blackColor);
			line(g_ScreenBuffer, scrollstartx + (scrollstartwidth / 2), scrollstarty + 2, scrollstartx + scrollstartwidth - 2, scrollstarty + (scrollstartarrowheight / 2), blackColor);
			
			rect(g_ScreenBuffer, scrollstartx, scrollstarty + scrollstartheight - scrollstartarrowheight, scrollstartx + scrollstartwidth, scrollstarty + scrollstartheight, blackColor);
			rectfill(g_ScreenBuffer, scrollstartx + 1, scrollstarty + scrollstartheight - scrollstartarrowheight + 1, scrollstartx + scrollstartwidth - 1, scrollstarty + scrollstartheight - 1, (pressedMouse == 10 && insideScrollBottom) ? buttonPressedColor : buttonColor);
			line(g_ScreenBuffer, scrollstartx + (scrollstartwidth / 2), scrollstarty + scrollstartheight - 2, scrollstartx + (scrollstartwidth / 2), scrollstarty + scrollstartheight - scrollstartarrowheight + 2, blackColor);
			line(g_ScreenBuffer, scrollstartx + (scrollstartwidth / 2), scrollstarty + scrollstartheight - 2, scrollstartx + 2, scrollstarty + scrollstartheight - (scrollstartarrowheight / 2), blackColor);
			line(g_ScreenBuffer, scrollstartx + (scrollstartwidth / 2), scrollstarty + scrollstartheight - 2, scrollstartx + scrollstartwidth - 2, scrollstarty + scrollstartheight - (scrollstartarrowheight / 2), blackColor);
			
			// draw the bar
			line(g_ScreenBuffer, scrollstartx, scrollstarty, scrollstartx, scrollstarty + scrollstartheight, blackColor);
			line(g_ScreenBuffer, scrollstartx + scrollstartwidth, scrollstarty, scrollstartx + scrollstartwidth, scrollstarty + scrollstartheight, blackColor);
			rectfill(g_ScreenBuffer, scrollstartx + scrollbargap, scrollstarty + scrollstartarrowheight + scrollbargap + ((topitem * (scrollstartheight - (2 * (scrollstartarrowheight + scrollbargap)))) / currentitemcount), scrollstartx + scrollstartwidth - scrollbargap, scrollstarty + scrollstartarrowheight - scrollbargap + (((topitem + maxitemsinlist) * (scrollstartheight - (2 * (scrollstartarrowheight + scrollbargap)))) / currentitemcount), blackColor);
			
		//	rectfill(g_ScreenBuffer, scrollstartx, scrollstarty + ((topitem * scrollstartheight) / (currentitemcount /** (listitemheight + listitemgap)*/)), scrollstartx + scrollstartwidth, scrollstarty + (((topitem + maxitemsinlist) * scrollstartheight) / (currentitemcount /** (listitemheight + listitemgap)*/)), blackColor);
		}
		else
		{
			// no scroll bar if it is not needed...
		}
		
		// draw item info area
		rectfill(g_ScreenBuffer, iteminfox - 2, iteminfoy - 2, iteminfox + iteminfowidth + 2, iteminfoy + iteminfoheight + 2, blackColor);
		rectfill(g_ScreenBuffer, iteminfox, iteminfoy, iteminfox + iteminfowidth, iteminfoy + iteminfoheight, buttonColor);
		
		if (currentitemcount > 0)
		{
			if (!(g_ItemTemplates[currentitemlist[itemindex]].attributes & isMessage))
			{
				// draw static item info txt
				alfont_textout_ex(g_ScreenBuffer, g_DialogFont, iteminfoarmortxt, iteminfoarmorx, iteminfoarmory, blackColor, -1);
				alfont_textout_ex(g_ScreenBuffer, g_DialogFont, iteminfodamagetxt, iteminfodamagex, iteminfodamagey, blackColor, -1);
				alfont_textout_ex(g_ScreenBuffer, g_DialogFont, iteminfospeedtxt, iteminfospeedx, iteminfospeedy, blackColor, -1);
				alfont_textout_ex(g_ScreenBuffer, g_DialogFont, iteminfochargestxt, iteminfochargesx, iteminfochargesy, blackColor, -1);
				alfont_textout_ex(g_ScreenBuffer, g_DialogFont, iteminfostaminatxt, iteminfostaminax, iteminfostaminay, blackColor, -1);
				alfont_textout_ex(g_ScreenBuffer, g_DialogFont, iteminfodamagehealedtxt, iteminfodamagehealedx, iteminfodamagehealedy, blackColor, -1);
				
				alfont_textprintf_ex(g_ScreenBuffer, g_DialogFont, iteminfoarmorvaluex, iteminfoarmorvaluey, blackColor, -1, "%d", g_ItemTemplates[currentitemlist[itemindex]].armor);
				alfont_textprintf_ex(g_ScreenBuffer, g_DialogFont, iteminfodamagevaluex, iteminfodamagevaluey, blackColor, -1, "%d", g_ItemTemplates[currentitemlist[itemindex]].damage);
				alfont_textprintf_ex(g_ScreenBuffer, g_DialogFont, iteminfospeedvaluex, iteminfospeedvaluey, blackColor, -1, "%d", g_ItemTemplates[currentitemlist[itemindex]].speed);
				alfont_textprintf_ex(g_ScreenBuffer, g_DialogFont, iteminfochargesvaluex, iteminfochargesvaluey, blackColor, -1, "%d", g_ItemTemplates[currentitemlist[itemindex]].charges);
				alfont_textprintf_ex(g_ScreenBuffer, g_DialogFont, iteminfostaminavaluex, iteminfostaminavaluey, blackColor, -1, "%d", g_ItemTemplates[currentitemlist[itemindex]].stamina);
				alfont_textprintf_ex(g_ScreenBuffer, g_DialogFont, iteminfodamagehealedvaluex, iteminfodamagehealedvaluey, blackColor, -1, "%d", g_ItemTemplates[currentitemlist[itemindex]].damageHealed);
			}
			
			// draw selected item's info
			draw_sprite(g_ScreenBuffer, g_LargeIcons[currentitemlist[itemindex]], iteminfoiconx, iteminfoicony);
			alfont_textout_ex(g_ScreenBuffer, g_DialogFont, g_ItemTemplates[currentitemlist[itemindex]].name, iteminfonamex, iteminfonamey, blackColor, -1);
			textout_paragraph_ex(g_ScreenBuffer, g_DialogFont, g_ItemTemplates[currentitemlist[itemindex]].description, iteminfodescx, iteminfodescy, blackColor, -1, iteminfodescwidth);
		}
		
		rectfill(g_ScreenBuffer, moneyamountx, moneyamounty + textheight, moneyamountx + iteminfowidth, moneyamounty - textheight, grayColor);
		alfont_textprintf_ex(g_ScreenBuffer, g_DialogFont, moneyamountx, moneyamounty, blackColor, -1, "You have $%d.", g_Saric.money);
		
		// draw button list at the bottom
		rectfill(g_ScreenBuffer, donebuttonx - 2, donebuttony - 2, donebuttonx + donebuttonwidth + 2, donebuttony + buttonheight + 2, blackColor);
		rectfill(g_ScreenBuffer, donebuttonx, donebuttony, donebuttonx + donebuttonwidth, donebuttony + buttonheight, (pressedButton == 3 || (pressedMouse == 3 && insideDone)) ? buttonPressedColor : buttonColor);
		alfont_textout_ex(g_ScreenBuffer, g_Font, donebuttontxt, donebuttonx + buttonborder + ((donebuttonwidth - (2 * buttonborder) - donebuttontxtwidth) / 2), donebuttony + buttonborder + TEXT_HEIGHT_EXTRA, blackColor, -1);
		
		rectfill(g_ScreenBuffer, statsbuttonx - 2, statsbuttony - 2, statsbuttonx + statsbuttonwidth + 2, statsbuttony + buttonheight + 2, blackColor);
		rectfill(g_ScreenBuffer, statsbuttonx, statsbuttony, statsbuttonx + statsbuttonwidth, statsbuttony + buttonheight, (pressedButton == 2 || (pressedMouse == 2 && insideStats)) ? buttonPressedColor : buttonColor);
		alfont_textout_ex(g_ScreenBuffer, g_Font, statsbuttontxt, statsbuttonx + buttonborder + ((statsbuttonwidth - (2 * buttonborder) - statsbuttontxtwidth) / 2), statsbuttony + buttonborder + TEXT_HEIGHT_EXTRA, blackColor, -1);
		
		rectfill(g_ScreenBuffer, itemsbuttonx - 2, itemsbuttony - 2, itemsbuttonx + itemsbuttonwidth + 2, itemsbuttony + buttonheight + 2, blackColor);
		rectfill(g_ScreenBuffer, itemsbuttonx, itemsbuttony, itemsbuttonx + itemsbuttonwidth, itemsbuttony + buttonheight, (pressedButton == 1 || (pressedMouse == 1 && insideItems)) ? buttonPressedColor : buttonColor);
		alfont_textout_ex(g_ScreenBuffer, g_Font, itemsbuttontxt, itemsbuttonx + buttonborder + ((itemsbuttonwidth - (2 * buttonborder) - itemsbuttontxtwidth) / 2), itemsbuttony + buttonborder + TEXT_HEIGHT_EXTRA, blackColor, -1);
		
		if (currentitemlist == storelist)
		{
			rectfill(g_ScreenBuffer, buybuttonx - 2, buybuttony - 2, buybuttonx + buybuttonwidth + 2, buybuttony + buttonheight + 2, blackColor);
			rectfill(g_ScreenBuffer, buybuttonx, buybuttony, buybuttonx + buybuttonwidth, buybuttony + buttonheight, (pressedButton == 4 || (pressedMouse == 4 && insideBuy)) ? buttonPressedColor : buttonColor);
			alfont_textout_ex(g_ScreenBuffer, g_Font, buybuttontxt, buybuttonx + buttonborder + ((buybuttonwidth - (2 * buttonborder) - buybuttontxtwidth) / 2), buybuttony + buttonborder + TEXT_HEIGHT_EXTRA, blackColor, -1);
		}
		else
		{
			rectfill(g_ScreenBuffer, buybuttonx - 2, buybuttony - 2, buybuttonx + buybuttonwidth + 2, buybuttony + buttonheight + 2, grayedOutBackgroundColor);
			rectfill(g_ScreenBuffer, buybuttonx, buybuttony, buybuttonx + buybuttonwidth, buybuttony + buttonheight, buttonColor);
			alfont_textout_ex(g_ScreenBuffer, g_Font, buybuttontxt, buybuttonx + buttonborder + ((buybuttonwidth - (2 * buttonborder) - buybuttontxtwidth) / 2), buybuttony + buttonborder + TEXT_HEIGHT_EXTRA, grayedOutTextColor, -1);
		}
		
		drawBufferToScreen();
		
		if (flag)
		{
			waitForSpecificInput(INPUT_KEY | INPUT_MOUSE_BUTTONS | INPUT_MOUSE_POSITION);
		}
		else
		{
			updateSound();
			rest(NUM_MILLISECONDS_BETWEEN_REFRESH * 3);
		}
	}
	
	if (storelist)
	{
		free(storelist);
	}
	
	if (mylist)
	{
		free(mylist);
	}
	
	return ret;
	
#else
	return dialogClose;
#endif
}

void gameDialog(short display, short param)
{
	// we can't go into a store
	// unless this is one.
	Boolean canDoStore = (display == dialogStore);	
	
	// this is MUCH better than a goto
	Boolean flag = true;
	
	set_mouse_sprite(g_CursorBitmap);
	set_mouse_sprite_focus(4, 2);
	show_mouse(screen);
	
	while (flag)
	{
		switch (display)
		{
		case dialogClose:
			flag = false;
			break;
			
		case dialogStats:
			display = doStatsDialog(canDoStore);
			
			if (!canDoStore && display == dialogStore)
			{
				display = dialogClose;			// safety measure
			}
			
			break;
			
		case dialogStore:
			display = doStoreDialog(param);
			break;
			
		case dialogItems:
			display = doItemsDialog(canDoStore);
			
			if (!canDoStore && display == dialogStore)
			{
				display = dialogClose;			// safety measure
			}
			
			break;
		}
	}
	
	show_mouse(NULL);
}

