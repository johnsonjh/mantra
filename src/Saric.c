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
#include "EnemyCollision.h"
#include "Saric.h"


void initSaric()
{
	short i;
	
	g_Saric.where.left = 256;
	g_Saric.where.top = 160;
	g_Saric.where.right = 288;
	g_Saric.where.bottom = 192;
	
	g_Saric.legCounter = 0;
	g_Saric.legState = false;
	g_Saric.oldPosition.h = 0;
	g_Saric.oldPosition.v = 0;
	g_Saric.spriteRef = 1000;
	g_Saric.health = 10;
	g_Saric.maxHealth = 10;
	g_Saric.armorValue = 0;
	g_Saric.damage = 1;
	g_Saric.facing = 0;
	g_Saric.speed = 2;
	g_Saric.woundCounter = 0;
	g_Saric.messageCounter = 0;
	g_Saric.wasOnDoor = false;
	g_Saric.experience = 0;
	g_Saric.nextLevel = kBaseNextLevel;
	g_Saric.level = 0;
	g_Saric.incrementalDamageCounter = 0;
	g_Saric.money = 0;
	g_Saric.stamina = 10;
	g_Saric.maxStamina = 10;
	g_Saric.sitCounter = 0;			//DJMplemented 6/18/94
	g_Saric.runCounter = 0;			//DJMplemented 6/18/94
	g_Saric.damageType = 0;
	g_Saric.immunities = 0;
	
	for(i=0;i<250;i++)
	{
		g_Saric.itemCharges[i] = g_ItemTemplates[i].charges;
		g_Saric.itemQuantities[i] = 0;			
		g_Saric.itemEquipped[i] = false;
	}
	
	for(i=0;i<3;i++)
	{
		g_Saric.itemEffects[i].name = NULL;
		g_Saric.itemEffects[i].description = NULL;
		g_Saric.itemEffects[i].attributes = 0;
		g_Saric.itemEffects[i].armor = 0;
		g_Saric.itemEffects[i].damage = 0;
		g_Saric.itemEffects[i].speed = 0;
		g_Saric.itemEffects[i].rateOfFire = 0;
		g_Saric.itemEffects[i].fireCounter = 0;
		g_Saric.itemEffects[i].charges = 0;
		g_Saric.itemEffects[i].stamina = 0;
		g_Saric.itemEffects[i].damageHealed = 0;
		g_Saric.itemEffects[i].quantity = 0;		// note this is used as a flag whether it exists
		g_Saric.itemEffects[i].spriteRef = 16000;
		g_Saric.itemEffects[i].firedMonsterID = 0;
	}
}

void levelUpSaric()
{
	g_Saric.health += 5;
	g_Saric.maxHealth += 5;
	g_Saric.level++;
	g_Saric.stamina += 5;
	g_Saric.maxStamina += 5;
	
	//adjust the required xp for next level, plateauing after 8th level
	if(g_Saric.nextLevel < kBaseNextLevel * 32)
	{
		g_Saric.nextLevel *= 2;
	}
	else
	{
		g_Saric.nextLevel += kBaseNextLevel * 32;
	}
}

void drawSword()
{
	BITMAP *sword;
	int x = 0, y = 0;
	
	// calculate sword location
	switch (g_Saric.facing)
	{	case 0:
		x = g_Saric.where.left - 16;
		y = g_Saric.where.top;
		break;
	case 1:
		x = g_Saric.where.left + 16;
		y = g_Saric.where.top;
		break;
	case 2:
		x = g_Saric.where.left;
		y = g_Saric.where.top + 16;
		break;
	case 3:
		x = g_Saric.where.left;
		y = g_Saric.where.top - 16;
		break;
	}
	
	g_Saric.oldSword.h = x;
	g_Saric.oldSword.v = y;
	
	// actually draw sword
	sword = g_SwordIcons[(int)g_Saric.facing];
	
	draw_sprite(g_ScreenBuffer, sword, x, y);
	
	// add any finishing touches (nothing for now)
}


void drawSaric()
{
	BITMAP *saric;
	int x, y;
	
	if (g_Saric.swordOut)
	{
		drawSword();
	}
	
	g_Saric.wasSwordOut = g_Saric.swordOut;
	g_Saric.swordOut = false;
	
	// draw saric
	x = g_Saric.where.left;
	y = g_Saric.where.top;
	
	saric = g_SaricIcons[g_Saric.spriteRef - 1000 + (g_Saric.wasSwordOut * 8)];
	
	draw_sprite(g_ScreenBuffer, saric, x, y);
	
	g_Saric.oldPosition.h = g_Saric.where.left;
	g_Saric.oldPosition.v = g_Saric.where.top;
}

void runItemSpecialRoutine(unsigned char itemNumber)
{
	switch(itemNumber)
	{
		case 104:
			powerMantraItem();
		case 150:
			keySpecialItem();
			break;
	}
}

void powerMantraItem()
{
	EnemyHandle	nextHandle;
	EnemyPtr	next;
	
	RGB yellowColor = {32, 64, 0};
	PALETTE yellowPalette;
	
	int i;
	
//	FadeToPalette(15,128);
//	FadeToPalette(15,8);
	
	for(i = 0;i < 256;i++)
	{
		yellowPalette[i] = yellowColor;
	}
	
	fade_from(g_SystemPalette, yellowPalette, 32);
	fade_from(yellowPalette, g_SystemPalette, 32);
	
//	HLock((Handle)firstEnemy);
	nextHandle = g_FirstEnemy;
	
	while((*nextHandle)->nextEnemy)
	{
		nextHandle = (*nextHandle)->nextEnemy;
	//	HLock((Handle)nextHandle);
		next = *nextHandle;
		
		switch(next->movementType)
		{
			case 0:
			case 58:
			case 13:
			case 15:
				break;
			default:
				if(next->attributes & killable && next->attributes & isEnemy)
				{
					next->health -= 20;
				}
				break;
		}
	}
}

void keySpecialItem()
{
	EnemyHandle	nextHandle;
	Rect	rect;
	EnemyPtr	next;
	short	enemyModNum;
	
//	HLock((Handle)firstEnemy);
	nextHandle = g_FirstEnemy;
	
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
		
		if(testIntercept((short)(next->spriteRef - 2000 + enemyModNum), (short)g_Saric.facing, &next->where, &rect, testEnemyIntercept, testSwordIntercept, next->attributes & isBossEnemy, 0))
		{
			if(next->movementType == doorEnemy)
			{
				next->health = 0;
				
				// play sound
				playSoundEffect(134);
			//	PlaySndAsynchChannel( 134, kChan3, kHighPriority );	// Item sound, chan 3
				
				g_Saric.itemQuantities[150]--;
				
				if(g_Saric.itemQuantities[150] <= 0)
				{
					g_Saric.itemEquipped[150] = false;
				}
			}
		}
	}
}

void killSaric()
{	
	loseGame();
	
//	done = true;
//	isGameCurrentlyOpen = false;
}


