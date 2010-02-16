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

#if defined(MANTRA_WINDOWS)
//#include "StdAfx.h"
#include <allegro.h>
#else
#include <Allegro/allegro.h>
#endif

#include "GameConstants.h"
#include "Utils.h"
#include "Sound.h"
#include "Dialogs.h"
#include "Map.h"
#include "Enemies.h"
#include "EnemyCollision.h"
#include "Saric.h"
#include "Input.h"


// functions to fix: 
// NONE!!!  YAY!!!
// well, still have to do winGame, loseGame
// but those can be done later
// just some cool graphics stuff...

// functions fixed:
// checkKeys, killSaric, winGame, loseGame
// checkForRoomChange, handleTheRoomChange 
// runItemSpecialRoutine, 
// powerMantraItem, keySpecialItem


int g_PrevKeys[NUM_KEYS];
int g_NextKeys[NUM_KEYS];

int g_PrevMouseButtons[NUM_MOUSE_BUTTONS];
int g_NextMouseButtons[NUM_MOUSE_BUTTONS];

int g_PrevMouseX;
int g_NextMouseX;

int g_PrevMouseY;
int g_NextMouseY;

int g_PrevMouseVWheel;
int g_NextMouseVWheel;

//int g_PrevMouseHWheel;
//int g_NextMouseHWheel;

/*
 enum {
	 MANTRA_KEY_0 = 0,
	 MANTRA_KEY_1 = 1,
	 MANTRA_KEY_2 = 2,
	 MANTRA_KEY_3 = 3,
	 MANTRA_KEY_4 = 4,
	 MANTRA_KEY_5 = 5,
	 MANTRA_KEY_6 = 6,
	 MANTRA_KEY_7 = 7,
	 MANTRA_KEY_UP = 8,
	 MANTRA_KEY_DOWN = 9,
	 MANTRA_KEY_RIGHT = 10,
	 MANTRA_KEY_LEFT = 11,
	 MANTRA_KEY_S = 12 ,
	 MANTRA_KEY_I = 13,
	 MANTRA_KEY_Q = 14,
	 MANTRA_KEY_P = 15,
	 MANTRA_KEY_ESC = 16,
	 MANTRA_KEY_H = 17,
	 MANTRA_KEY_D = 18,
	 MANTRA_KEY_E = 19,
	 MANTRA_KEY_T = 20,
	 MANTRA_KEY_B = 21,
	 MANTRA_KEY_SPACE = 22,
	 MANTRA_KEY_ENTER = 23,
	 MANTRA_KEY_SHIFT = 24,
	 MANTRA_KEY_ALT = 25
 };
 */

void initInput(void)
{
	pollKeys();
	pollKeys();
	
	pollMouseButtons();
	pollMouseButtons();
	
	pollMousePosition();
	pollMousePosition();
}

#define POLL_KEY(thekey, actualkey) \
g_PrevKeys[thekey] = g_NextKeys[thekey];\
g_NextKeys[thekey] = key[actualkey];\
if (g_PrevKeys[thekey] != g_NextKeys[thekey]) \
changed = 1

#define POLL_KEYFLAG(thekey, actualflag) \
g_PrevKeys[thekey] = g_NextKeys[thekey];\
g_NextKeys[thekey] = key_shifts & actualflag;\
if (g_PrevKeys[thekey] != g_NextKeys[thekey]) \
changed = 1

#define POLL_MOUSE_BUTTON(thebutton, actualbutton) \
g_PrevMouseButtons[thebutton] = g_NextMouseButtons[thebutton];\
g_NextMouseButtons[thebutton] = mouse_b & actualbutton;\
if (g_PrevMouseButtons[thebutton] != g_NextMouseButtons[thebutton]) \
changed = 1

int pollKeys(void)
{
	int changed = 0;
	
	poll_keyboard();
	
	POLL_KEY(MANTRA_KEY_0, KEY_0);
	POLL_KEY(MANTRA_KEY_1, KEY_1);
	POLL_KEY(MANTRA_KEY_2, KEY_2);
	POLL_KEY(MANTRA_KEY_3, KEY_3);
	POLL_KEY(MANTRA_KEY_4, KEY_4);
	POLL_KEY(MANTRA_KEY_5, KEY_5);
	POLL_KEY(MANTRA_KEY_6, KEY_6);
	POLL_KEY(MANTRA_KEY_7, KEY_7);
	POLL_KEY(MANTRA_KEY_UP, KEY_UP);
	POLL_KEY(MANTRA_KEY_DOWN, KEY_DOWN);
	POLL_KEY(MANTRA_KEY_RIGHT, KEY_RIGHT);
	POLL_KEY(MANTRA_KEY_LEFT, KEY_LEFT);
	POLL_KEY(MANTRA_KEY_S, KEY_S);
	POLL_KEY(MANTRA_KEY_I, KEY_I);
	POLL_KEY(MANTRA_KEY_Q, KEY_Q);
	POLL_KEY(MANTRA_KEY_P, KEY_P);
	POLL_KEY(MANTRA_KEY_ESC, KEY_ESC);
	POLL_KEY(MANTRA_KEY_H, KEY_H);
	POLL_KEY(MANTRA_KEY_D, KEY_D);
	POLL_KEY(MANTRA_KEY_E, KEY_E);
	POLL_KEY(MANTRA_KEY_T, KEY_T);
	POLL_KEY(MANTRA_KEY_B, KEY_B);
	POLL_KEY(MANTRA_KEY_N, KEY_N);
	POLL_KEY(MANTRA_KEY_O, KEY_O);
	POLL_KEY(MANTRA_KEY_R, KEY_R);
	POLL_KEY(MANTRA_KEY_SPACE, KEY_SPACE);
	POLL_KEY(MANTRA_KEY_ENTER, KEY_ENTER);
	
	POLL_KEYFLAG(MANTRA_KEY_SHIFT, KB_SHIFT_FLAG);
	POLL_KEYFLAG(MANTRA_KEY_ALT, KB_ALT_FLAG);
	
	return changed;
}

int keyDown(int key)
{
	return g_NextKeys[key];
}

int keyUp(int key)
{
	return !g_NextKeys[key];
}

int keyPressed(int key)
{
	return g_NextKeys[key] && !g_PrevKeys[key];
}

int keyReleased(int key)
{
	return !g_NextKeys[key] && g_PrevKeys[key];
}

int anyKeyPressed(void)
{
	return (keyPressed(MANTRA_KEY_0) || 
		keyPressed(MANTRA_KEY_1) || 
		keyPressed(MANTRA_KEY_2) || 
		keyPressed(MANTRA_KEY_3) || 
		keyPressed(MANTRA_KEY_4) || 
		keyPressed(MANTRA_KEY_5) || 
		keyPressed(MANTRA_KEY_6) || 
		keyPressed(MANTRA_KEY_7) || 
		keyPressed(MANTRA_KEY_UP) || 
		keyPressed(MANTRA_KEY_DOWN) || 
		keyPressed(MANTRA_KEY_RIGHT) || 
		keyPressed(MANTRA_KEY_LEFT) || 
		keyPressed(MANTRA_KEY_S) || 
		keyPressed(MANTRA_KEY_I) || 
		keyPressed(MANTRA_KEY_Q) || 
		keyPressed(MANTRA_KEY_P) || 
		keyPressed(MANTRA_KEY_ESC) || 
		keyPressed(MANTRA_KEY_H) || 
		keyPressed(MANTRA_KEY_D) || 
		keyPressed(MANTRA_KEY_T) || 
		keyPressed(MANTRA_KEY_B) || 
		keyPressed(MANTRA_KEY_N) || 
		keyPressed(MANTRA_KEY_O) || 
		keyPressed(MANTRA_KEY_R) || 
		keyPressed(MANTRA_KEY_SPACE) || 
		keyPressed(MANTRA_KEY_ENTER) || 
		keyPressed(MANTRA_KEY_SHIFT) || 
		keyPressed(MANTRA_KEY_ALT));
}

int anyKeyReleased(void)
{
	return (keyReleased(MANTRA_KEY_0) || 
		keyReleased(MANTRA_KEY_1) || 
		keyReleased(MANTRA_KEY_2) || 
		keyReleased(MANTRA_KEY_3) || 
		keyReleased(MANTRA_KEY_4) || 
		keyReleased(MANTRA_KEY_5) || 
		keyReleased(MANTRA_KEY_6) || 
		keyReleased(MANTRA_KEY_7) || 
		keyReleased(MANTRA_KEY_UP) || 
		keyReleased(MANTRA_KEY_DOWN) || 
		keyReleased(MANTRA_KEY_RIGHT) || 
		keyReleased(MANTRA_KEY_LEFT) || 
		keyReleased(MANTRA_KEY_S) || 
		keyReleased(MANTRA_KEY_I) || 
		keyReleased(MANTRA_KEY_Q) || 
		keyReleased(MANTRA_KEY_P) || 
		keyReleased(MANTRA_KEY_ESC) || 
		keyReleased(MANTRA_KEY_H) || 
		keyReleased(MANTRA_KEY_D) || 
		keyReleased(MANTRA_KEY_T) || 
		keyReleased(MANTRA_KEY_B) || 
		keyReleased(MANTRA_KEY_N) || 
		keyReleased(MANTRA_KEY_O) || 
		keyReleased(MANTRA_KEY_R) || 
		keyReleased(MANTRA_KEY_SPACE) || 
		keyReleased(MANTRA_KEY_ENTER) || 
		keyReleased(MANTRA_KEY_SHIFT) || 
		keyReleased(MANTRA_KEY_ALT));
}

int keyHeld(int key)
{
	return g_NextKeys[key] && g_PrevKeys[key];
}

void waitForKeyPressed(void)
{
	pollKeys();
	pollMouseButtons();
	
	while(!anyKeyPressed() && !anyMouseButtonPressed())
	{
		updateSound();
		rest(NUM_MILLISECONDS_BETWEEN_REFRESH);
		pollKeys();
		pollMouseButtons();
	}
}

void waitForKeyTap(void)
{
	int initKeys[NUM_KEYS];
	int i, flag;
	
	for(i = 0;i < NUM_KEYS;i++)
	{
		initKeys[i] = g_NextKeys[i];
	}
	
	flag = 1;
	
	while(flag)
	{
		while(!anyKeyReleased())
		{
			updateSound();
			rest(NUM_MILLISECONDS_BETWEEN_REFRESH);
			pollKeys();
		}
		
		for(i = 0;i < NUM_KEYS;i++)
		{
			if (keyReleased(i))
			{
				if (initKeys[i])
				{
					initKeys[i] = 0;
				}
				else
				{
					flag = 0;
					break;
				}
			}
		}
		
		pollKeys();
	}
}

int pollMouseButtons(void)
{
	int changed = 0;
	
	poll_mouse();
	
	POLL_MOUSE_BUTTON(MANTRA_MOUSE_LEFT, 1);
	POLL_MOUSE_BUTTON(MANTRA_MOUSE_RIGHT, 2);
	
	return changed;
}

int pollMousePosition(void)
{
	int changed = 0;
	
	poll_mouse();
	
	g_PrevMouseX = g_NextMouseX;
	g_NextMouseX = mouse_x;
	
	if (g_PrevMouseX != g_NextMouseX)
	{
		changed = 1;
	}
	
	g_PrevMouseY = g_NextMouseY;
	g_NextMouseY = mouse_y;
	
	if (g_PrevMouseY != g_NextMouseY)
	{
		changed = 1;
	}
	
	return changed;
}

int pollMouseScrollWheel(void)
{
	int changed = 0;
	
	poll_mouse();
	/*
	g_PrevMouseHWheel = g_NextMouseHWheel;
	g_NextMouseHWheel = mouse_w;
	
	if (g_PrevMouseHWheel != g_NextMouseHWheel)
	{
		changed = 1;
	}
	*/
	g_PrevMouseVWheel = g_NextMouseVWheel;
	g_NextMouseVWheel = mouse_z;
	
	if (g_PrevMouseVWheel != g_NextMouseVWheel)
	{
		changed = 1;
	}
	
	return changed;
}

int mouseButtonDown(int button)
{
	return g_NextMouseButtons[button];
}

int mouseButtonUp(int button)
{
	return !g_NextMouseButtons[button];
}

int mouseButtonPressed(int button)
{
	return !g_PrevMouseButtons[button] && g_NextMouseButtons[button];
}

int mouseButtonReleased(int button)
{
	return g_PrevMouseButtons[button] && !g_NextMouseButtons[button];
}

int anyMouseButtonPressed(void)
{
	return (mouseButtonPressed(MANTRA_MOUSE_LEFT) || 
		mouseButtonPressed(MANTRA_MOUSE_RIGHT));
}

int anyMouseButtonReleased(void)
{
	return (mouseButtonReleased(MANTRA_MOUSE_LEFT) || 
		mouseButtonReleased(MANTRA_MOUSE_RIGHT));
}

int mouseButtonHeld(int button)
{
	return g_PrevMouseButtons[button] && g_NextMouseButtons[button];
}

int mouseMoved(void)
{
	/*
	return (g_PrevMouseHWheel != g_NextMouseHWheel || 
		g_PrevMouseVWheel != g_NextMouseVWheel);
	*/
	return g_PrevMouseVWheel != g_NextMouseVWheel;
}

int currentMouseX(void)
{
	return g_NextMouseX;
}

int currentMouseY(void)
{
	return g_NextMouseY;
}

int mousePositionInRect(int x1, int y1, int x2, int y2)
{
	return g_NextMouseX >= x1 && g_NextMouseX <= x2 && 
		g_NextMouseY >= y1 && g_NextMouseY <= y2;
}

void waitForSpecificInput(int thingsToWaitFor)
{
	while(!(((thingsToWaitFor & INPUT_KEY) && pollKeys()) || 
		((thingsToWaitFor & INPUT_MOUSE_BUTTONS) && pollMouseButtons()) || 
		((thingsToWaitFor & INPUT_MOUSE_POSITION) && pollMousePosition()) || 
		((thingsToWaitFor & INPUT_MOUSE_WHEEL) && pollMouseScrollWheel())))
	{
		updateSound();
		rest(NUM_MILLISECONDS_BETWEEN_REFRESH);
	}
}

void waitForAnyInput(void)
{
	waitForSpecificInput(INPUT_KEY | INPUT_MOUSE_BUTTONS | INPUT_MOUSE_POSITION | INPUT_MOUSE_WHEEL);
}

int checkKeys(void)
{
//	KeyMap			keyMap;
	Rect			oldRect, rect, oldEnemyRect, mapRect;
	EnemyPtr		next;
	EnemyHandle		nextHandle;
//	int			key;
	short			i, j, enemyModNum, m;
	Boolean			isRunning = false;
//	Boolean			done = false;

	checkForRoomChange();
	
//	GetKeys( keyMap );
	/*
	if (keypressed())
	{
		key = readkey();
	}
	else
	{
		key = 0;
	}
	*/
	pollKeys();	
	
	/**** Pauses the game ****/
	// The “P” Key, “esc” Key, “Q” Key
	if (keyPressed(MANTRA_KEY_Q) || keyPressed(MANTRA_KEY_P) || keyPressed(MANTRA_KEY_ESC))
	{
//		done = true;
		return 0;
	}
	
	/**** Help function ****/
	if(keyPressed(MANTRA_KEY_H))
	{
		showHelp();
//		GameHelp();						// stupid user
	}

#ifdef _Is_Developer_Version_
	/**** Developer’s aid, immortality ****/
	// The “D” Key
	if (keyPressed(MANTRA_KEY_D))
	{
		g_Saric.health = g_Saric.maxHealth;
		g_Saric.stamina = g_Saric.maxStamina;
		drawStats();
	}
	/*
	if( (keyMap[0] >> 26) & 0x01 )				// The “D” Key
	{	saric.health = saric.maxHealth;
		saric.stamina = saric.maxStamina;
		DrawStats();
	}
	*/
	
	/**** Developer’s aid, teleportation ****/
	if (keyPressed(MANTRA_KEY_T))
	{
		// teleport code here
		// we might implement this...
	}
	/*
	if( (keyMap[0] >> 9) & 0x01 )				// The “T” Key
	{	
		TeleportDialog();
	}
	*/
#endif
	
	/**** Goes to the Items list ****/
	if (keyPressed(MANTRA_KEY_I))
	{
		gameDialog(dialogItems, 0);
	}
	/*
	if( (keyMap[1] >> 26) & 0x01 )	// The “I” Key
	{	GameDialog(dialogItems, 0);
		theTicks = LMGetTicks();
	}
	*/
	
	/**** Turns off sound ****/
	if (keyPressed(MANTRA_KEY_0))
	{
		setMusicVolume(0);
	}
	/*
	if( (keyMap[0] >> 5) & 0x01 )	// The “0” Key
	{	SetSoundVol( 0 );
		gGameVolume = 0;
	}
	*/
	
	/**** Sets sound to 1 ****/
	if (keyPressed(MANTRA_KEY_1))
	{
		setMusicVolume(1);
	}
	/*
	if( (keyMap[0] >> 10) & 0x01 )	// The “1” Key
	{	SetSoundVol( 1 );
		gGameVolume = 1;
	}
	*/
	
	/**** Sets sound to 2 ****/
	if (keyPressed(MANTRA_KEY_2))
	{
		setMusicVolume(2);
	}
	/*
	if( (keyMap[0] >> 11) & 0x01 )	// The “2” Key
	{	SetSoundVol( 2 );
		gGameVolume = 2;
	}
	*/
	
	/**** Sets sound to 3 ****/
	if (keyPressed(MANTRA_KEY_3))
	{
		setMusicVolume(3);
	}
	/*
	if( (keyMap[0] >> 12) & 0x01 )	// The “3” Key
	{	SetSoundVol( 3 );
		gGameVolume = 3;
	}
	*/
	
	/**** Sets sound to 4 ****/
	if(keyPressed(MANTRA_KEY_4))
	{
		setMusicVolume(4);
	}
	/*
	if( (keyMap[0] >> 13) & 0x01 )	// The “4” Key
	{	SetSoundVol( 4 );
		gGameVolume = 4;
	}
	*/
	
	/**** Sets sound to 5 ****/
	if (keyPressed(MANTRA_KEY_5))
	{
		setMusicVolume(5);
	}
	/*
	if( (keyMap[0] >> 15) & 0x01 )	// The “5” Key
	{	SetSoundVol( 5 );
		gGameVolume = 5;
	}
	*/
	
	/**** Sets sound to 6 ****/
	if (keyPressed(MANTRA_KEY_6))
	{
		setMusicVolume(6);
	}
	/*
	if( (keyMap[0] >> 14) & 0x01 )	// The “6” Key
	{	SetSoundVol( 6 );
		gGameVolume = 6;
	}
	*/
	
	/**** Sets sound to 7 ****/
	if (keyPressed(MANTRA_KEY_7))
	{
		setMusicVolume(7);
	}
	/*
	if( (keyMap[0] >> 2) & 0x01 )	// The “7” Key
	{	SetSoundVol( 7 );
		gGameVolume = 7;
	}
	*/
	/**** draw vital stats ****/
	if (keyPressed(MANTRA_KEY_S))
	{
		gameDialog(dialogStats, 0);
	}
	/*
	if (keyMap[0] & 0x2000000)		// The “S” key
	{	GameDialog(dialogStats, 0);
	}
	*/
	
	// start by updating saric
	g_Saric.legCounter++;
	
	if(g_Saric.legCounter > 100)
	{
		g_Saric.legCounter = 0;
	}

	if (g_Saric.sitCounter > 30)		// 30 here could be a variable of how fast gains back
	{
		g_Saric.sitCounter = 0;
		
		if (g_Saric.stamina < g_Saric.maxStamina)
		{
			g_Saric.stamina++;
			drawStats();
		}
	}
	
	if (g_Saric.runCounter > 30)		// 30 here could be a variable of how fast loses
	{
		g_Saric.runCounter = 0;
		
		if (g_Saric.stamina)			// if he has any fight left in him
		{
			g_Saric.stamina--;
			drawStats();
		}
	}
	
	// how sad, he died...
	if(g_Saric.health <= 0)
	{
		killSaric();
		
		return 0;
	}
	
	// YAY!! we gained a level!!
	if( g_Saric.experience >= g_Saric.nextLevel )
	{
		levelUpSaric();
		drawStats();
	}
	
// this delays before Saric can recieve another wound
	if(g_Saric.woundCounter)
	{
		g_Saric.woundCounter++;
	}
	
	if(g_Saric.woundCounter > 30)
	{
		g_Saric.woundCounter = 0;
	}
	
// this kludge delays before Saric can recieve another message
	if(g_Saric.messageCounter)
	{
		g_Saric.messageCounter++;
	}
	
	if(g_Saric.messageCounter > 10)
	{
		g_Saric.messageCounter = 0;
	}
	
	//checks the squares upon which Saric stands	
	mapRect = g_Saric.where;
	
	for(i = mapRect.top / 32;i <= mapRect.bottom / 32;i++)
	{
		for(j = mapRect.left / 32;j <= mapRect.right / 32;j++)
		{
			if( j < 0 || j > 15 || i < 0 || i > 7 )
			{
				continue;
			}
			
			if( (g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].modifiers & doesDamage) && g_Saric.woundCounter == 0)
			{
				if( !(g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].modifiers & isDoor))
				{
					g_Saric.health -= g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].special;
					g_Saric.woundCounter = 1;
					
					if(g_Saric.health > g_Saric.maxHealth)
					{
						g_Saric.health = g_Saric.maxHealth;
					}
					
					drawStats();
				}
			}
		}
	}
	
/*then check all the rest of the keys*/
	oldRect = g_Saric.where;

//logicalXYZ is used here so that the sword operates independantly of graphics

	g_Saric.itemEffects[0].fireCounter++;
	
//	if( keyMap[1] & 0x200 )	//sword
	if (keyDown(MANTRA_KEY_SPACE))
	{
		m = g_Saric.itemEffects[0].spriteRef - 16000;		// Symantec's compiler Bug… Symantec Who?
		
		if(!g_Saric.hadHitEnemy && g_Saric.itemEquipped[m])
		{	
			if(g_Saric.itemEffects[0].fireCounter >= g_Saric.itemEffects[0].rateOfFire)
			{
				if(!g_Saric.logicalWasSwordOut)
				{
					if(g_Saric.stamina - g_Saric.itemEffects[0].stamina < 0 && g_Saric.itemEffects[0].stamina > 0)
					{
						// eww, goto... tsk tsk...
						goto notEnoughSpellPointsForWeapon;
					}
				}
				
				if(g_Saric.logicalWasSwordOut == false)
				{
					// play sound
					playSoundEffect(128);
//					PlaySndAsynchChannel( 128, kChan1, kHighPriority );	// Saric sound, chan 1
				}
				
				g_Saric.swordOut = true;
				
				if(!g_Saric.logicalWasSwordOut)
				{
					g_Saric.stamina -= g_Saric.itemEffects[0].stamina;
					
					if(g_Saric.itemEffects[0].attributes & hasCharges)
					{
						m = g_Saric.itemEffects[0].spriteRef - 16000;
						g_Saric.itemCharges[m]--;
						
						if(g_Saric.itemCharges[m] <= 0)
						{
							g_Saric.itemEffects[0].quantity--;
							g_Saric.itemQuantities[m]--;
							
							if(g_Saric.itemQuantities[m] <= 0)
							{
								g_Saric.itemEquipped[m] = false;
							}
							
							g_Saric.itemCharges[m] = g_ItemTemplates[m].charges;
						}
					}
					
					if(g_Saric.stamina > g_Saric.maxStamina)
					{
						g_Saric.stamina = g_Saric.maxStamina;
					}
					
					if(g_Saric.itemEffects[0].attributes & isMissile)
					{
						saricFireEnemy(g_Saric.itemEffects[0].firedMonsterID);
					}
					
					g_Saric.health += g_Saric.itemEffects[0].damageHealed;
					
					if(g_Saric.health > g_Saric.maxHealth)
					{
						g_Saric.health = g_Saric.maxHealth;
					}
					
					drawStats();
				}
				
			//	HLock((Handle)firstEnemy);
				nextHandle = g_FirstEnemy;
				i = g_Saric.damage + g_Saric.itemEffects[0].damage + g_Saric.itemEffects[2].damage;
				
				switch (g_Saric.facing)
				{
					case 0:
						rect.left = g_Saric.where.left - SWORD_OFFSET;
						rect.top = g_Saric.where.top;
						break;
					case 1:
						rect.left = g_Saric.where.left + SWORD_OFFSET;
						rect.top = g_Saric.where.top;
						break;
					case 2:
						rect.left = g_Saric.where.left;
						rect.top = g_Saric.where.top + SWORD_OFFSET;
						break;
					case 3:
						rect.left = g_Saric.where.left;
						rect.top = g_Saric.where.top - SWORD_OFFSET;
						break;
				}
				
				while((*nextHandle)->nextEnemy)
				{
					nextHandle = (*nextHandle)->nextEnemy;
				//	HLock((Handle)nextHandle);
					next = *nextHandle;
					
					enemyModNum = next->legState;
					
					if(next->attributes & isMultiFacing)
					{
						enemyModNum += (next->facing - 1) * 2;
						
						if(next->facing == 0)
						{
							enemyModNum += 2;
						}
					}
					
		// enemy legCounters are used as their woundCounters: the legcounter is set to 0 when hit
					if(next->legCounter > 15 && testIntercept((short)(next->spriteRef - 2000 + enemyModNum), (short)g_Saric.facing, &next->where, &rect, testEnemyIntercept, testSwordIntercept, next->attributes & isBossEnemy, 0))
					{
						oldEnemyRect = next->where;
						
						if(i - next->armorValue > 0 && (next->attributes & isEnemy) &&
							(next->attributes & killable) && CHECK_IMMUNITIES(next->immunities, g_Saric.damageType))
						{
							next->health -= (i - next->armorValue);
							
							if(next->health <= 0 && !(next->attributes & isMissile))
							{
								g_Saric.experience += next->xp;
								drawStats();
							}
							
							next->legCounter = 0;
							
							switch (g_Saric.facing)
							{
								case 0:
									next->where.left -= SWORD_OFFSET;
									next->where.right -= SWORD_OFFSET;
									break;
								case 1:
									next->where.left += SWORD_OFFSET;
									next->where.right += SWORD_OFFSET;
									break;
								case 2:
									next->where.top += SWORD_OFFSET;
									next->where.bottom += SWORD_OFFSET;
									break;
								case 3:
									next->where.top -= SWORD_OFFSET;
									next->where.bottom -= SWORD_OFFSET;
									break;
							}
							
							g_Saric.hadHitEnemy = true;
							
							// play sound
							playSoundEffect(130);
						//	PlaySndAsynchChannel( 130, kChan2, kHighPriority );	// Enemy sound, chan 2
							
							// we don't need this since
							// we pass it in now
							// YAY!!  less globals!!
						//	currentEnemy = nextHandle;
							
							if(!enemyStandableRect(nextHandle))
							{
								next->where = oldEnemyRect;
							}
						}
					}
					
		//			HUnlock((Handle)nextHandle);
				}
				
		//		HUnlock((Handle)firstEnemy);
			}
		}
	}
	else  //ie. !" "
	{
		g_Saric.hadHitEnemy = false;
	}
	
notEnoughSpellPointsForWeapon:
	if(g_Saric.logicalWasSwordOut && !g_Saric.swordOut)
	{
		g_Saric.itemEffects[0].fireCounter = 0;
	}
	
	g_Saric.logicalWasSwordOut = g_Saric.swordOut;
	
//other weapon

	g_Saric.itemEffects[1].fireCounter++;
	
	if (keyDown(MANTRA_KEY_SHIFT))
//	if( keyMap[1] & 0x01 )	//shift key: other weapon
	{
		m = g_Saric.itemEffects[1].spriteRef - 16000;
		
		if(g_Saric.itemEquipped[m])
		{
			if(g_Saric.itemEffects[1].fireCounter >= g_Saric.itemEffects[1].rateOfFire)
			{
				if(!g_Saric.logicalOffHandWasOut)
				{
					if(g_Saric.stamina - g_Saric.itemEffects[1].stamina < 0 && g_Saric.itemEffects[1].stamina > 0)
					{
						goto notEnoughSpellPointsForOffHand;
					}
				}

				if(!g_Saric.logicalOffHandWasOut)
				{
					// play sound
					playSoundEffect(128);
				//	PlaySndAsynchChannel( 128, kChan1, kHighPriority );	// Saric sound, chan 1
				}
				
				g_Saric.offHandOut = true;
				
				if(!g_Saric.logicalOffHandWasOut)
				{
					g_Saric.stamina -= g_Saric.itemEffects[1].stamina;
					
					if(g_Saric.itemEffects[1].attributes & hasCharges)
					{
						m = g_Saric.itemEffects[1].spriteRef - 16000;
						g_Saric.itemCharges[m]--;
						
						if(g_Saric.itemCharges[m] <= 0)
						{
							g_Saric.itemEffects[1].quantity--;
							g_Saric.itemQuantities[m]--;
							
							if(g_Saric.itemQuantities[m] <= 0)
							{
								g_Saric.itemEquipped[m] = false;
							}
							
							g_Saric.itemCharges[m] = g_ItemTemplates[m].charges;
						}
					}
					
					if(g_Saric.stamina > g_Saric.maxStamina)
					{
						g_Saric.stamina = g_Saric.maxStamina;
					}
					
					if(g_Saric.itemEffects[1].attributes & isMissile)
					{
						saricFireEnemy(g_Saric.itemEffects[1].firedMonsterID);
					}
					
					g_Saric.health += g_Saric.itemEffects[1].damageHealed;
					
					if(g_Saric.health > g_Saric.maxHealth)
					{
						g_Saric.health = g_Saric.maxHealth;
					}
					
					if(g_Saric.itemEffects[1].attributes & hasSpecialRoutine)
					{
						runItemSpecialRoutine((unsigned char)(g_Saric.itemEffects[1].spriteRef - 16000));
					}
					
					drawStats();
				}
			}
		}
	}
	else /* •• keyMap[1] &0x10 (offHand key) */
	{
		g_Saric.offHandOut = false;
	}
	
notEnoughSpellPointsForOffHand:
	if(g_Saric.logicalOffHandWasOut && !g_Saric.offHandOut)
	{
		g_Saric.itemEffects[1].fireCounter = 0;
	}
	
	g_Saric.logicalOffHandWasOut = g_Saric.offHandOut;
	
	if (/* key_shifts & KB_COMMAND_FLAG || */ key_shifts & KB_ALT_FLAG)
//	if(keyMap[1] & 0x8000)	//cmdKey
	{
		if (g_Saric.stamina > 0)
		{
			isRunning = true;
			g_Saric.speed = 6 + g_Saric.itemEffects[0].speed + g_Saric.itemEffects[1].speed + g_Saric.itemEffects[2].speed;
		}
		else		// else operate at normal speed w/o upping stamina
		{
			g_Saric.speed = 2 + g_Saric.itemEffects[0].speed + g_Saric.itemEffects[1].speed + g_Saric.itemEffects[2].speed;
		}
	}
	else
	{
		isRunning = false;
		g_Saric.sitCounter++;
		g_Saric.speed = 2 + g_Saric.itemEffects[0].speed + g_Saric.itemEffects[1].speed + g_Saric.itemEffects[2].speed;
	}
	
	// arrow keys
	// of course, the ONE thing they decide not to comment...
	// oh well, lets just hope I get the directions correct...
	if (keyDown(MANTRA_KEY_LEFT))
//	if( (keyMap[3] >> 3) & 0x01 || keyMap[2] & 0x4000)
	{
		if (isRunning)
		{
			g_Saric.runCounter++;
		}
		
		oldRect = g_Saric.where;
		offsetRect(&g_Saric.where, -g_Saric.speed, 0);
		
		if(!standableRect(g_Saric.where))
		{
			g_Saric.where = oldRect;
		}
		
		g_Saric.spriteRef = 1000 + g_Saric.legState * 4;
		g_Saric.facing = 0;
		if(g_Saric.legCounter > 4)
		{
			g_Saric.legState = !g_Saric.legState;
			g_Saric.legCounter = 0;
		}
		
		return 1;
	}
	
	if (keyDown(MANTRA_KEY_RIGHT))
//	if( (keyMap[3] >> 4) & 0x01 || keyMap[2] & 0x01)
	{
		if (isRunning)
		{
			g_Saric.runCounter++;
		}
		
		oldRect = g_Saric.where;
		offsetRect(&g_Saric.where, g_Saric.speed, 0);
		
		if(!standableRect(g_Saric.where))
		{
			g_Saric.where = oldRect;
		}
		
		g_Saric.spriteRef = 1001 + g_Saric.legState * 4;
		g_Saric.facing = 1;
		
		if(g_Saric.legCounter > 4)
		{
			g_Saric.legState = !g_Saric.legState;
			g_Saric.legCounter = 0;
		}
		
		return 1;
	}
	
	if (keyDown(MANTRA_KEY_DOWN))
//	if( (keyMap[3] >> 5) & 0x01 || keyMap[2] & 0x1000)
	{			
		if (isRunning)
		{
			g_Saric.runCounter++;
		}
		
		oldRect = g_Saric.where;
		offsetRect(&g_Saric.where, 0, g_Saric.speed);
		
		if(!standableRect(g_Saric.where))
		{
			g_Saric.where = oldRect;
		}
		
		g_Saric.spriteRef = 1002 + g_Saric.legState * 4;
		g_Saric.facing = 2;
		
		if( g_Saric.legCounter > 4 )
		{
			g_Saric.legState = !g_Saric.legState;
			g_Saric.legCounter = 0;
		}
		
		return 1;
	}
	
	if (keyDown(MANTRA_KEY_UP))
//	if( (keyMap[3] >> 6) & 0x01 || keyMap[2] & 0x8)
	{			
		if (isRunning)
		{
			g_Saric.runCounter++;
		}
		
		oldRect = g_Saric.where;
		offsetRect(&g_Saric.where, 0, -g_Saric.speed);
		if(!standableRect(g_Saric.where))
		{
			g_Saric.where = oldRect;
		}
		
		g_Saric.spriteRef = 1003 + g_Saric.legState * 4;
		g_Saric.facing = 3;
		
		if(g_Saric.legCounter > 4)
		{
			g_Saric.legState = !g_Saric.legState;
			g_Saric.legCounter = 0;
		}
		
		return 1;
	}
	
	return 1;
}

