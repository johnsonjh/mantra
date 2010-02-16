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
#include "EnemyUpdate.h"

void clearEnemies(void)
{
	EnemyHandle deleteEnemy = (*g_FirstEnemy)->nextEnemy;
	EnemyHandle nextEnemy;
	
	while (deleteEnemy) 
	{
		nextEnemy = (**deleteEnemy).nextEnemy;
		freeHandle((void **)deleteEnemy);
		deleteEnemy = nextEnemy;
	}
	
	(*g_FirstEnemy)->nextEnemy = NULL;
}

void drawEnemies(void)
{
	drawEnemiesWithBitmap(g_ScreenBuffer);
}

void drawEnemiesWithBitmap(BITMAP *bitmap)
{
	int tileIndex;
	
	EnemyHandle currentEnemy = (*g_FirstEnemy)->nextEnemy;
	
	while(currentEnemy)
	{
		tileIndex = (*currentEnemy)->spriteRef + (*currentEnemy)->legState - 2000;
		
		if ((*currentEnemy)->attributes & isMultiFacing)
		{
			tileIndex += ((*currentEnemy)->facing - 1) * 2;
			
			if ((*currentEnemy)->facing == 0)
			{
				tileIndex += 2;
			}
		}
		
		if ((*currentEnemy)->attributes&isBossEnemy)
		{
			draw_sprite(bitmap, g_BossIcons[tileIndex], (*currentEnemy)->where.left, (*currentEnemy)->where.top);
		}
		else {
			draw_sprite(bitmap, g_EnemyIcons[tileIndex], (*currentEnemy)->where.left, (*currentEnemy)->where.top);
		}
		
		(*currentEnemy)->originalPosition = *((Point *)&(*currentEnemy)->where);
		
		currentEnemy = (*currentEnemy)->nextEnemy;
	}
}

void killCurrentEnemy(EnemyHandle enemy)
{
	EnemyPtr temp;
	EnemyPtr curTemp = *enemy;
	EnemyHandle newHandle;
	
	if(curTemp->movementType == doorEnemy)
	{
		return;
	}
	
	newHandle = (EnemyHandle)mallocHandle(sizeof(Enemy));
	(**newHandle) = g_TmplEnemies[2056 - 2000];
	temp = *newHandle;
	
	temp->previousEnemy = g_FirstEnemy;
	temp->nextEnemy = (*g_FirstEnemy)->nextEnemy;
	
	if(temp->nextEnemy)
	{
		(*temp->nextEnemy)->previousEnemy = newHandle;
	}
	
	(*g_FirstEnemy)->nextEnemy = newHandle;
	
	temp->where = curTemp->where;
	temp->facing = 0;
	temp->originalPosition.h = 0;
	temp->originalPosition.v = 0;
	temp->legCounter = 0;
	temp->legState = false;
	temp->attributes &= ~originalToRoom;
	temp->attributes |= insubstantial;
	temp->movementType = dyingEnemy;
	temp->theta = 0;
	temp->disFromUnitCircle = 100;
	temp->originalNumber = 0;
	temp->angledCourse.h = 0;
	temp->angledCourse.v = 0;
	temp->deadItem = curTemp->deadItem;
	temp->stuckCounter = 0;
	
	// play sound for enemy dying...
	playSoundEffect(131);
//	PlaySndAsynchChannel( 131, kChan2, kHighPriority );	// Enemy sound, goes on chan 2
	
	g_EnemiesInRoom++;
}


void updateEnemies(void)
{
	EnemyHandle currentEnemy = (*g_FirstEnemy)->nextEnemy;
	EnemyHandle tempEnemy;
	
	// kill it if its dead
	while(currentEnemy)
	{
		//If the enemy is dead, die it.
		if((*currentEnemy)->health <= 0)
		{
			//Kill the currentEnemy
			if((*currentEnemy)->attributes & isEnemy)
			{
				g_EnemiesInRoom--;
			}
			
			if((*currentEnemy)->attributes & originalToRoom)
			{
				g_DeathRecord[g_CurrentScreen] &= ~(1 << (*currentEnemy)->originalNumber);
			}
			
			// will copy in shortly...
			// in original, this erases the enemies from the screen
			// not necessary here
			//WipeOutEnemy();
			
			if(!((*currentEnemy)->movementType == dyingEnemy) && !((*currentEnemy)->attributes & isMissile))
			{
				if(!((*currentEnemy)->attributes & canBeHeld))
				{
					// will copy in shortly...
					killCurrentEnemy(currentEnemy);
				}
			}
			
			(*(*currentEnemy)->previousEnemy)->nextEnemy = (*currentEnemy)->nextEnemy;
			if((*currentEnemy)->nextEnemy)
			{
				(*(*currentEnemy)->nextEnemy)->previousEnemy = (*currentEnemy)->previousEnemy;
			}
			
			tempEnemy = currentEnemy;
			currentEnemy = (*currentEnemy)->nextEnemy;
			freeHandle(tempEnemy);
			
			continue;
		}
		
		currentEnemy = (*currentEnemy)->nextEnemy;
	}
	
	// update it on the screen (giant switch statement)
	currentEnemy = (*g_FirstEnemy)->nextEnemy;
	
	while(currentEnemy)
	{
		// move the enemy
		switch((*currentEnemy)->movementType)
		{
			case none:
			case doorEnemy:
				standingMonster(currentEnemy);
				break;
			case randomMovement:
				randomMonster(currentEnemy);
				break;
			case homing:
				homingMonster(currentEnemy);
				break;
			case smart:
				smartMonster(currentEnemy);
				break;
			case gaurdian:
				gaurdianMonster(currentEnemy);
				break;
			case circular:
				circlingMonster(currentEnemy);
				break;
			case semibumpTurn:
			case bumpTurn:
				bumpTurnMonster(currentEnemy);
				break;
			case semihoming:
				semihomingMonster(currentEnemy);
				break;
			case linear:
				linearMonster(currentEnemy);
				break;
			case directFire:
				directFireMonster(currentEnemy);
				break;
			case dyingEnemy:
				dyingMonster(currentEnemy);
				break;
			case waitingForTime:
				//waitingForTimeMonster(currentEnemy);
				break;
			case waitingForSaric:
				waitingForSaricMonster(currentEnemy);
				break;
			/*
			case worm:
				wormMonster(currentEnemy);
				break;
			*/
			case hiveBoss:
				hiveBossMonster(currentEnemy);
				break;
			case crabBoss:
				crabBossMonster(currentEnemy);
				break;
			/*
			case lizardBoss:
				lizardBossMonster(currentEnemy);
				break;
			*/
			case blobBoss:
				blobBossMonster(currentEnemy);
				break;
			case sentryBoss:
				sentryBossMonster(currentEnemy);
				break;
			case linearBoss:
				linearBossMonster(currentEnemy);
				break;
			case rhinoBoss:
				rhinoBossMonster(currentEnemy);
				break;
			case elementalBoss:
				elementalBossMonster(currentEnemy);
				break;
			case finalBoss:
				finalBossMonster(currentEnemy);
				break;
		}
		
		if (!g_GameInProgress)
		{
			return;
		}
		
		currentEnemy = (*currentEnemy)->nextEnemy;
	}
}

void fireEnemy(EnemyHandle currentEnemy)
{
//	Handle	resH;
	EnemyHandle	new;
	EnemyPtr	temp,curTemp;
	long	currentEnemyStorage;
	short	whichOutlet;
	
	// don't add it if we have too many enemies
	if(g_EnemiesInRoom > MAX_ENEMIES_ON_SCREEN)
	{
		return;
	}
	
	curTemp = *currentEnemy;
	
	// allocate the spot for the new enemy
//	resH = GetResource('tmpl',(**currentEnemy).firedEnemy);
	new = (EnemyHandle)mallocHandle(sizeof(Enemy));
	**new = g_TmplEnemies[(*currentEnemy)->firedEnemy - 2000];
	
	/*
	if(resH == nil)
	{
		DoErrorMes( 12, nil, true );
		return;
	}
	else
	{
		new = (EnemyHandle)resH;
		HandToHand((Handle *)&new);
		if (!new)
		{
			DoErrorMes( 13, nil, true );
			return;
		}
		HLock((Handle)new);
		temp = *new;
	}
	*/
	
	temp = *new;
	
	// set up the new enemy
	temp->where = curTemp->where;
	temp->facing = curTemp->facing;
	temp->originalPosition.h = 0;
	temp->originalPosition.v = 0;
	temp->legCounter = 0;
	temp->legState = false;
	temp->attributes &= ~originalToRoom;
	temp->theta = 0;
	temp->disFromUnitCircle = 100;
	temp->originalNumber = 0;
	temp->angledCourse.h = 0;
	temp->angledCourse.v = 0;
	temp->stuckCounter = 0;
	
	whichOutlet = abs(shortRand()%2);
	
	// start enemy in the right place
	switch(curTemp->facing)
	{
		case 0:
		case 1:
			if(curTemp->attributes & isBossEnemy)
			{
				temp->where.left += 64;
				temp->where.right += 64;
				temp->where.top += 32 * whichOutlet;
				temp->where.bottom += 32 * whichOutlet;
			}
			else
			{
				temp->where.left += 32;
				temp->where.right += 32;
			}
			
			break;
		case 2:
			if(curTemp->attributes & isBossEnemy)
			{
				temp->where.top += 64;
				temp->where.bottom += 64;
				temp->where.left += 32 * whichOutlet;
				temp->where.right += 32 * whichOutlet;
			}
			else
			{
				temp->where.top += 32;
				temp->where.bottom += 32;
			}
			
			break;
		case 3:
			if(curTemp->attributes & isBossEnemy)
			{
				temp->where.top += 32 * whichOutlet;
				temp->where.bottom += 32 * whichOutlet;
			}
			
			temp->where.left -= 32;
			temp->where.right -= 32;
			break;
		case 4:
			if(curTemp->attributes & isBossEnemy)
			{
				temp->where.left += 32 * whichOutlet;
				temp->where.right += 32 * whichOutlet;
			}
			
			temp->where.top -= 32;
			temp->where.bottom -= 32;
			break;
	}
	
	currentEnemyStorage = (long)currentEnemy;
	currentEnemy = new;
	
	//if our poor widdle enemy will get stuck
	if(!enemyStandableRect(currentEnemy))
	{
		freeHandle(new);
		
		return;
	}
	
	currentEnemy = (EnemyHandle)currentEnemyStorage;
	
	// add it to the chain of enemies
	temp->previousEnemy = currentEnemy;
	temp->nextEnemy = curTemp->nextEnemy;
	
	if(temp->nextEnemy)
	{
		(*temp->nextEnemy)->previousEnemy = new;
	}
	
	curTemp->nextEnemy = new;
	
	// more enemies!!!
	g_EnemiesInRoom++;
	
//	HUnlock((Handle)new);
}

// appears to be a duplicate of the previous function with more options
Boolean bossFireEnemy(EnemyHandle currentEnemy, Point *whereIndex, short firedNum, Boolean testStuck, Boolean createdIsBoss)
{
//	Handle	resH;
	EnemyHandle	new;
	EnemyPtr	temp,curTemp;
	long	currentEnemyStorage;
	
	if(g_EnemiesInRoom > MAX_ENEMIES_ON_SCREEN)
	{
		return false;
	}
	
	curTemp = *currentEnemy;
	
	// allocate a spot for the new enemy
//	resH = GetResource('tmpl',firedNum);
	new = (EnemyHandle)mallocHandle(sizeof(Enemy));
	**new = g_TmplEnemies[firedNum - 2000];
	/*
	if(resH == nil)
	{
		DoErrorMes( 12, nil, true );
		return(false);
	}
	else
	{
		new = (EnemyHandle)resH;
		HandToHand((Handle *)&new);
		if (!new)
		{
			DoErrorMes( 13, nil, true );
			return(false);
		}
		HLock((Handle)new);
		temp = *new;
	}
	*/
	temp = *new;
	
	// initialize the new enemy
	temp->where.top = curTemp->where.top + (32 * whereIndex->v);
	temp->where.bottom = temp->where.top + 32;
	temp->where.left = curTemp->where.left + (32 * whereIndex->h);
	temp->where.right = temp->where.left + 32;
	
	temp->facing = curTemp->facing;
	temp->originalPosition.h = 0;
	temp->originalPosition.v = 0;
	temp->legCounter = 0;
	temp->legState = false;
	temp->attributes &= ~originalToRoom;
	temp->attributes |= isBossEnemy * createdIsBoss;
	temp->theta = 0;
	temp->disFromUnitCircle = 100;
	temp->originalNumber = 0;
	temp->angledCourse.h = 0;
	temp->angledCourse.v = 0;
	temp->stuckCounter = 0;
	
	currentEnemyStorage = (long)currentEnemy;
	currentEnemy = new;
	
	// only test if we are told to
	if(testStuck)
	{
		// test to see if the new enemy can stand
		if(!enemyStandableRect(currentEnemy))
		{
			//if our poor widdle enemy will get stuck
			currentEnemy = (EnemyHandle)currentEnemyStorage;
			freeHandle(new);
			return false;
		}
	}
	
	currentEnemy = (EnemyHandle)currentEnemyStorage;
	
	// add him to the chain of enemies
	temp->previousEnemy = currentEnemy;
	temp->nextEnemy = curTemp->nextEnemy;
	
	if(temp->nextEnemy)
	{
		(*temp->nextEnemy)->previousEnemy = new;
	}
	
	curTemp->nextEnemy = new;
	
	// more enemies!!!
	g_EnemiesInRoom++;
	
	return true;
}

void saricFireEnemy(short id)
{
//	Handle		resH;
	EnemyHandle	currentEnemy;
	EnemyHandle	new;
	EnemyPtr	temp, curTemp;
	
	// allocate a spot for the new enemy
//	resH = GetResource( 'tmpl', id );
	new = (EnemyHandle)mallocHandle(sizeof(Enemy));
	**new = g_TmplEnemies[id - 2000];
	/*
	if(resH == nil)
	{	DoErrorMes( 12, nil, false );
		return;
	}
	else
	{
		new = (EnemyHandle)resH;
		HandToHand((Handle *)&new);
		if( !new )
		{	DoErrorMes( 13, nil, true );
			return;
		}
		HLock((Handle)new);
		temp = *new;
	}
	*/
	temp = *new;
	
	// loop through the current enemies
	// if one is dying, replace that one
	// otherwise stick it at the end
	currentEnemy = g_FirstEnemy;
	while((*currentEnemy)->nextEnemy)
	{
		currentEnemy = (*currentEnemy)->nextEnemy;
		if(!((*currentEnemy)->movementType == dyingEnemy))
		{
			currentEnemy = (*currentEnemy)->previousEnemy;
			break;
		}
	}
	
//	HLock((Handle)currentEnemy);
	curTemp = *currentEnemy;
	
	// add it to the chain
	temp->previousEnemy = currentEnemy;
	temp->nextEnemy = curTemp->nextEnemy;
	
	if(temp->nextEnemy)
	{
		(*temp->nextEnemy)->previousEnemy = new;
	}
	
	curTemp->nextEnemy = new;
	
	// intialize the new enemy
	temp->where = g_Saric.where;
	temp->originalPosition.h = 0;
	temp->originalPosition.v = 0;
	temp->legCounter = 0;
	temp->legState = false;
	temp->attributes &= ~originalToRoom;
	temp->attributes &= ~isEnemy;
	temp->theta = 0;
	temp->disFromUnitCircle = 100;
	temp->originalNumber = 0;
	temp->angledCourse.h = 0;
	temp->angledCourse.v = 0;
	temp->stuckCounter = 0;
	
	// make sure it starts in the right place
	// and make sure it is facing the right way
	switch(g_Saric.facing)
	{
		case 0:
			temp->where.left -= 32;
			temp->where.right -= 32;
			temp->facing = 3;
			break;
		case 1:
			temp->where.left += 32;
			temp->where.right += 32;
			temp->facing = 1;
			break;
		case 2:
			temp->where.top += 32;
			temp->where.bottom += 32;
			temp->facing = 2;
			break;
		case 3:
			temp->where.top -= 32;
			temp->where.bottom -= 32;
			temp->facing = 4;
			break;
	}
	
//	HUnlock((Handle)currentEnemy);
//	HUnlock((Handle)new);
}

