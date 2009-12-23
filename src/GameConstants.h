/*
 *  GameConstants.h
 *  Mantra
 *
 *  Created by Chris O'Neill on 1/14/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#if defined(MANTRA_WINDOWS)
#include <allegro.h>
#else
#include <Allegro/allegro.h>
#endif

#include "alfont.h"

#include "GameDefines.h"
#include "GameTypes.h"

// Types
struct MapScreen
{
	// map[x][y] = tiles[(x * 10) + y]
	MapItem tiles[160];
	Enemy enemies[16];
};

typedef struct MapScreen MapScreen;

// Global Constants (will try to keep these minimal)

// loaded data
extern PALETTE g_SystemPalette;
extern int g_BlackColor;
extern int g_RedColor;

extern int g_CurrentScreenX;
extern int g_CurrentScreenY;

extern MapScreen g_MapScreens[NUM_SCREENS];
extern RegionCell g_MapRegions[NUM_SCREENS];

extern BITMAP* g_TileIcons[NUM_TILE_ICONS];
extern BITMAP* g_BossIcons[NUM_BOSS_ICONS];
extern BITMAP* g_EnemyIcons[NUM_ENEMY_ICONS];
extern BITMAP* g_SaricIcons[NUM_SARIC_ICONS];
extern BITMAP* g_SwordIcons[NUM_SWORD_ICONS];

extern unsigned char g_BossMasks[4096 * NUM_BOSS_ICONS];
extern unsigned char g_EnemyMasks[1024 * NUM_ENEMY_ICONS];
extern unsigned char g_SaricMask[1024];
extern unsigned char g_SwordMasks[NUM_SWORD_ICONS * 1024];
extern unsigned char g_TileMasks[NUM_TILE_ICONS * 1024];

extern Item g_ItemTemplates[NUM_ITEM_TEMPLATES];

extern BITMAP* g_SmallIcons[NUM_ITEM_TEMPLATES];
extern BITMAP* g_LargeIcons[NUM_ITEM_TEMPLATES];

extern BITMAP* g_ImmunityIcons[NUM_IMMUNITY_ICONS];

extern Enemy g_TmplEnemies[NUM_TMPL_ENEMIES];

extern Str255 g_Messages[NUM_MESSAGES];

// current screen info
extern int g_CurrentScreen;

extern EnemyHandle g_FirstEnemy, g_CurrentEnemy;
extern int g_EnemiesInRoom;

// saric
extern Saric g_Saric;

// 1 is alive, 0 is dead
extern unsigned short g_DeathRecord[NUM_SCREENS];

// screen buffer, should make updates MUCH faster...
extern BITMAP *g_ScreenBuffer;

extern ALFONT_FONT *g_Font;
extern ALFONT_FONT *g_DialogFont;
extern ALFONT_FONT *g_LargeFont;

extern Store g_Stores[NUM_STORES];

extern BITMAP *g_SwordAnimData[NUM_SWORD_FRAMES];

// global bitmaps
extern BITMAP *g_WinBitmap;
extern BITMAP *g_LoseBitmap;
extern BITMAP *g_StoryBitmap;
extern BITMAP *g_MantraBitmap;
extern BITMAP *g_CursorBitmap;
extern BITMAP *g_HelpBitmap;

// non graphics related
extern char g_GameInProgress;
extern char g_GameDirty;
extern volatile LONG_LONG g_GameplayTime;
extern SavedGame g_SavedGames[MAX_SAVED_GAMES];


// legacy trig funcs in LUTs (really need to fix this...)
extern short sineof[256];
extern short cosof[256];

#endif

