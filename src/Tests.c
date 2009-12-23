/*
 *  Tests.c
 *  Mantra
 *
 *  Created by Chris O'Neill on 1/18/07.
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

#include "GameTypes.h"
#include "GameDefines.h"
#include "GameConstants.h"

#include "Tests.h"

///////////////////////
// test loaded palette
///////////////////////

void testPalette()
{
	int i;
	int x, y;
	
	for(i = 0;i < 256;i++)
	{
		x = (i % 16) * 20;
		y = (i / 16) * 20;
		rectfill(screen, x, y, x + 20, y + 20, i);
	}
	
	readkey();
}

///////////////////////
// test loaded bitmaps
///////////////////////


void testLoadedIconBitmaps()
{
	int i;
	int x, y;
	
	clear_to_color(screen, makecol(255, 255, 255));
	
	for (i = 0;i < NUM_IMMUNITY_ICONS;i++) {
		x = (i % 32) * 16;
		y = (i / 32) * 16;
		
		if (g_ImmunityIcons[i])
		{
			blit(g_ImmunityIcons[i], screen, 0, 0, x, y, 16, 16);
		}
		else
		{
			rectfill(screen, x, y, x + 16, y + 16, makecol(0, 0, 0));
		}
	}
	
	readkey();
	
	clear_to_color(screen, makecol(255, 255, 255));
	
	for (i = 0;i < NUM_ITEM_TEMPLATES;i++) {
		x = (i % 32) * 16;
		y = (i / 32) * 16;
		
		if (g_SmallIcons[i])
		{
			blit(g_SmallIcons[i], screen, 0, 0, x, y, 16, 16);
		}
		else
		{
			rectfill(screen, x, y, x + 16, y + 16, makecol(0, 0, 0));
		}
	}
	
	readkey();
	
	clear_to_color(screen, makecol(255, 255, 255));
	
	for (i = 0;i < NUM_ITEM_TEMPLATES;i++) {
		x = (i % 16) * 32;
		y = (i / 16) * 32;
		
		if (g_LargeIcons[i])
		{
			blit(g_LargeIcons[i], screen, 0, 0, x, y, 32, 32);
		}
		else {
			rectfill(screen, x, y, x + 32, y + 32, makecol(0, 0, 0));
		}

	}
	
	readkey();
}

void testLoadedTileBitmaps()
{
	int i;
	int x, y;
	
	clear_to_color(screen, makecol(255, 255, 255));
	
	for (i = 0;i < NUM_TILE_ICONS;i++) {
		x = (i % 16) * 32;
		y = (i / 16) * 32;
		
		blit(g_TileIcons[i], screen, 0, 0, x, y, 32, 32);
	}
	
	readkey();
}

void testLoadedEnemyBitmaps()
{
	int i;
	int x, y;
	
	clear_to_color(screen, makecol(255, 255, 255));
	
	for (i = 0;i < NUM_ENEMY_ICONS;i++) {
		x = (i % 16) * 32;
		y = (i / 16) * 32;
		
		blit(g_EnemyIcons[i], screen, 0, 0, x, y, 32, 32);
	}
	
	readkey();
}

void testLoadedSaricBitmaps()
{
	int i;
	int x, y;
	
	clear_to_color(screen, makecol(255, 255, 255));
	
	for (i = 0;i < NUM_SARIC_ICONS;i++) {
		x = (i % 16) * 32;
		y = (i / 16) * 32;
		
		blit(g_SaricIcons[i], screen, 0, 0, x, y, 32, 32);
	}
	
	readkey();
}

void testLoadedSwordBitmaps()
{
	int i;
	int x, y;
	
	clear_to_color(screen, makecol(255, 255, 255));
	
	for (i = 0;i < NUM_SWORD_ICONS;i++) {
		x = (i % 16) * 32;
		y = (i / 16) * 32;
		
		blit(g_SwordIcons[i], screen, 0, 0, x, y, 32, 32);
	}
	
	readkey();
}

void testLoadedBossBitmaps()
{
	int i;
	int x, y;
	
	clear_to_color(screen, makecol(255, 255, 255));
	
	for (i = 0;i < NUM_BOSS_ICONS;i++) {
		x = (i % 8) * 64;
		y = (i / 8) * 64;
		
		blit(g_BossIcons[i], screen, 0, 0, x, y, 64, 64);
	}
	
	readkey();
}

void testSwordAnimData()
{
	int i;
	
	for(i = 0;;i++)
	{
		if (!g_SwordAnimData[i])
		{
			i = 0;
		}
		
		blit(g_SwordAnimData[i], screen, 0, 0, 0, 0, g_SwordAnimData[i]->w, g_SwordAnimData[i]->h);
		
		readkey();
	}
}

void testLoadedBitmaps()
{
	// this is big enough to show all of one type of icon
	if (set_gfx_mode(GFX_AUTODETECT, 16*32, 16*32, 0, 0)) {
		allegro_message("Error setting 320x200x8 gfx mode:\n%s\n", allegro_error);
		return;
	}
	
	set_palette(g_SystemPalette);
	/*
	testPalette();
	testLoadedIconBitmaps();
	testLoadedEnemyBitmaps();
	testLoadedSaricBitmaps();
	testLoadedSwordBitmaps();
	testLoadedTileBitmaps();
	testLoadedBossBitmaps();
	*/
	testSwordAnimData();
}

/////////////////////
// test loaded masks
/////////////////////

void testLoadedTileMasks()
{
	int i, j, k;
	int basex, basey;
	
	clear_to_color(screen, makecol(255, 255, 255));
	
	for (i = 0;i < NUM_TILE_ICONS;i++) {
		basex = (i % 16) * 32;
		basey = (i / 16) * 32;
		
		for(j = 0;j < 32;j++)
		{
			for(k = 0;k < 32;k++)
			{
				putpixel(screen, basex + j, basey + k, g_TileMasks[(i * 1024) + (k * 32) + j] ? makecol(0, 0, 0) : makecol(255, 255, 255));
			}
		}
	}
	
	readkey();
}

void testLoadedEnemyMasks()
{
	int i, j, k;
	int basex, basey;
	
	clear_to_color(screen, makecol(255, 255, 255));
	
	for (i = 0;i < NUM_ENEMY_ICONS;i++) {
		basex = (i % 16) * 32;
		basey = (i / 16) * 32;
		
		for(j = 0;j < 32;j++)
		{
			for(k = 0;k < 32;k++)
			{
//				putpixel(screen, basex + j, basey + k, getpixel(g_EnemyIcons[i], j, k) ? makecol(0, 0, 0) : makecol(255, 255, 255));
				putpixel(screen, basex + j, basey + k, g_EnemyMasks[(i * 1024) + (k * 32) + j] ? makecol(0, 0, 0) : makecol(255, 255, 255));
			}
		}
	}
	
	readkey();
}

void testLoadedSaricMasks()
{
	int j, k;
//	int basex, basey;
	
	clear_to_color(screen, makecol(255, 255, 255));
	
	for(j = 0;j < 32;j++)
	{
		for(k = 0;k < 32;k++)
		{
			putpixel(screen, j, k, g_SaricMask[(k * 32) + j] ? makecol(0, 0, 0) : makecol(255, 255, 255));
		}
	}
	
	readkey();
}

void testLoadedSwordMasks()
{
	int i, j, k;
	int basex, basey;
	
	clear_to_color(screen, makecol(255, 255, 255));
	
	for (i = 0;i < NUM_SWORD_ICONS;i++) {
		basex = (i % 16) * 32;
		basey = (i / 16) * 32;
		
		for(j = 0;j < 32;j++)
		{
			for(k = 0;k < 32;k++)
			{
				putpixel(screen, basex + j, basey + k, g_SwordMasks[(i * 1024) + (k * 32) + j] ? makecol(0, 0, 0) : makecol(255, 255, 255));
			}
		}
	}
	
	readkey();
}

void testLoadedBossMasks()
{
	int i, j, k;
	int basex, basey;
	
	clear_to_color(screen, makecol(255, 255, 255));
	
	for (i = 0;i < NUM_BOSS_ICONS;i++) {
		basex = (i % 8) * 64;
		basey = (i / 8) * 64;
		
		for(j = 0;j < 64;j++)
		{
			for(k = 0;k < 64;k++)
			{
//				putpixel(screen, basex + j, basey + k, getpixel(g_BossIcons[i], j, k) ? makecol(0, 0, 0) : makecol(255, 255, 255));
				putpixel(screen, basex + j, basey + k, g_BossMasks[(i * 4096) + (k * 32) + j] ? makecol(0, 0, 0) : makecol(255, 255, 255));
			}
		}
	}
	
	readkey();
}

void testLoadedMasks()
{
	// this is big enough to show all of one type of icon
	if (set_gfx_mode(GFX_AUTODETECT, 16*32, 16*32, 0, 0)) {
		allegro_message("Error setting 320x200x8 gfx mode:\n%s\n", allegro_error);
		return;
	}
	
	set_palette(g_SystemPalette);
	
	testPalette();
	testLoadedEnemyMasks();
	testLoadedSaricMasks();
	testLoadedSwordMasks();
	testLoadedTileMasks();
	testLoadedBossMasks();
}

