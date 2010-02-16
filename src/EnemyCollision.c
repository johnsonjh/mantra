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

#include "GameTypes.h"
#include "GameConstants.h"
#include "GameDefines.h"

#include "Utils.h"
#include "Sound.h"
#include "Dialogs.h"

#include "EnemyCollision.h"


// translated from assembly I don't even understand... ick...
Boolean interceptAsm(Ptr mask1, Ptr mask2, long nextrow1, long nextrow2, short rows, short columns)
{
	int i, j;
	int rowsize1 = nextrow1 + columns;
	int rowsize2 = nextrow2 + columns;
	
	// loop through every pixel
	for(i = 0;i < rows;i++)
	{
		for(j = 0;j < columns;j++)
		{
			// return true if they have one in common
			if (mask1[(i * rowsize1) + j] && mask2[(i * rowsize2) + j])
			{
				return true;
			}
		}
	}
	
	return false;
}

// generic function for testing an intersection
// "preps" for interceptAsm
Boolean testIntercept(short sprite1, short sprite2, Rect *r1, Rect *r2, int type1, int type2, long isBoss1, long isBoss2)
{
	Rect  rect1 = *r1;
	Rect  rect2 = *r2;
	short r1startrow, r2startrow;
	short rows;
	short r1startcolumn, r2startcolumn;
	short columns;
	long  nextrow1, nextrow2;
	Ptr   mask1 = 0, mask2 = 0;
	
	rect1.right = rect1.left + 32;
	rect2.right = rect2.left + 32;
	rect1.bottom = rect1.top + 32;
	rect2.bottom = rect2.top + 32;
	
	if(isBoss1)
	{
		rect1.right += 32;
		rect1.bottom += 32;
	}
	
	if(isBoss2)
	{
		rect2.right += 32;
		rect2.bottom += 32;
	}
	
	if (rect1.right <= rect2.left || rect2.right <= rect1.left)
	{
		return false;			// no - rects don't intercept at all.
	}
	
	if (rect1.left < rect2.left)	// rect1 on left
	{
		r1startcolumn = rect2.left - rect1.left;
		r2startcolumn = 0;				// start on left of rect2
		columns = rect1.right - rect2.left;
	}
	else							// rect2 on left
	{
		r1startcolumn = 0;
		r2startcolumn = rect1.left - rect2.left;
		columns = rect2.right - rect1.left;
	}
	
	if (rect1.bottom <= rect2.top || rect2.bottom <= rect1.top)
	{
		return false;			// no - rects don't intercept at all.
	}
	
	if (rect1.top < rect2.top)	// rect1 on top
	{
		r1startrow = rect2.top - rect1.top;
		r2startrow = 0;				// start on top of rect2
		rows = rect1.bottom - rect2.top;
	}
	else							// rect2 on top
	{
		r1startrow = 0;
		r2startrow = rect1.top - rect2.top;
		rows = rect2.bottom - rect1.top;
	}
	
	if (rows <= 0 || columns <= 0)
	{
		return false;
	}
	
	if(isBoss1)
	{
		nextrow1 = 64 - columns;
	}
	else
	{
		nextrow1 = 32 - columns;
	}
	
	if(isBoss2)
	{
		nextrow2 = 64 - columns;
	}
	else
	{
		nextrow2 = 32 - columns;
	}
	
	switch (type1)
	{
	case testSaricIntercept:
		mask1 = (Ptr)g_SaricMask;
		break;
		
	case testEnemyIntercept:
		if(isBoss1)
		{
			mask1 = (Ptr)(&g_BossMasks[4096*sprite1]);
		}
		else
		{
			mask1 = (Ptr)(&g_EnemyMasks[1024*sprite1]);
		}
		
		break;
		
	case testSwordIntercept:
		mask1 = (Ptr)(&g_SwordMasks[1024*sprite1]);
		break;
		
	case testMapIntercept:
		mask1 = (Ptr)(&g_TileMasks[1024*sprite1]);
		break;
	}
	
	switch (type2)
	{
	case testSaricIntercept:
		mask2 = (Ptr)g_SaricMask;
		break;
		
	case testEnemyIntercept:
		if(isBoss2)
		{
			mask2 = (Ptr)(&g_BossMasks[4096*sprite2]);
		}
		else
		{
			mask2 = (Ptr)(&g_EnemyMasks[1024*sprite2]);
		}
		
		break;
		
	case testSwordIntercept:
		mask2 = (Ptr)(&g_SwordMasks[1024*sprite2]);
		break;
		
	case testMapIntercept:
		mask2 = (Ptr)(&g_TileMasks[1024*sprite2]);
		break;
	}
	
	if(isBoss1)
	{
		mask1 += r1startrow * 64 + r1startcolumn;
	}
	else
	{
		mask1 += r1startrow * 32 + r1startcolumn;
	}
	
	if(isBoss2)
	{
		mask2 += r2startrow * 64 + r2startcolumn;
	}
	else
	{
		mask2 += r2startrow * 32 + r2startcolumn;
	}
	
	return interceptAsm(mask1, mask2, nextrow1, nextrow2, rows, columns);
}

// this is terrible...
// need to find a better way to do it
Boolean g_SaricStandableRectHasIterated = false;

// note: this function does a hell of a 
// lot more than just testing the rect...
// it damages saric again (and maybe even again)
// if he hits another enemy

Boolean	standableRect(Rect rect)
{
	short		i, j, enemyModNum;
	Rect		mapRect, oldRect;
	EnemyPtr	next;
	EnemyHandle	nextHandle;
	
	// find out which tiles we are on
	rect.left	= (rect.left / 32);
	rect.top	= (rect.top / 32);
	rect.right	= (rect.right - 1) / 32;
	rect.bottom	= (rect.bottom - 1) / 32;
	
	// correct any mistakes
	if( rect.bottom > 9 )
	{
		rect.bottom = 9;
	}
	
	if( rect.top < 0 )
	{
		rect.top = 0;
	}
	
	if( rect.left < 0 )
	{
		rect.left = 0;
	}
	
	if( rect.right > 15 )
	{
		rect.right = 15;
	}
	
	// check the enemies to see if there is one there
	nextHandle = g_FirstEnemy;
	
	while((*nextHandle)->nextEnemy)
	{
		nextHandle = (*nextHandle)->nextEnemy;
		next = *nextHandle;
		
		// this one doesn't matter
		if(next->attributes & insubstantial)
		{
			continue;
		}
		
		// figure out which sprite index to use
		enemyModNum = next->legState;
		
		if(next->attributes & isMultiFacing)
		{
			enemyModNum += (next->facing - 1) * 2;
			
			if(next->facing == 0)
			{
				enemyModNum += 2;
			}
		}
		
		// see if saric intercepts the enemy
		if(testIntercept((short)(next->spriteRef - 2000 + enemyModNum), 0, &next->where, &g_Saric.where, testEnemyIntercept, testSaricIntercept, next->attributes & isBossEnemy, 0))
		{
			// see if saric should be damaged
			if(g_Saric.woundCounter == 0 && (next->attributes & isEnemy) == isEnemy)
			{
				// calculate the exact amount of damage
				i = next->damage - (g_Saric.armorValue + g_Saric.itemEffects[0].armor + g_Saric.itemEffects[1].armor + g_Saric.itemEffects[2].armor);
				
				// no damage should be done
				if(i <= 0 && next->movementType != dyingEnemy)
				{
					g_Saric.incrementalDamageCounter++;
					g_Saric.woundCounter = 1;
					
					// do a tiny bit (it all adds up)
					if(g_Saric.incrementalDamageCounter >= 5)
					{
						i = 1;
						g_Saric.incrementalDamageCounter = 0;
					}
				}
				
			//	printf("About to do damage!\n");
				
				// damage should actually be done
				if(i > 0 && CHECK_IMMUNITIES(g_Saric.immunities, next->damageType))
				{
					// damage saric
					g_Saric.health -= i;
					
					printf("Doing damage!\n");
					
					// play sound
					playSoundEffect(129);
//					PlaySndAsynchChannel( 129, kChan1, kHighPriority );
					
					g_Saric.woundCounter = 1;
					oldRect = g_Saric.where;
					
					// bump saric in the right direction
					switch(next->facing)
					{
						case 1:
							g_Saric.where.left += 8;
							g_Saric.where.right += 8;
							break;
						case 2:
							g_Saric.where.top += 8;
							g_Saric.where.bottom += 8;
							break;
						case 3:
							g_Saric.where.left -= 8;
							g_Saric.where.right -= 8;
							break;
						case 4:
							g_Saric.where.top -= 8;
							g_Saric.where.bottom -= 8;
							break;
					}
					
					if(g_SaricStandableRectHasIterated)
					{
						return false;
					}
					else
					{
						printf("Recursing!\n");
						
						g_SaricStandableRectHasIterated = true;
						
						// RECURSIVE??!?!?!?!?!?
						if(!standableRect(g_Saric.where))
						{
							printf("We failed!\n");
							g_Saric.where = oldRect;
						}
						
						g_SaricStandableRectHasIterated = false;
					}
					
					drawStats();
				}
				
			}
			
			// if the enemy has a message, display it
			if(next->messageID && !g_Saric.messageCounter)
			{
				g_Saric.messageCounter = 1;
				
				if(next->messageID > 0)
				{
					displayMessage(8000, next->messageID, next->deadItem);
				}
				else
				{
					gameDialog(dialogStore, next->messageID);
				}
			}
			
			// and now, the point of this function:
			// this rect is NOT standable
			return false;
		}
	}
	
	// no enemies intersected, now check the map
	for(i = rect.top;i <= rect.bottom;i++)
	{
		for(j = rect.left;j <= rect.right;j++)
		{
			// check if the tile is standable
			if(!(g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].modifiers & standable))
			{
				// figure out the rect for this tile
				mapRect.left = j * 32;
				mapRect.right = mapRect.left + 32;
				mapRect.top = i * 32;
				mapRect.bottom = mapRect.top + 32;
				
				// test if we intercept something on the map
				if(testIntercept((short)(g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].spriteRef - 1000), 0, &mapRect, &g_Saric.where, testMapIntercept, testSaricIntercept, 0, 0))
				{
					// this rect is NOT standable
					return false;
				}
			}
		}
	}
	
	// finally, after all that,
	// saric is allowed to stand here
	return true;
}

// checks to see if an enemy intersects with Saric (duh)
Boolean checkEnemyInterceptWithSaric(EnemyHandle currentEnemy)
{
	short           i, enemyModNum;
	EnemyPtr        temp;
	Rect            oldRect, rect;
	
	temp = *currentEnemy;
	rect = temp->where;
	
	// calculate which mask to use
	enemyModNum = temp->legState;
	
	if(temp->attributes & isMultiFacing)
	{	
		enemyModNum += (temp->facing - 1) * 2;
		
		if(temp->facing == 0)
		{
			enemyModNum += 2;
		}
	}
	
	// actually test for an intersection
	if(testIntercept((short)(temp->spriteRef - 2000 + enemyModNum), 0, &temp->where, &g_Saric.where, testEnemyIntercept, testSaricIntercept, temp->attributes & isBossEnemy, 0))
	{
		// do stuff if it was detected
		
		// a message should be displayed
		if(temp->messageID && !g_Saric.messageCounter)
		{
			g_Saric.messageCounter = 1;
			
			if(temp->messageID > 0)
			{
				displayMessage(8000, temp->messageID, temp->deadItem);
			}
			else
			{
				gameDialog(dialogStore, temp->messageID);
			}
		}
		
		// saric is picking up an item
		if(temp->deadItem && (temp->attributes & canBeHeld))
		{
			// play sound
			playSoundEffect(133);
//			PlaySndAsynchChannel( 133, kChan3, kHighPriority );
			
			// give saric the item
			g_Saric.itemQuantities[temp->deadItem] += g_ItemTemplates[temp->deadItem].quantity;
			
			// item contains money
			if(g_ItemTemplates[temp->deadItem].attributes & isMoney)
			{
				g_Saric.money += g_ItemTemplates[temp->deadItem].quantity;
				// play sound
				playSoundEffect(137);
			//	PlaySndAsynchChannel( 137, kChan4, kHighPriority );
			}
			
			// item is a message
			if(g_ItemTemplates[temp->deadItem].attributes & isMessage)
			{
				displayItemMessage(temp->deadItem);
			}
			
			// we are done with the item
			temp->deadItem = 0;
			
			// kill the enemy if it is not already dead
			if(temp->movementType != dyingEnemy)
			{
				temp->health = 0;
			}
		}
		
		// hurt saric if he hit an enemy
		if(g_Saric.woundCounter == 0 && (temp->attributes & isEnemy) == isEnemy)
		{
			// calculate the damage done
			i = temp->damage - (g_Saric.armorValue + g_Saric.itemEffects[0].armor + g_Saric.itemEffects[1].armor + g_Saric.itemEffects[2].armor);
			
			// if no damage, we only hurt him a little bit
			if(i <= 0 && temp->movementType != dyingEnemy )
			{
				g_Saric.incrementalDamageCounter++;
				g_Saric.woundCounter = 10;
				
				if(g_Saric.incrementalDamageCounter >= 5)
				{
					i = 1;
					g_Saric.incrementalDamageCounter = 0;
				}
			}
			
		//	printf("About to do damage!\n");
			
			// damage will actually be done
			if(i > 0 && CHECK_IMMUNITIES(g_Saric.immunities, temp->damageType))
			{
				g_Saric.health -= i;
				
				printf("Doing damage!\n");
				
				// play sound
				playSoundEffect(129);
//				PlaySndAsynchChannel( 129, kChan1, kHighPriority );
				
				g_Saric.woundCounter = 1;
				oldRect = g_Saric.where;
				
				// bump saric in the correct direction
				switch(temp->facing)
				{
					case 1:
						g_Saric.where.left += 8;
						g_Saric.where.right += 8;
						break;
					case 2:
						g_Saric.where.top += 8;
						g_Saric.where.bottom += 8;
						break;
					case 3:
						g_Saric.where.left -= 8;
						g_Saric.where.right -= 8;
						break;
					case 4:
						g_Saric.where.top -= 8;
						g_Saric.where.bottom -= 8;
						break;
				}
				
				// if this new location isn't ok, revert back
				if(!standableRect(g_Saric.where))
				{
					g_Saric.where = oldRect;
				}
			}
			
			drawStats();
		}
		
		return true;
	}
	
	return false;
}

// checks to see if an enemy intersects
// with a rock or tree or whatever...
Boolean checkEnemyInterceptWithMap(EnemyHandle currentEnemy)
{
	short           i, j, enemyModNum;
	EnemyPtr        temp;
	Rect            mapRect, rect;
	
	temp = *currentEnemy;
	rect = temp->where;
	
	// it is off the screen, that is a problem
	if(rect.top < 0 || rect.left < 0)
	{
		return true;
	}
	
	// figure out which tiles are covered
	rect.left	= (rect.left / 32);
	rect.top	= (rect.top / 32);
	rect.right	= (rect.right - 1) / 32;
	rect.bottom	= (rect.bottom - 1) / 32;
	
	// off the screen, again this is a problem
	if(rect.bottom > 9 || rect.right > 15)
	{
		return true;
	}
	
	// a boss enemy takes up more room
	if(temp->attributes & isBossEnemy)
	{
		rect.right++;
		rect.bottom++;
	}
	
	// this enemy doesn't matter (how sad)
	if(temp->attributes & insubstantial)
	{
		return false;
	}
	
	// calculate which sprite index to use
	enemyModNum = temp->legState;
	
	if(temp->attributes & isMultiFacing)
	{
		enemyModNum += (temp->facing - 1) * 2;
		
		if(temp->facing == 0)
		{
			enemyModNum += 2;
		}
	}
	
	// check all the covered map tiles
	for(i = rect.top;i <= rect.bottom;i++)
	{
		for(j = rect.left;j <= rect.right;j++)
		{
			// check if the rect is standable
			if( (g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].modifiers & standable) == 0 )
			{
				// these should do the same thing, but
				// this order always confuses me...
//				SetRect(&mapRect, j*32, i*32, j*32 + 32, i*32 + 32);
				
				// figure out the rect for this tile
				mapRect.left = j * 32;
				mapRect.right = mapRect.left + 32;
				mapRect.top = i * 32;
				mapRect.bottom = mapRect.top + 32;
				
				// invalid ref... not sure what that means
				if(g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].spriteRef < 1000 || g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].spriteRef > 1100)
				{
					continue;
				}
				
				// test if it intersects
				if(testIntercept((short)(g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].spriteRef - 1000), (short)(temp->spriteRef - 2000 + enemyModNum), &mapRect, &temp->where, testMapIntercept, testEnemyIntercept, 0, temp->attributes & isBossEnemy))
				{
					return true;
				}
			}
		}
	}
	
	return false;
}

Boolean g_EnemyStandableRectHasIterated = false;

// enemies everywhere!  better make
// sure they don't hit each other...
Boolean checkEnemyInterceptWithEnemies(EnemyHandle currentEnemy)
{
	short           i, enemyModNum, enemyModNum2;
	EnemyPtr        temp, next;
	EnemyHandle     scratchEnemy;
	Rect            oldEnemyRect;
	
	temp = *currentEnemy;
	next = *g_FirstEnemy;

	next = *next->nextEnemy;
	
	while(next)
	{
		// determine current enemy sprite ref
		enemyModNum = temp->legState;
		if(temp->attributes & isMultiFacing)
		{	
			enemyModNum += (temp->facing - 1) * 2;
			if(temp->facing == 0)
			{
				enemyModNum += 2;
			}
		}
		
		// determine next enemy sprite ref
		enemyModNum2 = next->legState;
		if(next->attributes & isMultiFacing)
		{	
			enemyModNum2 += (next->facing - 1) * 2;
			if(next->facing == 0)
			{
				enemyModNum2 += 2;
			}
		}
		
		// see if this enemy intercepts the current enemy
		if(testIntercept((short)(next->spriteRef - 2000 + enemyModNum2), (short)(temp->spriteRef - 2000 + enemyModNum), &next->where, &temp->where, testEnemyIntercept, testEnemyIntercept, next->attributes & isBossEnemy, temp->attributes & isBossEnemy))
		{
			// see if one is an enemy and the other isn't
			// for some reason, we don't worry about anything else
			if(((temp->attributes & isEnemy) == 0 && (next->attributes & isEnemy)) ||
				((temp->attributes & isEnemy) && (next->attributes & isEnemy) == 0))
			{
				// we only do something if they matter
				if(!(temp->attributes & insubstantial && next->attributes & insubstantial))
				{
					// if its a missile, kill it (why?)
					// possibly because 
					if(temp->attributes & isMissile)
					{
						temp->health = 0;
					}
					
					oldEnemyRect = next->where;
					
					// determine damage
					i = temp->damage - next->armorValue;
					
					// see if it actually get wounded
					if(i > 0 && (next->attributes & killable) && next->legCounter > 15 && CHECK_IMMUNITIES( next->immunities, temp->damageType))
					{
						// do the damage
						next->health -= i;
						
						// if it dies, add the experience
						if(next->health <= 0 && (next->attributes & isEnemy))
						{
							g_Saric.experience += next->xp;
						}
						
						// bump it in the correct direction
						next->legCounter = 0;
						switch (temp->facing)
						{
							case 1:
								next->where.left += SWORD_OFFSET;
								next->where.right += SWORD_OFFSET;
								break;
							case 2:
								next->where.top += SWORD_OFFSET;
								next->where.bottom += SWORD_OFFSET;
								break;
							case 3:
								next->where.left -= SWORD_OFFSET;
								next->where.right -= SWORD_OFFSET;
								break;
							case 4:
								next->where.top -= SWORD_OFFSET;
								next->where.bottom -= SWORD_OFFSET;
								break;
						}
						
						// RECURSIVE?!?!??!!?!?
						if(!g_EnemyStandableRectHasIterated)
						{
							// store enemy for next call
							// probably not needed now that
							// currentEnemy is not global,
							// but I don't want to mess with 
							// the logic now (might mess it up)
							scratchEnemy = currentEnemy;
							currentEnemy = (*next->previousEnemy)->nextEnemy;
							
							g_EnemyStandableRectHasIterated = true;
							
							if(!enemyStandableRect(currentEnemy))
							{
								next->where = oldEnemyRect;
							}
							
							g_EnemyStandableRectHasIterated = false;
							
							// restore the current enemy
							currentEnemy = scratchEnemy;
						}
						else
						{
							return false;
						}
						
						// play sound
						playSoundEffect(130);
//						PlaySndAsynchChannel( 130, kChan2, kHighPriority );
					}
				}
			}
		}
		
		// break if no next enemy
		if(!next->nextEnemy)
		{
			break;
		}
		
		next = *(next->nextEnemy);
	}
	
	// exit if this enemy doesn't matter
	if(temp->attributes & insubstantial)
	{
		return false;
	}
	
	next = *(*g_FirstEnemy)->nextEnemy;
	
	while(next)
	{
		// skip this enemy if they don't matter
		if(next->attributes & insubstantial)
		{
			if(next->nextEnemy == NULL)
			{
				break;
			}
			
			next = *next->nextEnemy;
			continue;
		}
		
		// calculate mask index
		enemyModNum = temp->legState;
		
		if(temp->attributes & isMultiFacing)
		{	
			enemyModNum += (temp->facing - 1) * 2;
			
			if(temp->facing == 0)
			{
				enemyModNum += 2;
			}
		}
		
		// calculate mask index
		enemyModNum2 = next->legState;
		
		if(next->attributes & isMultiFacing)
		{	
			enemyModNum2 += (next->facing - 1) * 2;
			
			if(next->facing == 0)
			{
				enemyModNum2 += 2;
			}
		}
		
		// only test the intercept if
		// it is a different enemy
		if(temp != next)
		{
			// test for intercept
			if(testIntercept((short)(next->spriteRef - 2000 + enemyModNum2), (short)(temp->spriteRef - 2000 + enemyModNum), &next->where, &temp->where, testEnemyIntercept, testEnemyIntercept, next->attributes & isBossEnemy, temp->attributes & isBossEnemy))
			{
				return true;
			}
		}
		
		// exit if no next enemy
		// seems redundant...
		if(!next->nextEnemy)
		{
			break;
		}
		
		next = *next->nextEnemy;
	}
	
	return false;
}

// checks to see if Saric's sword will hit the enemy
Boolean checkProximityToSword(EnemyHandle currentEnemy)
{
	Rect rect,oldRect;
	EnemyPtr	temp;
	short	enemyModNum;
	
	temp = *currentEnemy;
	rect = temp->where;
	oldRect = temp->where;
	
	// calculate the rect of the sword
	switch (g_Saric.facing)
	{
		case 0:
			rect.left = g_Saric.where.left - (SWORD_OFFSET+32);
			rect.top = g_Saric.where.top;
			break;
		case 1:
			rect.left = g_Saric.where.left + (SWORD_OFFSET+32);
			rect.top = g_Saric.where.top;
			break;
		case 2:
			rect.left = g_Saric.where.left;
			rect.top = g_Saric.where.top + (SWORD_OFFSET+32);
			break;
		case 3:
			rect.left = g_Saric.where.left;
			rect.top = g_Saric.where.top - (SWORD_OFFSET+32);
			break;
	}
// •• what about the bottom and right of the rect?  they're still leftover from the enemy's rect...
// ••  which makes me wonder why rect = temp->where is up there..
	
	// calculate mask index
	enemyModNum = temp->legState;
	
	if(temp->attributes & isMultiFacing)
	{	
		enemyModNum += (temp->facing - 1) * 2;
		
		if(temp->facing == 0)
		{
			enemyModNum += 2;
		}
	}

	// actually test for intercept
// •• If the sword's not out this routine should just return false, right?
	if(temp->legCounter > 15 && g_Saric.swordOut && testIntercept((short)(temp->spriteRef - 2000 + enemyModNum), (short)g_Saric.facing, &temp->where, &rect, testEnemyIntercept, testSwordIntercept, temp->attributes & isBossEnemy, 0))
	{
		// we hit him!!!
		
		// oldRect will hold the previous coordinates
		// (as in before it last moved, i.e. backup one frame)
		oldRect = temp->where;
		switch(temp->facing)
		{
			case 1:
				offsetRect(&oldRect, -temp->speed, 0);
				break;
			case 2:
				offsetRect(&oldRect, 0, -temp->speed);
				break;
			case 3:
				offsetRect(&oldRect, temp->speed, 0);
				break;
			case 4:
				offsetRect(&oldRect, 0, temp->speed);
				break;
		}
// •• what is this?  Comments!
		
		// YAY!!  I figured it out!  here are the comments:
		
		// only hit if the taxicab distance between Saric and the old position
		// is higher than the taxicab distance between Saric and the current position
		
		// I think this is to make sure the enemy isn't currently moving away from him
		if((abs(g_Saric.where.top - oldRect.top) + abs(g_Saric.where.left - oldRect.left)) > (abs(g_Saric.where.left - temp->where.left) + abs(g_Saric.where.top - temp->where.top)))
		{
			return true;
		}
	}

//	HUnlock((Handle)currentEnemy);
	return false;
}

// check to see if an enemy is pushing Saric (poor Saric...)
// or is it checking to see if Saric is pushing an enemy?
// (probably the second one...)
Boolean checkEnemyPushing(EnemyHandle currentEnemy)
{
	short	enemyModNum;
	EnemyPtr	temp;
	Rect	oldRect,bumpRect;
	
	temp = *currentEnemy;
	
	// figure out where we are (or it is) getting bumped to
	bumpRect = temp->where;
	switch (g_Saric.facing)
	{
		case 0:
			bumpRect.left += 4;
			bumpRect.right += 4;
			break;
		case 1:
			bumpRect.left -= 4;
			bumpRect.right -= 4;
			break;
		case 2:
			bumpRect.top -= 4;
			bumpRect.bottom -= 4;
			break;
		case 3:
			bumpRect.top += 4;
			bumpRect.bottom += 4;
			break;
	}
	
	// calculate mask index
	enemyModNum = temp->legState;
	if(temp->attributes & isMultiFacing)
	{
		enemyModNum += (temp->facing - 1) * 2;
		
		if(temp->facing == 0)
		{
			enemyModNum += 2;
		}
	}

// •• If it's not pushable, why get this far?
	if(temp->attributes & pushable)
	{
// •• Funky indentation!!  Cool!
// not anymore!!!
		if(testIntercept((short)(temp->spriteRef - 2000 + enemyModNum), 0, &bumpRect, &g_Saric.where, testEnemyIntercept, testSaricIntercept, temp->attributes & isBossEnemy, 0))
		{
			// the enemy and Saric intercept
			
			// find the old rect before we (or it) got bumped
			oldRect = temp->where;
			switch (g_Saric.facing)
			{
				case 0:
					temp->where.left -= temp->pushableSpeed;
					temp->where.right -= temp->pushableSpeed;
					break;
				case 1:
					temp->where.left += temp->pushableSpeed;
					temp->where.right += temp->pushableSpeed;
					break;
				case 2:
					temp->where.top += temp->pushableSpeed;
					temp->where.bottom += temp->pushableSpeed;
					break;
				case 3:
					temp->where.top -= temp->pushableSpeed;
					temp->where.bottom -= temp->pushableSpeed;
					break;
			}
			
			// RECURSIVE?!?!?!?!?!?!?
			// no, not this time (thank goodness...)
			if(!g_EnemyStandableRectHasIterated)
			{
				g_EnemyStandableRectHasIterated = true;
				
				// check to see if the enemy can stand here
				if(!enemyStandableRect(currentEnemy))
				{
					temp->where = oldRect;
				}
				
				g_EnemyStandableRectHasIterated = false;
			}
			else
			{
	//			HUnlock((Handle)currentEnemy);
				return true;
			}
		}
	}
	
	return false;
}

//checks if spot is acceptable
// basically runs all of the above
// in a very special way (which of
// course I completely understand...)
Boolean	enemyStandableRect(EnemyHandle currentEnemy)
{
	EnemyPtr	temp;
	
	temp = *currentEnemy;

// •• All right, that was just a little redundancy above.  No harm.
	if(temp->attributes & pushable)
	{
		checkEnemyPushing(currentEnemy);
	}
	
	if(checkEnemyInterceptWithSaric(currentEnemy))
	{
//		HUnlock((Handle)currentEnemy);
		return false;
	}
	
	if(checkEnemyInterceptWithMap(currentEnemy))
	{
//		HUnlock((Handle)currentEnemy);
		return false;
	}

	if(checkEnemyInterceptWithEnemies(currentEnemy))
	{
//		HUnlock((Handle)currentEnemy);
		return false;
	}

// •• This routine doesn't work? 
/*Dear Dustin: It damn well does, 
	but we don't want to check proximity on knockback*/
//	CheckProximityToSword();
// •• white space: <g>

	
	
//	HUnlock((Handle)currentEnemy);
	return true;
}
