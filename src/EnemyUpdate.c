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

#include "Enemies.h"
#include "EnemyCollision.h"
#include "EnemyUpdate.h"


/////////////////////////////////
// Fixed routines
/////////////////////////////////

// insert routines from new files
// should all be in here now...

void standingMonster(EnemyHandle currentEnemy)
{
	EnemyPtr	temp;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	
// •• Problem is, do these routines just check, or do they actually do damage, etc..
// •• It's unclear by name...  Keep this sort of thing in mind in the future.
	checkEnemyInterceptWithSaric(currentEnemy);
	checkEnemyPushing(currentEnemy);
	
	// if the leg counter is right...
	temp->legCounter++;
	if(temp->legCounter >= 32)
	{
		// if we are allowed to fire...
		if((temp->attributes & canFire) == canFire)
		{
			// if our luck is good enough...
			if((shortRand() % (17 - temp->rateOfFire)) == 0)
			{
				// FIRE!!!!!!
				fireEnemy(currentEnemy);
			}
		}
		
		// reset the leg count
		temp->legCounter=16;
	}
		
//	HUnlock((Handle)currentEnemy);
}

void waitingForSaricMonster(EnemyHandle currentEnemy)
{
	Rect	oldRect;
	EnemyPtr	temp;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
	checkEnemyInterceptWithSaric(currentEnemy);
	checkEnemyPushing(currentEnemy);
	
	// it appears like this enemy will wait until Saric is close enough, 
	// then change its movement type to something else...
	if(abs(g_Saric.where.top - temp->where.top) + abs(g_Saric.where.left - temp->where.left) <= temp->target)
	{
		temp->movementType = temp->movePhase;
	}
		
//	HUnlock((Handle)currentEnemy);
}

void dyingMonster(EnemyHandle currentEnemy)
{
	Rect	oldRect; //, rect = g_Saric.where;
	EnemyPtr	temp;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
	temp->legCounter++;

// We go through the death scene
	if(temp->legCounter <= 12)
	{
		temp->facing = 1 + temp->legCounter / 4;
		temp->legState = (temp->legCounter & 2);
	}
	
//and maintain until it goes away
	if(temp->legCounter == 14 && temp->deadItem)
	{
	//this section tests whether the enemy intercepts with the map at all
		
		// this looks really familiar...
		short	i, j, enemyModNum;
		Rect	nrect;
		short	numIntercepts = 0, numTests = 0;
		
		nrect = temp->where;
		
		nrect.left	= (nrect.left / 32);
		nrect.top	= (nrect.top / 32);
		nrect.right	= (nrect.right - 1) / 32;
		nrect.bottom	= (nrect.bottom - 1) / 32;
	
		if(nrect.top < 0)
		{
			nrect.top = 0;
		}
		
		if(nrect.left < 0)
		{
			nrect.left = 0;
		}
		
		if(nrect.bottom > 9)
		{
			nrect.bottom = 9;
		}
		
		if(nrect.right > 15)
		{
			nrect.right = 15;
		}
		
		if(temp->attributes & isBossEnemy)
		{
			nrect.right++;
			nrect.bottom++;
		}
		
		enemyModNum = temp->legState;
		
		if(temp->attributes & isMultiFacing)
		{	
			enemyModNum += (temp->facing - 1) * 2;
			
			if(temp->facing == 0)
			{
				enemyModNum += 2;
			}
		}
		
		// however, this is new...
		// checking how many regions that we are on 
		// are standable for some reason
		for(i = nrect.top;i <= nrect.bottom;i++)
		{
			for(j = nrect.left;j <= nrect.right;j++)
			{
				if( (g_MapScreens[g_CurrentScreen].tiles[(j * 10) + i].modifiers & standable) == 0 )
				{
					numIntercepts++;
				}
				
				numTests++;
			}
		}
		
		if(numIntercepts == numTests && numIntercepts > 0)
		{
			// play sound
			playSoundEffect(133);
		//	PlaySndAsynchChannel( 133, kChan3, kMedPriority );	// Item sound, chan3
			
			g_Saric.itemQuantities[temp->deadItem] += g_ItemTemplates[temp->deadItem].quantity;
			
			if(g_ItemTemplates[temp->deadItem].attributes & isMoney)
			{
				g_Saric.money += g_ItemTemplates[temp->deadItem].quantity;
				
				// play sound
				playSoundEffect(137);
			//	PlaySndAsynchChannel( 137, kChan4, kHighPriority );	// mo’ money
						// special sound, chan 4
			}
			
			temp->deadItem = 0;
		}
	}
	
	if(temp->legCounter >= 12 && temp->deadItem)
	{
		temp->legState = false;
	}
	else
	{
		temp->legState = true;
	}
	
	if(temp->legCounter >= 250)
	{
		if(!temp->deadItem)
		{
			temp->health = 0;
		}
		else
		{
			temp->legCounter = 16;
		}
	}
	
	checkEnemyInterceptWithSaric(currentEnemy);
	
//	HUnlock((Handle)currentEnemy);
}

// this is for enemies that have been fired and
// are on their way towards Saric as we speak...
void directFireMonster(EnemyHandle currentEnemy)
{
	Rect	oldRect; //, rect = g_Saric.where;
	EnemyPtr	temp;
	short	x, y;
	Boolean	stopped;
	
	temp = *currentEnemy;
	oldRect = temp->where;
	
	// calculate what angle the enemy should be fired at
	// only called if we have not already calculated it
	// basically we are aiming it towards Saric's current position
	// (where the hell else would we want to aim it?)
	if(temp->angledCourse.h == 0 && temp->angledCourse.v == 0)
	{
		x = g_Saric.where.left - temp->where.left;
		y = g_Saric.where.top - temp->where.top;
// •• you call abs twice here..  Oh well, this isn't too heavily looped.
// not a problem anymore!  thank you modern day processors! :)
		temp->angledCourse.h = abs((temp->speed * x) / (abs(x) + abs(y)));
		temp->angledCourse.v = abs((temp->speed * y) / (abs(x) + abs(y)));
		
		if(x < 0)
		{
			temp->angledCourse.h *= -1;
		}
		
		if(y < 0)
		{
			temp->angledCourse.v *= -1;
		}
		
		// make sur the enemy is facing the right way
		if(abs(temp->angledCourse.h) - abs(temp->angledCourse.v) > 0)
		{
			if(temp->angledCourse.h > 0)
			{
				temp->facing = 1;
			}
			else
			{
				temp->facing = 3;
			}
		}
		else
		{
			if(temp->angledCourse.v > 0)
			{
				temp->facing = 2;
			}
			else
			{
				temp->facing = 4;
			}
		}
	}
	
// •• This is wierd.  You base a lot off of legCounter, don't you.
	
	// increase leg counter
	temp->legCounter++;
	
	// if the leg counter is right...
	if(temp->legCounter >= 32)
	{
		// if we are allowed to fire...
		if((temp->attributes & canFire) == canFire)
		{
			// if we are lucky enough...
			if((shortRand() % (17 - temp->rateOfFire)) == 0)
			{
				// FIRE!!!!!
				fireEnemy(currentEnemy);
			}
		}
		
		temp->legCounter = 16;
	}
	
	// if temp->legCounter % 4 == 0 (why didn't you just do this?)
	// we change the leg state (whatever that does...)
	if((temp->legCounter / 4) * 4 == temp->legCounter)
	{
		temp->legState = 1 - temp->legState;
	}
	
	// move the enemy in its current direction
	offsetRect(&temp->where,temp->angledCourse.h,temp->angledCourse.v);
	
	// check for any intersections...
	stopped = checkEnemyInterceptWithSaric(currentEnemy);
	stopped += checkEnemyInterceptWithMap(currentEnemy);
	stopped += checkEnemyInterceptWithEnemies(currentEnemy);
	stopped += checkEnemyPushing(currentEnemy);
	
	// if a problem was detected...
	if(stopped)
	{
		// if its a missile, it dies...
		if((temp->attributes & isMissile) == isMissile)
		{
			temp->health = 0;
		}
		
		// reset vars
		temp->where = oldRect;
		temp->angledCourse.h = 0;
		temp->angledCourse.v = 0;
	}
	
// •• so every enemy has fields like angledCourse and movePhase that's only used in a few?
// why is this comment located here?
	
//	HUnlock((Handle)currentEnemy);
}

// ooooooh, this sounds like fun,
// but how truly random is it?
void randomMonster(EnemyHandle currentEnemy)
{
	Rect	oldRect; //, rect = g_Saric.where;
	EnemyPtr	temp;
	Boolean	stopped;
	short	i;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
	// increment leg counter
	temp->legCounter++;
	
	// if the leg counter is right...
	if(temp->legCounter >= 32)
	{
		// if we are allowed to fire...
		if((temp->attributes & canFire) == canFire)
		{
			// if we are lucky enough...
			if((shortRand() % (17 - temp->rateOfFire)) == 0)
			{
				// FIRE!!!!!!!
				fireEnemy(currentEnemy);
			}
		}
		
		// face a random direction 
		// (wow, thats random, even for me!)
		temp->facing = abs(shortRand() % 5);
		
//		printf("Random Facing: %d\n", temp->facing);
		
		// reset leg counter
		temp->legCounter = 16;
	}
	
	// if temp->legCounter % 4 == 0 
	if((temp->legCounter / 4) * 4 == temp->legCounter)
	{
		// change the leg state, whatever that does...
		temp->legState = 1 - temp->legState;
	}
	
	// move it in the correct direction
	switch(temp->facing)
	{
		case 1:
			offsetRect( &temp->where, temp->speed, 0 );
			break;
		case 2:
			offsetRect( &temp->where, 0, temp->speed );
			break;
		case 3:
			offsetRect( &temp->where, -temp->speed, 0 );
			break;
		case 4:
			offsetRect( &temp->where, 0, -temp->speed );
			break;
	}
	
// •• this kind of thing would be GREATLY accelerated in the form
// •• stopped = CheckThis() || CheckThat || …
// •• Because the code will stop calling the routines once it returns true.
// •• Oh, you need them all to be called anyway, right?  Nasty.  Yea.
// haha, you had a realization in mid-comment! :)
	
	// check to see if we intersect anything...
	stopped = checkEnemyInterceptWithSaric(currentEnemy);
	stopped += checkEnemyInterceptWithMap(currentEnemy);
	stopped += checkEnemyInterceptWithEnemies(currentEnemy);
	stopped += checkEnemyPushing(currentEnemy);
	
	// if we do...
	if(stopped)
	{
		// if its a missile, kill it
		if((temp->attributes & isMissile) == isMissile)
		{
			temp->health = 0;
		}
		
		// create a temporary random multiplier for the speed
		i = abs(shortRand() % 2) * 2 - 1;
		
		// move the enemy in a random direction
		switch(temp->facing)
		{
			case 1:
				offsetRect( &temp->where, 0,temp->speed * i);
				break;
			case 2:
				offsetRect( &temp->where, temp->speed * i, 0 );
				break;
			case 3:
				offsetRect( &temp->where, 0, -temp->speed * i );
				break;
			case 4:
				offsetRect( &temp->where, -temp->speed * i, 0 );
				break;
		}
		
		// hopefully this new place is not occupied...
		// if it is, pretend we never moved at all :(
		if(!enemyStandableRect(currentEnemy))
		{
			temp->where = oldRect;
		}
		
		// face ourselves in a random direction again...
		temp->facing = abs(shortRand() % 5);
	}
	
	// check to see if we hit the sword...
	if(checkProximityToSword(currentEnemy))
	{
		temp->facing = abs(shortRand() % 5);
		temp->legCounter = 250;
	}
}

// wow, that sounds scary...
// I'm too young to die!!!
// hopefully its not moving too fast...
void homingMonster(EnemyHandle currentEnemy)
{
	Rect	oldRect; //, rect = g_Saric.where;
	EnemyPtr	temp;
	Boolean	stopped;
	short	i;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
	// increment the leg counter
	temp->legCounter++;
	
	// if the leg counter is right...
	if(temp->legCounter >= 32)
	{
		// if we are allowed to fire...
		if((temp->attributes & canFire) == canFire)
		{
			// if we are lucky enough...
			if((shortRand() % (17 - temp->rateOfFire)) == 0)
			{
				// FIRE!!!!!
				fireEnemy(currentEnemy);
			}
		}
		
		// reset the leg counter...
		temp->legCounter = 16;
	}
	
	// if temp->legCounter % 4 == 0
	if((temp->legCounter / 4) * 4 == temp->legCounter)
	{
		// switch leg state
		temp->legState = 1 - temp->legState;
	}
	
	// reset facing direction
	temp->facing = 0;
	
	// reset the direction
	if(g_Saric.where.top - temp->where.top > 0)
	{
		temp->facing = 2;
	}
	
	if(g_Saric.where.top - temp->where.top < 0)
	{
		temp->facing = 4;
	}
	
	if(abs(g_Saric.where.left - temp->where.left) > abs(g_Saric.where.top - temp->where.top))
	{
		if(g_Saric.where.left - temp->where.left > 0)
		{
			temp->facing = 1;
		}
		
		if(g_Saric.where.left - temp->where.left < 0)
		{
			temp->facing = 3;
		}
	}
	
	// move in the desired direction
	switch(temp->facing)
	{
		case 1:
			offsetRect( &temp->where, temp->speed, 0 );
			break;
		case 2:
			offsetRect( &temp->where, 0, temp->speed );
			break;
		case 3:
			offsetRect( &temp->where, -temp->speed, 0 );
			break;
		case 4:
			offsetRect( &temp->where, 0, -temp->speed );
			break;
	}
	
	// check to see if we intersect anything
	stopped = checkEnemyInterceptWithSaric(currentEnemy);
	stopped += checkEnemyInterceptWithMap(currentEnemy);
	stopped += checkEnemyInterceptWithEnemies(currentEnemy);
	stopped += checkEnemyPushing(currentEnemy);
	
	// if we do...
	if(stopped)
	{
		// if it is a missile, kill it
		if((temp->attributes & isMissile) == isMissile)
		{
			temp->health = 0;
		}
		
		// find a random multiplier for the current speed
		i = (abs(shortRand() % 2)) * 2 - 1;
		
		// bump us in the desired direction
		switch(temp->facing)
		{
			case 1:
				offsetRect( &temp->where, 0,temp->speed*i);
				break;
			case 2:
				offsetRect( &temp->where, temp->speed*i, 0 );
				break;
			case 3:
				offsetRect( &temp->where, 0, -temp->speed*i );
				break;
			case 4:
				offsetRect( &temp->where, -temp->speed*i, 0 );
				break;
		}
		
		// check to see if we can stand in this new spot
		if(!enemyStandableRect(currentEnemy))
		{
			// pretend like we never moved...
			temp->where = oldRect;
		}
		
		// face a random direction
		temp->facing = abs(shortRand() % 5);
	}
	
	// check to see if we get hit by a sword...
	if(checkProximityToSword(currentEnemy))
	{
		temp->legCounter = 16;
	}
	
//	HUnlock((Handle)currentEnemy);
}

// not really sure what this does
// differently than everything else
void bumpTurnMonster(EnemyHandle currentEnemy)
{
	Rect	oldRect; //, rect = g_Saric.where;
	EnemyPtr	temp;
	Boolean	stopped;
	short	i;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
	// increment leg counter
	temp->legCounter++;
	
	// if the leg counter is right...
	if(temp->legCounter >= 32)
	{
		// if we are allowed to fire...
		if((temp->attributes & canFire) == canFire)
		{
			// if we are lucky enough...
			if((shortRand() % (17 - temp->rateOfFire)) == 0)
			{
				// FIRE!!!!!
				fireEnemy(currentEnemy);
			}
		}
		
		// reset leg counter
		temp->legCounter = 16;
	}
	
	// if temp->legCounter % 4 == 0
	if((temp->legCounter / 4) * 4 == temp->legCounter)
	{
		// switch leg state
		temp->legState = 1-temp->legState;
	}
	
	// if we have a certain movement type...
	if(temp->movementType == semibumpTurn)
	{
		// and we are lucky enough...
		if(shortRand() % 100 == 0)
		{
			// face a random direction
			temp->facing = abs(shortRand() % 5);
		}
	}
	
	// move in the desired direction
	switch(temp->facing)
	{
		case 0:
			temp->facing = abs(shortRand() % 5);
		case 1:
			offsetRect( &temp->where, temp->speed, 0 );
			break;
		case 2:
			offsetRect( &temp->where, 0, temp->speed );
			break;
		case 3:
			offsetRect( &temp->where, -temp->speed, 0 );
			break;
		case 4:
			offsetRect( &temp->where, 0, -temp->speed );
			break;
	}
	
	// check to see if we intersect anything...
	stopped = checkEnemyInterceptWithSaric(currentEnemy);
	stopped += checkEnemyInterceptWithMap(currentEnemy);
	stopped += checkEnemyInterceptWithEnemies(currentEnemy);
	stopped += checkEnemyPushing(currentEnemy);
	
	if(stopped)
	{
		// if its a missile, kill it
		if((temp->attributes & isMissile) == isMissile)
		{
			temp->health = 0;
		}
		
		// create a random multiplier for the current speed
		i = (abs(shortRand() % 2)) * 2 - 1;
		
		// move in the desired direction
		switch(temp->facing)
		{
			case 1:
				offsetRect( &temp->where, 0,temp->speed*i);
				break;
			case 2:
				offsetRect( &temp->where, temp->speed*i, 0 );
				break;
			case 3:
				offsetRect( &temp->where, 0, -temp->speed*i );
				break;
			case 4:
				offsetRect( &temp->where, -temp->speed*i, 0 );
				break;
		}
		
		// if we cannot stand in this new position...
		if(!enemyStandableRect(currentEnemy))
		{
			// ... then we pretend we never moved
			temp->where = oldRect;
		}
		
		// face a random direction again
		// (can't do this enough I guess)
		temp->facing = abs(shortRand() % 5);
	}
	
// •• Hu?
// hu?
// this makes sense to me...
	if(checkProximityToSword(currentEnemy))
	{
		temp->legCounter = 250;
		temp->facing = abs(shortRand() % 5);
	}
		
//	HUnlock((Handle)currentEnemy);
}

// homing only some of the time...
void semihomingMonster(EnemyHandle currentEnemy)
{
	Rect	oldRect;
	EnemyPtr	temp;
	Boolean	stopped;
	short	i;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
	// increment leg counter
	temp->legCounter++;
	
	// if the leg counter is high enough...
	if(temp->legCounter >= 32)
	{
		// if we are allowed to fire...
		if((temp->attributes & canFire) == canFire)
		{
			// if we are lucky enough...
			if((shortRand() % (17 - temp->rateOfFire)) == 0)
			{
				// FIRE!!!!!
				fireEnemy(currentEnemy);
			}
		}
		
		// new stuff this time!  exciting!
		
		// face a random direction (with an out of 
		// bounds possiblitity for some reason...)
		temp->facing = abs(shortRand() % 7);
		
		// if we were out of bounds...
		if(temp->facing > 4)
		{
			// this is the "homing" part of "semihoming"
			if(g_Saric.where.top - temp->where.top > 0)
			{
				temp->facing = 2;
			}
			
			if(g_Saric.where.top - temp->where.top < 0)
			{
				temp->facing = 4;
			}
			
			if(abs(g_Saric.where.left - temp->where.left) > abs(g_Saric.where.top - temp->where.top))
			{
				if(g_Saric.where.left - temp->where.left > 0)
				{
					temp->facing = 1;
				}
				
				if(g_Saric.where.left - temp->where.left < 0)
				{
					temp->facing = 3;
				}
			}
		}
		
		// reset the leg counter (as usual)
		temp->legCounter = 16;
	}
	
	// if temp->letCounter % 4 == 0
	if((temp->legCounter / 4) * 4 == temp->legCounter)
	{
		// switch the leg state
		temp->legState = 1 - temp->legState;
	}
	
	// move in the desired direction
	switch(temp->facing)
	{
		case 1:
			offsetRect( &temp->where, temp->speed, 0 );
			break;
		case 2:
			offsetRect( &temp->where, 0, temp->speed );
			break;
		case 3:
			offsetRect( &temp->where, -temp->speed, 0 );
			break;
		case 4:
			offsetRect( &temp->where, 0, -temp->speed );
			break;
	}
	
	// check to see if we intersect anything
	stopped = checkEnemyInterceptWithSaric(currentEnemy);
	stopped += checkEnemyInterceptWithMap(currentEnemy);
	stopped += checkEnemyInterceptWithEnemies(currentEnemy);
	stopped += checkEnemyPushing(currentEnemy);
	
	// if we do...
	if(stopped)
	{
		// if we are a missile, we die
		if((temp->attributes&isMissile) == isMissile)
		{
			temp->health = 0;
		}
		
		// find a random multiplier for the speed
		i = (abs(shortRand() % 2)) * 2 - 1;
		
		// bump us in the desired direction
		switch(temp->facing)
		{
			case 1:
				offsetRect( &temp->where, 0,temp->speed*i);
				break;
			case 2:
				offsetRect( &temp->where, temp->speed*i, 0 );
				break;
			case 3:
				offsetRect( &temp->where, 0, -temp->speed*i );
				break;
			case 4:
				offsetRect( &temp->where, -temp->speed*i, 0 );
				break;
		}
		
		// if we can't stand here...
		if(!enemyStandableRect(currentEnemy))
		{
			// pretend we never moved
			temp->where = oldRect;
		}
		
		// face a random direction
		temp->facing = abs(shortRand() % 5);
	}
	
	// check to see if we get hit by the sword
	if(checkProximityToSword(currentEnemy))
	{
		temp->legCounter = 250;
		temp->facing = abs(shortRand() % 5);
	}
		
//	HUnlock((Handle)currentEnemy);
}

// appears to pace back and forth or something...
// maybe it just stays within a certian distance of something...
void gaurdianMonster(EnemyHandle currentEnemy)
{
	Rect	oldRect; //, rect = g_Saric.where;
	EnemyPtr	temp;
	Boolean	stopped;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
	// increment leg counter
	temp->legCounter++;
	
	// fix the guardian range if it is out of range (haha)
	if(temp->gaurdianRange > 6)
	{
		temp->gaurdianRange = 6;
	}
	
	// if the leg counter is right...
	if(temp->legCounter >= 16 + 16 * temp->gaurdianRange)
	{
		// if we are allowed to fire...
		if((temp->attributes & canFire) == canFire)
		{
			// if we are lucky enough...
			if((shortRand() % (17 - temp->rateOfFire)) == 0)
			{
				// FIRE!!!!!
				fireEnemy(currentEnemy);
			}
		}
		
		// turn around
		temp->facing = temp->facing+2;
		if(temp->facing > 4)
		{
			temp->facing -= 4;
		}
		
		// reset leg counter
		temp->legCounter = 16;
	}
	
	// if leg counter mod 4 is 0, switch leg state
	// (english this time...  I wonder why...)
	if((temp->legCounter / 4) * 4 == temp->legCounter)
	{
		temp->legState = 1 - temp->legState;
	}
	
	// move in the desired direction
	switch(temp->facing)
	{
		case 0:
			temp->facing = abs(shortRand() % 5);
		case 1:
			offsetRect( &temp->where, temp->speed * 2, 0 );
			break;
		case 2:
			offsetRect( &temp->where, 0, temp->speed * 2 );
			break;
		case 3:
			offsetRect( &temp->where, -temp->speed * 2, 0 );
			break;
		case 4:
			offsetRect( &temp->where, 0, -temp->speed * 2 );
			break;
	}
	
	// check to see if we intersect anything
	stopped = checkEnemyInterceptWithSaric(currentEnemy);
	stopped += checkEnemyInterceptWithMap(currentEnemy);
	stopped += checkEnemyInterceptWithEnemies(currentEnemy);
	stopped += checkEnemyPushing(currentEnemy);
	
	// if we do...
	if(stopped)
	{
		// if we are a missile, we die
		if((temp->attributes & isMissile) == isMissile)
		{
			temp->health = 0;
		}
		
		// pretend we never moved
		temp->where = oldRect;
	}
	
//	HUnlock((Handle)currentEnemy);
}

void circlingMonster(EnemyHandle currentEnemy)
{	Rect		oldRect,dest; //, rect = g_Saric.where;
	EnemyPtr	temp;
	Boolean		stopped;
	
//	HLock( (Handle)currentEnemy );
	temp = *currentEnemy;
	oldRect = temp->where;
	
	// increment leg counter
	temp->legCounter++;
	
	// if the leg counter is right...
	if(temp->legCounter >= 32)
	{
		// if we are allowed to fire...
		if((temp->attributes & canFire) == canFire)
		{
			// if we are lucky enough...
			if((shortRand() % (17 - temp->rateOfFire)) == 0)
			{
				// FIRE!!!!!
				fireEnemy(currentEnemy);
			}
		}
		
		// reset leg counter
		temp->legCounter = 16;
	}
	
	// I am guessing that all this makes 
	// the enemy go in a circle
	temp->facing = ((temp->legCounter / 2) % 4) + 1;
	temp->legState = temp->legCounter % 2;
	
	if((temp->legCounter % 16) > 7)
	{
		temp->facing = 5 - temp->facing;
		temp->legState = !temp->legState;
	}
	
	temp->theta += temp->speed;
	
	dest.left = 256 + (((long)cosof[temp->theta] * (long)temp->disFromUnitCircle) / (long)32768);
	dest.top = 160 + (((long)sineof[temp->theta] * (long)temp->disFromUnitCircle) / (long)32768);
	
	temp->where.left += (dest.left - temp->where.left) / 2;
	temp->where.top += (dest.top - temp->where.top) / 2;
	temp->where.right = temp->where.left + 32;
	temp->where.bottom = temp->where.top + 32;
	
	// I know this one!!!
	// check to see if we intersect anything
	stopped = checkEnemyInterceptWithSaric(currentEnemy);
	stopped += checkEnemyInterceptWithMap(currentEnemy);
	stopped += checkEnemyInterceptWithEnemies(currentEnemy);
	stopped += checkEnemyPushing(currentEnemy);
	
	// if we do...
	if(stopped)
	{
		// if we are a missile, kill us
		if((temp->attributes & isMissile) == isMissile)
		{
			temp->health = 0;
		}
	}
	
//	HUnlock((Handle)currentEnemy);
}

// SMART?!!?!?!?!?! this can't be good...
// doesn't look that smart to me...
// it doesn't even appear to move...
void smartMonster(EnemyHandle currentEnemy)
{
	Rect	oldRect;
	EnemyPtr	temp;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
// •• Gee, this too.  Hopefully these will be taken out soon.
	// hmm, we check for the intercepts, but
	// we apparently don't care if we do or not
	checkEnemyInterceptWithSaric(currentEnemy);
	checkEnemyPushing(currentEnemy);
	
	// increment leg counter
	temp->legCounter++;
	
	// if the leg counter is right...
	if(temp->legCounter >= 32)
	{
		// if we are allowed to fire...
		if((temp->attributes & canFire) == canFire)
		{
			// if we are lucky enough...
			if((shortRand() % (17 - temp->rateOfFire)) == 0)
			{
				// FIRE!!!!!
				fireEnemy(currentEnemy);
			}
		}
		
		// reset leg counter
		temp->legCounter = 16;
	}
	
//	HUnlock((Handle)currentEnemy);
}

// probably moving in a straight line
void linearMonster(EnemyHandle currentEnemy)
{
	Rect	oldRect;
	EnemyPtr	temp;
	Boolean	stopped;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
	// increment leg counter
	temp->legCounter++;
	
	// if the leg counter is right...
	if(temp->legCounter >= 32)
	{
		// if we are allowed to fire...
		if((temp->attributes & canFire) == canFire)
		{
			// if we are lucky enough...
			if((shortRand() % (17 - temp->rateOfFire)) == 0)
			{
				fireEnemy(currentEnemy);
			}
		}
		
		temp->legCounter = 16;
	}
	
	// if leg counter % 4 == 0, switch leg state
	if((temp->legCounter / 4) * 4 == temp->legCounter)
	{
		temp->legState = 1 - temp->legState;
	}
	
	// move us in the right direction
	switch(temp->facing)
	{
		case 0:
			temp->facing = abs(shortRand() % 5);
		case 1:
			offsetRect( &temp->where, temp->speed, 0 );
			break;
		case 2:
			offsetRect( &temp->where, 0, temp->speed );
			break;
		case 3:
			offsetRect( &temp->where, -temp->speed, 0 );
			break;
		case 4:
			offsetRect( &temp->where, 0, -temp->speed );
			break;
	}
	
	// check to see if we intersect anything
	stopped = checkEnemyInterceptWithSaric(currentEnemy);
	stopped += checkEnemyInterceptWithMap(currentEnemy);
	stopped += checkEnemyInterceptWithEnemies(currentEnemy);
	stopped += checkEnemyPushing(currentEnemy);
	
	// if we do...
	if(stopped)
	{
		// if we are a missile, kill us
		if((temp->attributes & isMissile) == isMissile)
		{
			temp->health = 0;
		}
		
		temp->where = oldRect;
		temp->facing = abs(shortRand()%5);
	}
			
//	HUnlock((Handle)currentEnemy);
}

// this is probably for the giant red blogs
// that "shoot" smaller red blobs out for you to kill
// yay!!! lots of experience for us
void blobBossMonster(EnemyHandle currentEnemy)
{
	Rect	oldRect; //, rect = g_Saric.where;
	EnemyPtr	temp;
	Boolean	stopped;
	short	i, j;
	Point	whereIndex;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
	// increment leg counter
	temp->legCounter++;
	
	// if we are close enough to dying...
	if(temp->health <= 10)
	{
		// create 4 smaller blobs
		for(i = 0;i <= 1;i++)
		{
			for(j = 0;j <= 1;j++)
			{
				whereIndex.h = i;
				whereIndex.v = j;
				bossFireEnemy(currentEnemy, &whereIndex,temp->firedEnemy,false,false);
			}
		}
		
		// and die
		temp->health = 0;
	}
	
	// if the leg counter is right...
	if(temp->legCounter >= 32)
	{
		// if we are allowed to fire and we satisfy some other requirement...
		if((temp->attributes&canFire) == canFire && !(temp->movePhase & 4))
		{
			// fire 4 new guys!!!
			for(i = -1;i <= 2;i += 3)
			{
				for(j = -1;j <= 2;j += 3)
				{
					whereIndex.h = i;
					whereIndex.v = j;
					bossFireEnemy(currentEnemy, &whereIndex,temp->firedEnemy,true,false);
				}
			}
		}
		
		// face a random direction
		temp->facing = abs(shortRand() % 5);
		
		// reset leg counter
		temp->legCounter = 16;
		
		// increment move phase
		temp->movePhase++;
	}
	
	// if leg counter % 4 == 0, switch leg state
	// I think I am finally getting used to all
	// these random workarounds for simple logic
	if((temp->legCounter / 4) * 4 == temp->legCounter)
	{
		temp->legState = 1 - temp->legState;
	}
	
	// move in the desired direction
	switch(temp->facing)
	{
		case 1:
			offsetRect( &temp->where, temp->speed, 0 );
			break;
		case 2:
			offsetRect( &temp->where, 0, temp->speed );
			break;
		case 3:
			offsetRect( &temp->where, -temp->speed, 0 );
			break;
		case 4:
			offsetRect( &temp->where, 0, -temp->speed );
			break;
	}
	
// •• this kind of thing would be GREATLY accelerated in the form
// •• stopped = CheckThis() || CheckThat || …
// •• Because the code will stop calling the routines once it returns true.
// •• Oh, you need them all to be called anyway, right?  Nasty.  Yea.
	
	// nasty is right... maybe I should fix this later on...
	stopped = checkEnemyInterceptWithSaric(currentEnemy);
	stopped += checkEnemyInterceptWithMap(currentEnemy);
	stopped += checkEnemyInterceptWithEnemies(currentEnemy);
	stopped += checkEnemyPushing(currentEnemy);
	
	// if we hit something...
	if(stopped)
	{
		// if we are a missile, kill us
		if((temp->attributes & isMissile) == isMissile)
		{
			temp->health = 0;
		}
		
		// create a random multiplier for the current speed
		i = (abs(shortRand() % 2)) * 2 - 1;
		
		// move in the desired random direction
		switch(temp->facing)
		{
			case 1:
				offsetRect( &temp->where, 0,temp->speed * i);
				break;
			case 2:
				offsetRect( &temp->where, temp->speed * i, 0 );
				break;
			case 3:
				offsetRect( &temp->where, 0, -temp->speed * i );
				break;
			case 4:
				offsetRect( &temp->where, -temp->speed * i, 0 );
				break;
		}
		
		// if we can't stand here, pretend we never moved
		if(!enemyStandableRect(currentEnemy))
		{
			temp->where = oldRect;
		}
		
		temp->facing = abs(shortRand() % 5);
	}
	
	// check if we get hit by the sword
	if(checkProximityToSword(currentEnemy))
	{
		temp->facing = abs(shortRand() % 5);
		temp->legCounter = 250;
	}
}

// I can't tell what this one is...
// it appears to move in circles
// might be that giant fly...
void hiveBossMonster(EnemyHandle currentEnemy)
{
	Rect		oldRect, dest; //, rect = g_Saric.where;
	EnemyPtr	temp;
	Boolean		stopped;
	Point	where;
	
//	HLock( (Handle)currentEnemy );
	temp = *currentEnemy;
	oldRect = temp->where;
	
	// increment leg counter
	temp->legCounter++;
	
	// if the leg counter is right...
	if(temp->legCounter >= 32)
	{
		// if we are allowed to fire...
		if((temp->attributes & canFire) == canFire)
		{
			// if we have waited long enough...
			if(temp->movePhase & 12)
			{
				// FIRE!!!!!
				where.h = 2;
				where.v = 2;
				bossFireEnemy(currentEnemy, &where, temp->firedEnemy, true, false);
			}
		}
		
		// reset leg counter
		temp->legCounter = 16;
		temp->movePhase++;
	}
	
	// this looks a lot like a circular monster
	// I assume it does something similar
	temp->facing = ((temp->legCounter / 2) % 4) + 1;
	temp->legState = temp->legCounter % 2;
	
	if((temp->legCounter % 16) > 7)
	{
		temp->facing = 5 - temp->facing;
		temp->legState = !temp->legState;
	}
	
	temp->theta += temp->speed;
	
	dest.left = g_Saric.where.left - 16 + (((long)cosof[temp->theta] * (long)temp->disFromUnitCircle) / (long)32768);
	dest.top = g_Saric.where.top - 16 + (((long)sineof[temp->theta] * (long)temp->disFromUnitCircle) / (long)32768);
	
	temp->where.left += (dest.left - temp->where.left) / 8;
	temp->where.top += (dest.top - temp->where.top) / 8;
	temp->where.right = temp->where.left + 32;
	temp->where.bottom = temp->where.top + 32;
	
	// I know this part!!!
	// check to see if we intersect anything
	stopped = checkEnemyInterceptWithSaric(currentEnemy);
	stopped += checkEnemyInterceptWithMap(currentEnemy);
	stopped += checkEnemyInterceptWithEnemies(currentEnemy);
	stopped += checkEnemyPushing(currentEnemy);
	
	// if we do...
	if(stopped)
	{
		// if we are a missile, kill us
		if((temp->attributes & isMissile) == isMissile)
		{
			temp->health = 0;
		}
	}
	
//	HUnlock((Handle)currentEnemy);
}

void crabBossMonster(EnemyHandle currentEnemy)
{
	Rect		oldRect; //, rect = g_Saric.where;
	EnemyPtr	temp;
	Boolean		stopped;
	short		i;
	Point		whereIndex;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
	// increment leg counter
	temp->legCounter++;
	
	// if the leg counter is right...
	if(temp->legCounter >= 32)
	{
		// if we are allowed to fire
		if((temp->attributes & canFire) && (temp->movePhase & 4) == 0)
		{
			// FIRE!!!
			whereIndex.h = 1;
			whereIndex.v = 1;
			bossFireEnemy(currentEnemy, &whereIndex, 2024, true, false);
			
			// FIRE AGAIN!!!
			whereIndex.h = 0;
			whereIndex.v = 1;
			bossFireEnemy(currentEnemy, &whereIndex, 2024, true, false);
		}
		
		temp->facing = abs(shortRand() % 5);
		temp->legCounter = 16;
		temp->movePhase++;
	}
	
	// if legCounter % 4 == 0
	if((temp->legCounter / 4) * 4 == temp->legCounter)
	{
		// switch leg state
		temp->legState = 1 - temp->legState;
	}
	
	// if the move phase is correct
	if(temp->movePhase & 3)
	{
		// actually move
		temp->angledCourse.h = 0;
		temp->angledCourse.v = 0;
		
		switch(temp->facing)
		{
			case 1:
				offsetRect(&temp->where, temp->speed, 0);
				break;
			case 2:
				offsetRect(&temp->where, 0, temp->speed);
				break;
			case 3:
				offsetRect(&temp->where, -temp->speed, 0);
				break;
			case 4:
				offsetRect(&temp->where, 0, -temp->speed);
				break;
		}
	}
	// otherwise
	else
	{
		// face the right direction
		if(temp->angledCourse.h == 0 && temp->angledCourse.v == 0)
		{
			short x, y;
			
			x = g_Saric.where.left - temp->where.left;
			y = g_Saric.where.top - temp->where.top;
			
	// •• you call abs twice here..  Oh well, this isn't too heavily looped.
			// doesn't really matter anymore
			// thank god for modern processors!
			temp->angledCourse.h = abs((10 * x) / (abs(x) + abs(y)));
			temp->angledCourse.v = abs((10 * y) / (abs(x) + abs(y)));
			
			if(x < 0)
			{
				temp->angledCourse.h *= -1;
			}
			
			if(y < 0)
			{
				temp->angledCourse.v *= -1;
			}
			
			if(abs(temp->angledCourse.h) - abs(temp->angledCourse.v) > 0)
			{
				if(temp->angledCourse.h > 0)
				{
					temp->facing = 1;
				}
				else
				{
					temp->facing = 3;
				}
			}
			else
			{
				if(temp->angledCourse.v > 0)
				{
					temp->facing = 2;
				}
				else
				{
					temp->facing = 4;
				}
			}
		}
		
		// oh, we do move here...
		// what is the difference between 
		// this and the 'if' part?
		temp->where.top += temp->angledCourse.v;
		temp->where.left += temp->angledCourse.h;
		temp->where.bottom += temp->angledCourse.v;
		temp->where.right += temp->angledCourse.h;
	}
	
// •• this kind of thing would be GREATLY accelerated in the form
// •• stopped = CheckThis() || CheckThat || …
// •• Because the code will stop calling the routines once it returns true.
// •• Oh, you need them all to be called anyway, right?  Nasty.  Yea.
	stopped = checkEnemyInterceptWithSaric(currentEnemy);
	stopped += checkEnemyInterceptWithMap(currentEnemy);
	stopped += checkEnemyInterceptWithEnemies(currentEnemy);
	stopped += checkEnemyPushing(currentEnemy);
	
	// if we hit something
	if(stopped)
	{
		// if we are a missile, kill us
		if((temp->attributes & isMissile) == isMissile)
		{
			temp->health = 0;
		}
		
		// create a random amount to move
		// it should be either 1 or -1
		i = abs(shortRand() % 2) * 2 - 1;
		
		// move in the desired direction
		switch(temp->facing)
		{
			case 1:
				offsetRect(&temp->where, 0,temp->speed * i);
				break;
			case 2:
				offsetRect(&temp->where, temp->speed * i, 0);
				break;
			case 3:
				offsetRect(&temp->where, 0, -temp->speed * i);
				break;
			case 4:
				offsetRect(&temp->where, -temp->speed * i, 0);
				break;
		}
		
		// this line appears to make the previous part pointless
		// we do all this stuff to move it, then just revert
		temp->where = oldRect;
		temp->facing = abs(shortRand()%5);
	}
	
	// if we are hit by the sword, get bumped
	if(checkProximityToSword(currentEnemy))
	{
		temp->facing = abs(shortRand() % 5);
		temp->legCounter = 250;
	}
}

// possibly the giant soldier?  
void sentryBossMonster(EnemyHandle currentEnemy)
{
	Rect	oldRect; //, rect = g_Saric.where;
	EnemyPtr	temp;
	Boolean	stopped;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
	// increment leg counter (angle too?)
	temp->legCounter++;
	temp->theta++;
	
	// apparently it is guarding something...
	if(temp->gaurdianRange > 6)
	{
		temp->gaurdianRange = 6;
	}
	
	// face a certain direction
	temp->facing = 4;
	
	// if the leg counter is too high, reset it
	if(temp->legCounter >= 32)
	{
		temp->legCounter = 16;
	}
	
	// if we are facing the right way...
	if(temp->theta == 12)
	{
		// if we are allowed to fire...
		if((temp->attributes & canFire) == canFire)
		{
			// if we meet that weird special condition...
			if(!(temp->movePhase % 2))
			{
				// FIRE!!!
				Point whereIndex;
				whereIndex.h = 1;
				whereIndex.v = -2;
				bossFireEnemy(currentEnemy, &whereIndex, 2007, false, true);
			}
		}
	}
	
	// if the angle is high enough
	if(temp->theta >= 16)
	{
		// reset it and increase the move phase
		temp->movePhase++;
		temp->theta = 0;
	}
	
	// if legCounter % 4 == 0, switch legState
	if((temp->legCounter / 4) * 4 == temp->legCounter)
	{
		temp->legState = 1 - temp->legState;
	}
	
	// move forward or backward, depending on the move phase
	if(temp->movePhase % 2)
	{
		offsetRect(&temp->where, -temp->speed * 2, 0);
	}
	else
	{
		offsetRect(&temp->where, temp->speed * 2, 0);
	}
	
	// check to see if we hit something
	checkEnemyInterceptWithSaric(currentEnemy);
	stopped = checkEnemyInterceptWithMap(currentEnemy);
	checkEnemyInterceptWithEnemies(currentEnemy);
	stopped += checkEnemyPushing(currentEnemy);
	
	// if we hit something (only the map or pushing something)
	// if we hit saric or another enemy, t.s.
	if(stopped)
	{
		if((temp->attributes & isMissile) == isMissile)
		{
			temp->health = 0;
		}
		
		temp->where = oldRect;
	}
	
//	HUnlock((Handle)currentEnemy);
}


void linearBossMonster(EnemyHandle currentEnemy)
{
	Rect	oldRect;
	EnemyPtr	temp;
	Boolean	stopped;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
	// increment leg counter
	temp->legCounter++;
	
	// if the leg counter is right...
	if(temp->legCounter >= 32)
	{
		// if we are allowed to fire...
		if((temp->attributes & canFire) == canFire)
		{
			// if we are lucky enough...
			if((shortRand() % (17 - temp->rateOfFire)) == 0)
			{
				// FIRE!!!!
				fireEnemy(currentEnemy);
			}
		}
		
		// reset leg counter
		temp->legCounter = 16;
	}
	
	// if legCounter % 4 == 0, switch leg counter
	if((temp->legCounter / 4) * 4 == temp->legCounter)
	{
		temp->legState = 1 - temp->legState;
	}
	
	// move lineary, like we promised...
	switch(temp->facing)
	{
		case 0:
			temp->facing = abs(shortRand() % 5);
		case 1:
			offsetRect(&temp->where, temp->speed, 0);
			break;
		case 2:
			offsetRect(&temp->where, 0, temp->speed);
			break;
		case 3:
			offsetRect(&temp->where, -temp->speed, 0);
			break;
		case 4:
			offsetRect(&temp->where, 0, -temp->speed);
			break;
	}
	
	// check and see if we hit something
	stopped = checkEnemyInterceptWithSaric(currentEnemy);
	stopped += checkEnemyInterceptWithMap(currentEnemy);
	stopped += checkEnemyInterceptWithEnemies(currentEnemy);
	stopped += checkEnemyPushing(currentEnemy);
	
	// if we did...
	if(stopped)
	{
		// if we are a missile, kill us
		if((temp->attributes & isMissile) == isMissile)
		{
			temp->health = 0;
		}
		
		// undo our move
		temp->where = oldRect;
		temp->facing = abs(shortRand() % 5);
	}
			
//	HUnlock((Handle)currentEnemy);
}

void rhinoBossMonster(EnemyHandle currentEnemy)
{
	Rect		oldRect; //, rect = g_Saric.where;
	EnemyPtr	temp;
	Boolean		stopped;
	short		i;
	Point		whereIndex;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
	temp->legCounter++;
	
	// if the counter is right...
	if(temp->legCounter >= 32)
	{
		// if we are allowed to fire...
		if((temp->attributes & canFire) && (temp->movePhase & 4) == 0)
		{
			// decide what direction to fire in
			switch(temp->facing)
			{
				case 0:
				case 1:
					whereIndex.h = 2;
					whereIndex.v = 0;
					break;
				case 2:
					whereIndex.h = 0;
					whereIndex.v = 2;
					break;
				case 3:
					whereIndex.h = -1;
					whereIndex.v = 0;
					break;
				case 4:
					whereIndex.h = 0;
					whereIndex.v = -1;
					break;
			}
			
			// FIRE!!!!!
			bossFireEnemy(currentEnemy, &whereIndex, 2040, true, false);
		}
		
		temp->facing = abs(shortRand() % 5);
		temp->legCounter = 16;
		temp->movePhase++;
	}
	
	// if legcounter % 4 == 0
	if((temp->legCounter / 4) * 4 == temp->legCounter)
	{
		// fix the leg state
		temp->legState = 1 - temp->legState;
	}
	
	// if the move phase is right,
	// move vertically or horizontally
	if(temp->movePhase & 3)
	{
		temp->angledCourse.h = 0;
		temp->angledCourse.v = 0;
		
		switch(temp->facing)
		{
			case 1:
				offsetRect(&temp->where, temp->speed, 0);
				break;
			case 2:
				offsetRect(&temp->where, 0, temp->speed);
				break;
			case 3:
				offsetRect(&temp->where, -temp->speed, 0);
				break;
			case 4:
				offsetRect(&temp->where, 0, -temp->speed);
				break;
		}
	}
	// otherwise move towards Saric
	else
	{
		if(temp->angledCourse.h == 0 && temp->angledCourse.v == 0)
		{
			short	x,y;
			
			// set the angle to point towards saric
			x = g_Saric.where.left - temp->where.left;
			y = g_Saric.where.top - temp->where.top;
	// •• you call abs twice here..  Oh well, this isn't too heavily looped.
			temp->angledCourse.h = abs((7 * x)/(abs(x) + abs(y)));
			temp->angledCourse.v = abs((7 * y)/(abs(x) + abs(y)));
			
			if(x < 0)
			{
				temp->angledCourse.h *= -1;
			}
			
			if(y < 0)
			{
				temp->angledCourse.v *= -1;
			}
			
			if(abs(temp->angledCourse.h) - abs(temp->angledCourse.v) > 0)
			{
				if(temp->angledCourse.h > 0)
				{
					temp->facing = 1;
				}
				else
				{
					temp->facing = 3;
				}
			}
			else
			{
				if(temp->angledCourse.v > 0)
				{
					temp->facing = 2;
				}
				else
				{
					temp->facing = 4;
				}
			}
		}
		
		temp->where.top += temp->angledCourse.v;
		temp->where.left += temp->angledCourse.h;
		temp->where.bottom += temp->angledCourse.v;
		temp->where.right += temp->angledCourse.h;
	}
	
// •• this kind of thing would be GREATLY accelerated in the form
// •• stopped = CheckThis() || CheckThat || …
// •• Because the code will stop calling the routines once it returns true.
// •• Oh, you need them all to be called anyway, right?  Nasty.  Yea.
	stopped = checkEnemyInterceptWithSaric(currentEnemy);
	stopped += checkEnemyInterceptWithMap(currentEnemy);
	stopped += checkEnemyInterceptWithEnemies(currentEnemy);
	stopped += checkEnemyPushing(currentEnemy);
	
	// if we had any problems along the way
	if(stopped)
	{
		// if we are a missile, kill us
		if((temp->attributes & isMissile) == isMissile)
		{
			temp->health = 0;
		}
		
		// bump ourselves once again
		i=(abs(shortRand() % 2)) * 2 - 1;
		
		switch(temp->facing)
		{
			case 1:
				offsetRect(&temp->where, 0,temp->speed * i);
				break;
			case 2:
				offsetRect(&temp->where, temp->speed * i, 0);
				break;
			case 3:
				offsetRect(&temp->where, 0, -temp->speed * i);
				break;
			case 4:
				offsetRect(&temp->where, -temp->speed * i, 0);
				break;
		}
		
		temp->where = oldRect;
		temp->facing = abs(shortRand() % 5);
	}
	
	// check to see if we are getting stabbed...
	if(checkProximityToSword(currentEnemy))
	{
		temp->facing = abs(shortRand() % 5);
		temp->legCounter = 250;
	}
}

void elementalBossMonster(EnemyHandle currentEnemy)
{
	Rect	oldRect;
	EnemyPtr	temp;
	Boolean	stopped;
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
	// increment the leg counter
	temp->legCounter++;
	
	// if the leg counter is right...
	if(temp->legCounter >= 32)
	{
		// if we are allowed to fire...
		if((temp->attributes&canFire) == canFire)
		{
			// if we are lucky enough...
			if((shortRand()%(17-temp->rateOfFire)) == 0)
			{
				// FIRE!!!!!
				fireEnemy(currentEnemy);
			}
		}
		
		// reset the leg counter
		temp->legCounter = 16;
	}
	
	// if legCounter % 4 == 0
	if((temp->legCounter / 4) * 4 == temp->legCounter)
	{
		// change the leg state
		temp->legState = 1 - temp->legState;
	}
	
	// move in the right direction
	switch(temp->facing)
	{
		case 0:
			temp->facing = abs(shortRand() % 5);
		case 1:
			offsetRect(&temp->where, temp->speed, 0);
			break;
		case 2:
			offsetRect(&temp->where, 0, temp->speed);
			break;
		case 3:
			offsetRect(&temp->where, -temp->speed, 0);
			break;
		case 4:
			offsetRect(&temp->where, 0, -temp->speed);
			break;
	}
	
	// check to see if we hit anything
	checkEnemyInterceptWithSaric(currentEnemy);
	stopped = checkEnemyInterceptWithMap(currentEnemy);
	checkEnemyInterceptWithEnemies(currentEnemy);
	stopped += checkEnemyPushing(currentEnemy);
	
	// if we did...
	if( stopped )
	{
		/// kill us
		temp->health = 0;
		
		// become a missile?
		temp->attributes |= isMissile;
	}
			
//	HUnlock((Handle)currentEnemy);
}

/////////////////////////////////
// Unfixed routines
/////////////////////////////////

/**************** FinalBossMonster ****************/
void finalBossMonster(EnemyHandle currentEnemy)
{
	Rect		oldRect;
	EnemyPtr	temp;
//	Boolean		stopped; not used right now…
	
//	HLock((Handle)currentEnemy);
	temp = *currentEnemy;
	oldRect = temp->where;
	
	// increment leg counter
	temp->legCounter++;
	
	// increment theta
	temp->theta++;
	
	// make sure we only face one way
	temp->facing = 2;
	
	// reset leg counter if high enough...
	if(temp->legCounter >= 32)
	{
		temp->legCounter = 16;
	}
	
	// if legCounter % 4 == 0
	if((temp->legCounter / 4) * 4 == temp->legCounter)
	{
		// switch leg state
		temp->legState = 1 - temp->legState;
	}
	
	// if we aren't stuck and Saric is close enough
	if(!temp->stuckCounter && ((g_Saric.where.bottom / 32) < 9))
	{
		Point where;
		
		// mark some of the squares as not standable
		g_MapScreens[g_CurrentScreen].tiles[(6 * 10) + 9].modifiers &= ~standable;
		g_MapScreens[g_CurrentScreen].tiles[(7 * 10) + 9].modifiers &= ~standable;
		g_MapScreens[g_CurrentScreen].tiles[(8 * 10) + 9].modifiers &= ~standable;
		g_MapScreens[g_CurrentScreen].tiles[(9 * 10) + 9].modifiers &= ~standable;
		temp->stuckCounter = 1;
		
		// fire enemies
		where.h = 0;
		where.v = 6;
		bossFireEnemy(currentEnemy, &where, 2020, false, false);
		where.h = 1;
		where.v = 6;
		bossFireEnemy(currentEnemy, &where, 2020, false, false);
	}
	
	// if theta is large enough...
	if(temp->theta >= 15)
	{
		// if we haven't moved in a long enough time
		if(!(temp->movePhase%4))
		{
			Point where;
			
			// fire enemies...
			where.h = 0;
			where.v = 2;
			bossFireEnemy(currentEnemy, &where, 2108, true, false);
			where.h = 1;
			where.v = 2;
			bossFireEnemy(currentEnemy, &where, 2108, true, false);
		}
		
		// if the move phase is right...
		if(!((temp->movePhase + 2) % 4))
		{
			Point	where;
			
			// fire enemies...
			where.h = 0;
			where.v = 2;
			bossFireEnemy(currentEnemy, &where, 2035, false, true);
			where.h = 1;
			where.v = 2;
			bossFireEnemy(currentEnemy, &where, 2035, false, true);
		}
		
		// reset theta
		temp->theta = 0;
		
		// increment move phase
		temp->movePhase++;
	}
	
	// reset move phase if high enough
	if(temp->movePhase >= 32)
	{
		temp->movePhase = 0;
	}
	
	// check to see if we hit anything
	// more likely if anyone hit us like Saric...
	checkEnemyInterceptWithSaric(currentEnemy);
	checkEnemyInterceptWithMap(currentEnemy);
	checkEnemyInterceptWithEnemies(currentEnemy);
	checkEnemyPushing(currentEnemy);
	
	// if his health is too low...
	if(temp->health <= 10)
	{
		// WE WIN!!!
		winGame();
	}
}



