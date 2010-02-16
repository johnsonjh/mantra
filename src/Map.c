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

#include <string.h>

#include "GameTypes.h"
#include "GameDefines.h"
#include "GameConstants.h"
#include "Utils.h"
#include "Sound.h"
#include "Enemies.h"
#include "EnemyCollision.h"
#include "Saric.h"

#include "Map.h"

void initMapStuff()
{
	int i;
	
	// make sure all enemies are alive
	for(i = 0;i < NUM_SCREENS;i++)
	{
		g_DeathRecord[i] = 0xFFFF;
	}
	
	// in case before 
	g_MapScreens[g_CurrentScreen].tiles[(6 * 10) + 9].modifiers |= standable;
	g_MapScreens[g_CurrentScreen].tiles[(7 * 10) + 9].modifiers |= standable;
	g_MapScreens[g_CurrentScreen].tiles[(8 * 10) + 9].modifiers |= standable;
	g_MapScreens[g_CurrentScreen].tiles[(9 * 10) + 9].modifiers |= standable;
	
	g_FirstEnemy = (EnemyHandle)mallocHandle(sizeof(Enemy));
	(*g_FirstEnemy)->nextEnemy = NULL;
	
//	printf("Size of MapItem: %d\n", sizeof(MapItem));
}

// this function is essentially loadMonsters()
void loadScreen(int index)
{
	EnemyHandle creationEnemy;
	EnemyPtr readingEnemy;
	
	int numEnemies = 0;
	
	int i;
	
	// remember which map we are on
	g_CurrentScreen = index;
	
	printf("New Screen Loaded: (%d, %d)\n", index % 16, index / 16);
	
	// free all enemies from the previous screen
	clearEnemies();
	
	g_CurrentEnemy = g_FirstEnemy;
	
	for(i = 0;i < MAX_ENEMIES_ON_SCREEN;i++)
	{
		readingEnemy = &g_MapScreens[g_CurrentScreen].enemies[i];
		// not all 16 enemies are used
		if (readingEnemy->spriteRef < 2000)
		{
//			printf("Sprite ref: %d\n", readingEnemy->spriteRef);
			break;
		}
		
	//	printf("Attributes: %lx, Immunities: %lx\n", readingEnemy->attributes, readingEnemy->immunities);
		
		//make sure the enemy can actually come back
		if (!(g_DeathRecord[g_CurrentScreen] & (1 << i)))
		{
			if (!(readingEnemy->attributes & permanent) || generateRand() & 0x0F)
			{
				continue;
			}
			else {
				g_DeathRecord[g_CurrentScreen] |= (1 << i);
			}

		}
		
		creationEnemy = (EnemyHandle)mallocHandle(sizeof(Enemy));
		
		(*creationEnemy)->previousEnemy = g_CurrentEnemy;
		(*(*creationEnemy)->previousEnemy)->nextEnemy = creationEnemy;
		
		(*creationEnemy)->originalPosition.h = 0;
		(*creationEnemy)->originalPosition.v = 0;
		
		(*creationEnemy)->where.left = readingEnemy->originalPosition.h * 32;
		(*creationEnemy)->where.right = (readingEnemy->originalPosition.h * 32) + 32;
		(*creationEnemy)->where.top = readingEnemy->originalPosition.v * 32;
		(*creationEnemy)->where.bottom = (readingEnemy->originalPosition.v * 32) + 32;
		
		(*creationEnemy)->legCounter = 0;
		(*creationEnemy)->legState = false;
		(*creationEnemy)->health = readingEnemy->health;
		(*creationEnemy)->armorValue = readingEnemy->armorValue;
		(*creationEnemy)->damage = readingEnemy->damage;
		(*creationEnemy)->attributes = readingEnemy->attributes;
		(*creationEnemy)->attributes |= originalToRoom;
		(*creationEnemy)->speed = readingEnemy->speed;
		(*creationEnemy)->movePhase = readingEnemy->movePhase;
		(*creationEnemy)->facing = readingEnemy->facing;
		(*creationEnemy)->spriteRef = readingEnemy->spriteRef;
		(*creationEnemy)->movementType = readingEnemy->movementType;
		(*creationEnemy)->pushableSpeed = readingEnemy->pushableSpeed;
		(*creationEnemy)->firedEnemy = readingEnemy->firedEnemy;
		(*creationEnemy)->xp = readingEnemy->xp;
		(*creationEnemy)->immunities = readingEnemy->immunities;
		(*creationEnemy)->damageType = readingEnemy->damageType;
		(*creationEnemy)->gaurdianRange = readingEnemy->gaurdianRange;
		(*creationEnemy)->target = readingEnemy->target;
		(*creationEnemy)->rateOfFire = readingEnemy->rateOfFire;
		(*creationEnemy)->deadItem = readingEnemy->deadItem;
		(*creationEnemy)->originalNumber = i;
		(*creationEnemy)->theta = 0;
		(*creationEnemy)->disFromUnitCircle = 100;
		(*creationEnemy)->angledCourse.h = 0;
		(*creationEnemy)->angledCourse.v = 0;
		(*creationEnemy)->messageID = readingEnemy->messageID;
		(*creationEnemy)->stuckCounter = 0;
		
		g_CurrentEnemy = creationEnemy;
		numEnemies++;
	}
	
	(*g_CurrentEnemy)->nextEnemy = NULL;
	
	g_EnemiesInRoom = numEnemies;
}

void drawScreenPreview(int game, BITMAP *bitmap)
{
	BITMAP *tempBitmap = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT - STATS_HEIGHT);
	
	int x, y;
	
//	clear_to_color(tempBitmap, makecol(0, 0, 0));
	
	// draw the tiles
	for(x = 0;x < 16;x++)
	{
		for (y = 0;y < 10;y++) 
		{
//			printf("Tile sprite ref: %d\n", g_MapScreens[g_CurrentScreen].tiles[(y * 16) + x].spriteRef);
			blit(g_TileIcons[g_MapScreens[(g_SavedGames[game].mapScreen.v * 16) + g_SavedGames[game].mapScreen.h].tiles[(x * 10) + y].spriteRef - 1000], tempBitmap, 0, 0, 32 * x, 32 * y, 32, 32);
		}
	}
	
	printf("Drew map!\n");
	
	// draw the enemies
//	drawEnemiesWithBitmap(tempBitmap);
	
	// display saric
//	drawSaric(tempBitmap);
	draw_sprite(tempBitmap, g_SaricIcons[g_SavedGames[game].saric.spriteRef - 1000 + (g_SavedGames[game].saric.wasSwordOut * 8)], g_SavedGames[game].saric.where.left, g_SavedGames[game].saric.where.top);
	
	printf("Drew Saric!\n");
	
	stretch_blit(tempBitmap, bitmap, 0, 0, tempBitmap->w, tempBitmap->h, 0, 0, bitmap->w, bitmap->h);
	
	destroy_bitmap(tempBitmap);
}

void displayCurrentMapScreen()
{
	displayCurrentMapScreenBlit(true, true);
}

void displayCurrentMapScreenSaric(Boolean drawSaric)
{
	displayCurrentMapScreenBlit(drawSaric, true);
}

void displayCurrentMapScreenBlit(Boolean drawSaricOnScreen, Boolean drawOnScreen)
{
	int x, y;
//	int tileIndex;
	
	// draw the tiles
	for(x = 0;x < 16;x++)
	{
		for (y = 0;y < 10;y++) 
		{
//			printf("Tile sprite ref: %d\n", g_MapScreens[g_CurrentScreen].tiles[(y * 16) + x].spriteRef);
			blit(g_TileIcons[g_MapScreens[g_CurrentScreen].tiles[(x * 10) + y].spriteRef - 1000], g_ScreenBuffer, 0, 0, 32 * x, 32 * y, 32, 32);
		}
	}
	
	// draw the enemies
	drawEnemies();
	
	// display saric
	if (drawSaricOnScreen)
	{
		drawSaric();
	}
	
	// update stats
	drawStats();
	
	if (drawOnScreen)
	{
		drawBufferToScreen();
	}
}

void blackenInward(int i, int max)
{
	int blackColor = makecol(0, 0, 0);
	
	rectfill(g_ScreenBuffer, 0, 0, SCREEN_WIDTH - 1, (i * SCREEN_HEIGHT) / max, blackColor);
	rectfill(g_ScreenBuffer, 0, 0, (i * SCREEN_WIDTH) / max, SCREEN_HEIGHT - 1, blackColor);
	rectfill(g_ScreenBuffer, 0, (SCREEN_HEIGHT) - 1 - ((i * SCREEN_HEIGHT) / max), SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, blackColor);
	rectfill(g_ScreenBuffer, SCREEN_WIDTH - 1 - ((i * SCREEN_WIDTH) / max), 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, blackColor);
}

void clearWindowInwards()
{
	int i;
	
	for(i = 0;i < 20;i++)
	{
		blackenInward(i, 20);
		
		drawBufferToScreen();
		updateSound();
		rest(NUM_MILLISECONDS_BETWEEN_REFRESH);
	}
}

void wipeWindowOutwards()
{
	BITMAP *backupBitmap;
	int i;
	
	backupBitmap = create_bitmap(16*32, 10*32);
	
	blit(g_ScreenBuffer, backupBitmap, 0, 0, 0, 0, 16*32, 10*32);
	
	for(i = 19;i >= 0;i--)
	{
		blit(backupBitmap, g_ScreenBuffer, 0, 0, 0, 0, 16*32, 10*32);
		blackenInward(i, 20);
		
		drawBufferToScreen();
		updateSound();
		rest(NUM_MILLISECONDS_BETWEEN_REFRESH);
	}
	
	destroy_bitmap(backupBitmap);
}

// 1 = down, 2 = up, 3 = left and 4 = right
void scrollFromScreenToScreen(int oldScreen, int newScreen, int direction)
{
	BITMAP *scrollBitmap;
	Rect oldRect = g_Saric.where;
	int i;
	
	if (direction == 1)
	{
		scrollBitmap = create_bitmap(16*32, 10*32*2);
		
	//	loadScreen(oldScreen);
		displayCurrentMapScreen(false, false);
		
		blit(g_ScreenBuffer, scrollBitmap, 0, 0, 0, 0, 16*32, 10*32);
		
		loadScreen(newScreen);
		displayCurrentMapScreen(false, false);
		
		blit(g_ScreenBuffer, scrollBitmap, 0, 0, 0, 10*32, 16*32, 10*32);
		
		for(i = 0;i < 10*32;i += 5)
		{
			blit(scrollBitmap, g_ScreenBuffer, 0, i, 0, 0, 16*32, 10*32);
			offsetRect(&g_Saric.where, 0, -5);
			drawSaric();
			drawStats();
			drawBufferToScreen();
			
			updateSound();
			rest(NUM_MILLISECONDS_BETWEEN_REFRESH / 4);
		}
		
		destroy_bitmap(scrollBitmap);
	}
	else if (direction == 2)
	{
		scrollBitmap = create_bitmap(16*32, 10*32*2);
		
	//	loadScreen(oldScreen);
		displayCurrentMapScreenBlit(false, false);
		
		blit(g_ScreenBuffer, scrollBitmap, 0, 0, 0, 10*32, 16*32, 10*32);
		
		loadScreen(newScreen);
		displayCurrentMapScreenBlit(false, false);
		
		blit(g_ScreenBuffer, scrollBitmap, 0, 0, 0, 0, 16*32, 10*32);
		
		for(i = 0;i < 10*32;i += 5)
		{
			blit(scrollBitmap, g_ScreenBuffer, 0, 10*32 - 1 - i, 0, 0, 16*32, 10*32);
			offsetRect(&g_Saric.where, 0, 5);
			drawSaric();
			drawStats();
			drawBufferToScreen();
			
			updateSound();
			rest(NUM_MILLISECONDS_BETWEEN_REFRESH / 4);
		}
		
		destroy_bitmap(scrollBitmap);
	}
	else if (direction == 3)
	{
		scrollBitmap = create_bitmap(16*32*2, 10*32);
		
	//	loadScreen(oldScreen);
		displayCurrentMapScreenBlit(false, false);
		
		blit(g_ScreenBuffer, scrollBitmap, 0, 0, 16*32, 0, 16*32, 10*32);
		
		loadScreen(newScreen);
		displayCurrentMapScreenBlit(false, false);
		
		blit(g_ScreenBuffer, scrollBitmap, 0, 0, 0, 0, 16*32, 10*32);
		
		for(i = 0;i < 16*32;i += 6)
		{
			blit(scrollBitmap, g_ScreenBuffer, 16*32 - 1 - i, 0, 0, 0, 16*32, 10*32);
			offsetRect(&g_Saric.where, 6, 0);
			drawSaric();
			drawStats();
			drawBufferToScreen();
			
			updateSound();
			rest(NUM_MILLISECONDS_BETWEEN_REFRESH / 4);
		}
		
		destroy_bitmap(scrollBitmap);
	}
	else if (direction == 4)
	{
		scrollBitmap = create_bitmap(16*32*2, 10*32);
		
	//	loadScreen(oldScreen);
		displayCurrentMapScreenBlit(false, false);
		
		blit(g_ScreenBuffer, scrollBitmap, 0, 0, 0, 0, 16*32, 10*32);
		
		loadScreen(newScreen);
		displayCurrentMapScreenBlit(false, false);
		
		blit(g_ScreenBuffer, scrollBitmap, 0, 0, 16*32, 0, 16*32, 10*32);
		
		for(i = 0;i < 16*32;i += 6)
		{
			blit(scrollBitmap, g_ScreenBuffer, i, 0, 0, 0, 16*32, 10*32);
			offsetRect(&g_Saric.where, -6, 0);
			drawSaric();
			drawStats();
			drawBufferToScreen();
			
			updateSound();
			rest(NUM_MILLISECONDS_BETWEEN_REFRESH / 4);
		}
		
		destroy_bitmap(scrollBitmap);
	}
	
	g_Saric.where = oldRect;
}

// Checks to see if more then 50% of Saric has left the room, or if he has touched a door
void checkForRoomChange()
{
	short		i, j;
	Rect		oldRect, mapRect;
	Boolean		stillOnDoor = false;
	Point		newLoc;
	int oldRoom;
	
	oldRect = g_Saric.where;
	
	// 1=down, 2=up, 3=left and 4=right
	
	if(g_Saric.where.bottom > (320 + 16))
	{
		handleTheRoomChange(1);
		
		return;
	}
	
	if(g_Saric.where.top < -16)
	{
		handleTheRoomChange(2);
		
		return;
	}
	
	if(g_Saric.where.left < -16)
	{
		handleTheRoomChange(3);
		
		return;
	}
	
	if(g_Saric.where.right > (512 + 16))
	{
		handleTheRoomChange(4);
		
		return;
	}
	
	mapRect = g_Saric.where;
	
	for(i = mapRect.top / 32;i <= mapRect.bottom / 32;i++)
	{
		for(j = mapRect.left / 32;j <= mapRect.right / 32;j++)
		{
			if(g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].modifiers & isDoor )
			{	
				if((abs(g_Saric.where.top - (i * 32)) < 12) && (abs(g_Saric.where.left - (j * 32)) < 12))
				{
					if(g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].modifiers & leadsToUnderWorld)
					{
						if(g_Saric.wasOnDoor)
						{
							stillOnDoor = true;
							break;
						}
						
						// this was not here before,
						// but might be necessary...
						clearWindowInwards();
						
						if(g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].modifiers & doesDamage)
						{
							g_Saric.woundCounter = 1;
							g_Saric.health -= g_MapScreens[g_CurrentScreen].tiles[(i * 10) + j].special;
						}
						
						if ((g_CurrentScreen / 16) < 8)
					//	if(currentRoom.v < 8)
						{
							g_CurrentScreen += 8 * 16;
						//	currentRoom.v += 8;
						}
						else
						{
							g_CurrentScreen -= 8 * 16;
						//	currentRoom.v -= 8;
						}
						
					//	SetRect(&g_Saric.where, j * 32, i * 32, j * 32 + 32, i * 32 + 32);
						
						g_Saric.where.left = j * 32;
						g_Saric.where.right = g_Saric.where.left + 32;
						g_Saric.where.top = i * 32;
						g_Saric.where.bottom = g_Saric.where.top + 32;
						
						loadScreen(g_CurrentScreen);
					//	OpenRoom(currentRoom.h,currentRoom.v);
						
						displayCurrentMapScreenBlit(true, false);
					//	DrawMap();
						
						// don't actually want to do this here
						// as I said, this is an experiment...
					//	drawBufferToScreen();
					//	CopyVirginToConstruct();
						
						drawStats();
						drawFrame();
						
					//	ClearWindInwards();
						
						wipeWindowOutwards();
					//	WidenOutUpdateWind();
						
						if(!standableRect(g_Saric.where))
						{
							clearWindowInwards();
							
							if((g_CurrentScreen / 16) < 8)
							{
								printf("Adding...\n");
								
								g_CurrentScreen += 8 * 16;
							}
							else
							{
								printf("Subtracting...\n");
								g_CurrentScreen -= 8 * 16;
							}
							
							loadScreen(g_CurrentScreen);
						//	OpenRoom(currentRoom.h,currentRoom.v);
							
							displayCurrentMapScreen(true, false);
						//	DrawMap();
							
						//	CopyVirginToConstruct();
							
							drawStats();
						//	ClearWindInwards();
							
							wipeWindowOutwards();
						//	WidenOutUpdateWind();
						}
						
						g_Saric.wasOnDoor = true;
					//	theTicks = LMGetTicks();
						
						// play music
						// actually updates the song
						// will implement later...
						
						// implemeting it now!!!
						playMusic(g_MapRegions[g_CurrentScreen].musicIndex);
					//	UpdateRoomRgn();
						return;
					}
					
					//and if it is a teleport:
					if(g_Saric.wasOnDoor)
					{
						stillOnDoor = true;
						
						break;
					}
					
					clearWindowInwards();
					
					oldRoom = g_CurrentScreen;
				//	oldRoom = currentRoom;
					oldRect = g_Saric.where;
					
					newLoc.h = (g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].special & 0xF0) >> 4;
					newLoc.v = g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].special & 0xF;
					
					g_CurrentScreen = (((g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].special & 0xF00) >> 8) * 16) + 
						((g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].special & 0xF000) >> 12);
					/*
					currentRoom.h = (map[j][i].special&0xF000) >> 12;
					currentRoom.v = (map[j][i].special&0xF00) >> 8;
					*/
					
					// done above the previous statements now...
					/*
					newLoc.h = (map[j][i].special&0xF0) >> 4;
					newLoc.v = map[j][i].special&0xF;
					*/
					
					g_Saric.where.left = newLoc.h * 32;
					g_Saric.where.right = g_Saric.where.left + 32;
					g_Saric.where.top = newLoc.v * 32;
					g_Saric.where.bottom = g_Saric.where.top + 32;
					
					loadScreen(g_CurrentScreen);
				//	OpenRoom(currentRoom.h,currentRoom.v);
					
					displayCurrentMapScreen(true, false);
				//	DrawMap();
					
				//	CopyVirginToConstruct();
					
					drawStats();
				//	ClearWindInwards();
				//	WidenOutUpdateWind();
					
					wipeWindowOutwards();
					
					if(!standableRect(g_Saric.where))
					{
						clearWindowInwards();
						
						g_CurrentScreen = oldRoom;
					//	currentRoom = oldRoom;
						
						g_Saric.where = oldRect;
						
						loadScreen(g_CurrentScreen);
					//	OpenRoom(currentRoom.h,currentRoom.v);
						
						displayCurrentMapScreenBlit(true, false);
					//	DrawMap();
						
					//	CopyVirginToConstruct();
						
						drawStats();
						
					//	ClearWindInwards();
					//	WidenOutUpdateWind();
						
						wipeWindowOutwards();
					}
					
					g_Saric.wasOnDoor = true;
				//	theTicks = LMGetTicks();
					
					// play music
					// actually updates the music, but
					// we will implement that later...
					
					// implemeting it now!!!
					playMusic(g_MapRegions[g_CurrentScreen].musicIndex);
				//	UpdateRoomRgn();
					return;
				}
			}
		}
	}
	
	g_Saric.wasOnDoor = stillOnDoor;

	return;
}

//1=down, 2=up, 3=left and 4=right
void handleTheRoomChange(char direction)
{
	Rect oldSaricRect;
	int nextScreen = g_CurrentScreen;
	
	// shouldn't need this anymore...
//	UpdateScreen();	// We update ourselves, so as not to get into trouble with
					// the icons not aligning correctly. A very necessary call.
	
	// Get the current status
	oldSaricRect = g_Saric.where;
	
	//	Remember: 1=down, 2=up, 3=left and 4=right
	
	// Change the room posistion accordingly
	if((direction == 1) && ((g_CurrentScreen / 16) < 16))
	{
		nextScreen += 16;
	}
	
	if((direction == 2) && ((g_CurrentScreen / 16) > 0))
	{
		nextScreen -= 16;
	}
	
	if((direction == 3) && ((g_CurrentScreen % 16) > 0))
	{
		nextScreen--;
	}
	
	if((direction == 4) && ((g_CurrentScreen % 16) < 16))
	{
		nextScreen++;
	}
	/*
	if((direction == 1) && (currentRoom.v < 16))
	{
		currentRoom.v++;
	}
	
	if((direction == 2) && (currentRoom.v > 0))
	{
		currentRoom.v--;
	}
	
	if((direction == 3) && (currentRoom.h > 0))
	{
		currentRoom.h--;
	}
	
	if((direction == 4) && (currentRoom.h < 16))
	{
		currentRoom.h++;
	}
	*/
	
//	OpenRoom( currentRoom.h, currentRoom.v );	// change the room
	
	scrollFromScreenToScreen(g_CurrentScreen, nextScreen, direction);
	
	//	Remember: 1=down, 2=up, 3=left and 4=right
	
	// Change where Saric is
	// I think I am going to
	// do this while scrolling...
	// changed my mind, its not working...
	switch( direction )
	{
		case 1:
			offsetRect(&g_Saric.where, 0, -320);
			break;
		case 2:
			offsetRect(&g_Saric.where, 0, 320);
			break;
		case 3:
			offsetRect(&g_Saric.where, 512, 0);
			break;
		case 4:
			offsetRect(&g_Saric.where, -512, 0);
			break;
	}
	/*
	DrawMap();					// update the room
	CopyVirginToConstruct();	// update the construction
	DrawFrame();				// update the screen
	
	UpdateScrollWindow( direction );
	*/
	g_CurrentScreen = nextScreen;
	
//	loadScreen(g_CurrentScreen);
	displayCurrentMapScreen();
	drawStats();
	
	// play music
	// this updates the music
	// but saved for later...
	
	// saved until now!!!
	playMusic(g_MapRegions[g_CurrentScreen].musicIndex);
//	UpdateRoomRgn();
	
//	theTicks = LMGetTicks();	// a lot of time has been spent doing this, but it won’t change
						// the frame skipping routines
	
	/*	This will check to see if the square in the next room can be stood on,
		if not, Saric will moved back more than 50% off-screen. The program should
		then relize this, and this function will be called again. It looks convincing
		enough, but this shouldn’t even happen if the maps are made correctly.
		This is just a safe guard.
	*/
	
	if(!standableRect(g_Saric.where))
	{
		g_Saric.where = oldSaricRect;	// Change back to the old
		
		//	Remember: 1=down, 2=up, 3=left and 4=right
		
		switch(direction)
		{
			case 1:
				offsetRect(&g_Saric.where, 0, (-320 - 20));
				break;
			case 2:
				offsetRect(&g_Saric.where, 0, (320 + 20));
				break;
			case 3:
				offsetRect(&g_Saric.where, (512 + 20), 0);
				break;
			case 4:
				offsetRect(&g_Saric.where, (-512 - 20), 0);
				break;
		}
	}
	
	// play music
	// this actually updates the music
	// we will implement this later...
	
	// implemeting it now!!!
	playMusic(g_MapRegions[g_CurrentScreen].musicIndex);
//	UpdateRoomRgn();		// update any MODs, or other Region stuff.
}

