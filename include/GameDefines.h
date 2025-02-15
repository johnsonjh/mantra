/*
 *  GameDefines.h
 *  Mantra
 *
 *  Created by Chris O'Neill on 1/14/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GAME_DEFINES_H
#define GAME_DEFINES_H

#define NUM_TILE_ICONS 200
#define NUM_SARIC_ICONS 16
#define NUM_ENEMY_ICONS 187
#define NUM_BOSS_ICONS 51
#define NUM_SWORD_ICONS 4
#define NUM_SWORD_FRAMES 1000

#define NUM_SCREENS (16*16)

#define NUM_ITEM_TEMPLATES 250
#define NUM_IMMUNITY_ICONS 10

#define NUM_TMPL_ENEMIES 250

#define NUM_MESSAGES 80

#define NUM_STORES 5

#define NUM_KEYS 30
#define NUM_MOUSE_BUTTONS 2

#define kBaseNextLevel 20
#define SWORD_OFFSET 16

#define MAX_ENEMIES_ON_SCREEN 16

#define NUM_MILLISECONDS_BETWEEN_REFRESH 40

#define STATS_HEIGHT 24

#define SCREEN_WIDTH (32*16)
#define SCREEN_HEIGHT ((32*10)+STATS_HEIGHT)

#define DIALOG_WIDTH  (SCREEN_WIDTH-(SCREEN_WIDTH/20))
#define DIALOG_HEIGHT (SCREEN_HEIGHT-STATS_HEIGHT-((SCREEN_HEIGHT-STATS_HEIGHT)/20))

#define TEXT_HEIGHT_EXTRA 2

#define FADE_LENGTH 24
#define FADE_REST_MILLISECONDS (NUM_MILLISECONDS_BETWEEN_REFRESH / 4)

#define MAX_SAVED_GAMES 4

#endif

