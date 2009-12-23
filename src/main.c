/*
 *	Mantra
 *
 *	Created by Chris O'Neill on 1/6/07.
 *	Copyright (c) 2007 __MyCompanyName__. All rights reserved.
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
#include "LoadData.h"
#include "Sound.h"
#include "Enemies.h"
#include "EnemyUpdate.h"
#include "OpeningScreen.h"
#include "Map.h"
#include "Input.h"
#include "Saric.h"
#include "Tests.h"


int initGameData(void)
{
	if (!createWindow())
	{
		printf("Unable to create window!\n");
		return 0;
	}
	
//	createTempFiles();
	packfile_password("musicman3320");
	
	if (!readItemData())
	{
		printf("Unable to read item data!\n");
	//	deleteTempFiles();
		freeItemData();
		return 0;
	}
	
	if (!readMapData())
	{
		printf("Unable to read map data!\n");
	//	deleteTempFiles();
		freeItemData();
		freeMapData();
		return 0;
	}
	
	if (!readTmplEnemyData())
	{
		printf("Unable to read tmpl enemy data!\n");
	//	deleteTempFiles();
		freeItemData();
		freeMapData();
		freeTmplEnemyData();
		return 0;
	}
	
	if (!readMessageData())
	{
		printf("Unable to read message data!\n");
	//	deleteTempFiles();
		freeItemData();
		freeMapData();
		freeTmplEnemyData();
		freeMessageData();
		return 0;
	}
	
	if (!readGraphicsData())
	{
		printf("Unable to read graphics data!\n");
	//	deleteTempFiles();
		freeItemData();
		freeMapData();
		freeTmplEnemyData();
		freeMessageData();
		freeGraphicsData();
		return 0;
	}
	
	if (!readFontData())
	{
		printf("Unable to read font data!\n");
	//	deleteTempFiles();
		freeItemData();
		freeMapData();
		freeTmplEnemyData();
		freeMessageData();
		freeGraphicsData();
		freeFontData();
		return 0;
	}
	
	if (!readStoreData())
	{
		printf("Unable to read store data!\n");
	//	deleteTempFiles();
		freeItemData();
		freeMapData();
		freeTmplEnemyData();
		freeMessageData();
		freeGraphicsData();
		freeFontData();
		freeStoreData();
		return 0;
	}
	
	if (!readAnimData())
	{
		printf("Unable to read anim data!\n");
	//	deleteTempFiles();
		freeItemData();
		freeMapData();
		freeTmplEnemyData();
		freeMessageData();
		freeGraphicsData();
		freeFontData();
		freeStoreData();
		freeAnimData();
		return 0;
	}
	
	if (!readImagesData())
	{
		printf("Unable to read images data!\n");
	//	deleteTempFiles();
		freeItemData();
		freeMapData();
		freeTmplEnemyData();
		freeMessageData();
		freeGraphicsData();
		freeFontData();
		freeStoreData();
		freeAnimData();
		freeImagesData();
		return 0;
	}
	
	if (!initSoundData())
	{
		printf("Unable to init sound data!\n");
	//	deleteTempFiles();
		freeItemData();
		freeMapData();
		freeTmplEnemyData();
		freeMessageData();
		freeGraphicsData();
		freeFontData();
		freeStoreData();
		freeAnimData();
		freeImagesData();
		freeSoundData();
		return 0;
	}
	
	if (!readSavedGameData())
	{
		printf("Unable to init saved game data!\n");
	//	deleteTempFiles();
		freeItemData();
		freeMapData();
		freeTmplEnemyData();
		freeMessageData();
		freeGraphicsData();
		freeFontData();
		freeStoreData();
		freeAnimData();
		freeImagesData();
		freeSoundData();
		freeSavedGameData();
		return 0;
	}
	
//	createSavedGameData();
	
	initInput();
	
//	deleteTempFiles();
	
	return 1;
}

void timerInc(void)
{
	g_GameplayTime++;
}
END_OF_FUNCTION(timerInc)

int main(int argc, const char *argv[])
{
//	PALETTE tempPalette;
	int first, fade;
	
	allegro_init();
	install_keyboard();
	install_mouse();
	install_timer();

	if (!initGameData())
	{
		printf("Init failed!\n");
		return 1;
	}
	/*
	clear_to_color(screen, makecol(255, 255, 255));
	
	g_CurrentScreenX = 12;
	g_CurrentScreenY = 7;
	
	loadScreen((g_CurrentScreenY * 16) + g_CurrentScreenX);
	
//	if (!openGame("/Users/chris/Documents/Development/Mantra/NewSource/Saved Games/fake"))
//	if (!openGame("/Users/chris/Documents/Development/Mantra/NewSource/Saved Games/First Boss"))
//	if (!openGame("/Users/chris/Documents/Development/Mantra/NewSource/Saved Games/map test"))
	if (!openGame("/Users/chris/Documents/Development/Mantra/NewSource/Saved Games/End of the Game"))
	{
		printf("Unable to open game!\n");
		
		g_CurrentScreenX = 12;
		g_CurrentScreenY = 7;
		
		loadScreen((g_CurrentScreenY * 16) + g_CurrentScreenX);
	}
	*/
	LOCK_FUNCTION(timerInc);
	LOCK_VARIABLE(g_GameplayTime);
	
	first = 1;
	g_GameDirty = 0;
	g_GameplayTime = 0;
	
	// need a loop that loads screens with input
	for(;;)
	{
		if (!showOpeningScreen(first))
		{
			break;
		}
		
		first = 0;
		g_GameInProgress = 1;
		fade = 1;
		g_GameDirty = 1;
		/*
		for(i = 0;i < FADE_LENGTH;i++)
		{
			fade_interpolate(g_SystemPalette, black_palette, tempPalette, (i * 64) / FADE_LENGTH, 0, 255);
			set_palette(tempPalette);
			
			rest(NUM_MILLISECONDS_BETWEEN_REFRESH / 2);
		}
		
		clear_to_color(g_ScreenBuffer, g_BlackColor);
		drawBufferToScreen();
	//	set_palette(g_SystemPalette);
		*/
		playMusic(g_MapRegions[g_CurrentScreen].musicIndex);
		
		install_int(timerInc, 1000);
		
		// this is the busy loop that updates enemies:
		for (;;) {
			// get input
			if (!checkKeys())
			{
				break;
			}
			
			// update enemies
			updateEnemies();
			
			// check for game done
			if (!g_GameInProgress)
			{
				break;
			}
			
			if (fade)
			{
				// draw screen and fade
				displayCurrentMapScreenBlit(true, false);
				fadeFromBlack();
			}
			else
			{
				// draw screen and blit
				displayCurrentMapScreen();
				drawBufferToScreen();
			}
			
			fade = 0;
			
			// update sound
			updateSound();
			
			// pause for a bit...
			rest(NUM_MILLISECONDS_BETWEEN_REFRESH);
		}
		
		/*
		for(i = 0;i < FADE_LENGTH;i++)
		{
			fade_interpolate(g_SystemPalette, black_palette, tempPalette, (i * 64) / FADE_LENGTH, 0, 255);
			set_palette(tempPalette);
			
			rest(NUM_MILLISECONDS_BETWEEN_REFRESH / 2);
		}
		
		set_palette(g_SystemPalette);
		*/
		fadeToBlack();
		stopMusic();
		remove_int(timerInc);
	}
	
	showCredits();
	
	freeItemData();
	freeMapData();
	freeTmplEnemyData();
	freeMessageData();
	freeGraphicsData();
	freeFontData();
	freeStoreData();
	freeAnimData();
	freeImagesData();
	freeSoundData();
	freeSavedGameData();
	
	remove_keyboard();
	remove_mouse();
	remove_timer();
	
	return 0;
}
END_OF_MAIN();

