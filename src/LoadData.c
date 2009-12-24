/*
 *  LoadData.c
 *  Mantra
 *
 *  Created by Chris O'Neill on 1/14/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#if defined(MANTRA_WINDOWS)
//#include "StdAfx.h"
#include <allegro.h>
#else
#include <Allegro/allegro.h>
#endif

#include <alfont.h>

#include <string.h>

#include "GameTypes.h"
#include "GameDefines.h"
#include "GameConstants.h"
#include "Utils.h"
#include "Input.h"
#include "Sound.h"
#include "Saric.h"
#include "Map.h"

#include "LoadData.h"

typedef struct {
	short id;
	long len;
} IconHeader;

int overallsize = 0;

#define BITSWAP(a) bitswap(&(a),sizeof(a));

#define VERIFYFREAD(var,file) \
if (pack_fread(&var,sizeof(var),file)<((signed)sizeof(var)))\
{\
	return 0;\
}else\
{\
	overallsize += sizeof(var);\
}

#define VERIFYFWRITE(var,file) \
if (pack_fwrite(&var,sizeof(var),file)<((signed)sizeof(var)))\
{\
	return 0;\
}else\
{\
	overallsize += sizeof(var);\
}

//#define IS_VALID_ASCII_LETTER(a) ((((a)>='0')&&((a)<='9'))||(((a)>='A')&&((a)<='Z'))||(((a)>='a')&&((a)<='z')))
#define IS_VALID_ASCII_LETTER(a) (((a)>=' ')&&((a)<='z'))

void bitswap(void *vdata, unsigned int len)
{
	unsigned char *data = (unsigned char *)vdata;
	unsigned char temp;
	unsigned int i;
	
	//	printf("Length: %d\n", len);
	for(i = 0;i < len / 2;i++)
	{
		//		printf("Swapping %X and %X\n", data[i], data[len - 1 - i]);
		temp = data[i];
		data[i] = data[len - 1 - i];
		data[len - 1 - i] = temp;
		//		printf("Now we have %X and %X\n", data[i], data[len - 1 - i]);
	}
}

int myUnpackBitsSrc(void *vsrc, void *vdest, unsigned int numSrcBytes)
{
	unsigned char *dest = (unsigned char *)vdest;
	signed char *src = (signed char *)vsrc;
	
	unsigned int currentDest = 0;
	unsigned char byteCount;
	unsigned int i, j;
	
	for(i = 0;i < numSrcBytes;i++)
	{
		if (src[i] < 0)
		{
			byteCount = ((-1)*src[i]) + 1;
			i++;
			
			if (i >= numSrcBytes)
			{
//				printf("Stopped in the middle of a repeat\n");
				return 0;
			}
			
			for(j = currentDest;j < currentDest + byteCount;j++)
			{
				dest[j] = (unsigned char)src[i];
			}
			
			currentDest += byteCount;
		}
		else
		{
			byteCount = src[i] + 1;
			i++;
			
			if (i >= numSrcBytes)
			{
//				printf("Stopped in the middle of a block\n");
				return 0;
			}
			
			for(j = currentDest;j < currentDest + byteCount;j++)
			{
				dest[j] = (unsigned char)src[i];
				i++;
				
				if (j < currentDest + byteCount - 1 && i >= numSrcBytes)
				{
//					printf("Stopped in the middle of the loop\n");
					return 0;
				}
			}
			
			currentDest += byteCount;
			
			i--;
		}
	}
	
	return currentDest;
}

int myUnpackBitsDest(void *vsrc, void *vdest, unsigned int numDestBytes)
{
	unsigned char *dest = (unsigned char *)vdest;
	signed char *src = (signed char *)vsrc;
	
	unsigned int currentDest = 0;
	unsigned char byteCount;
	unsigned int i, j;
	
	for(i = 0;;i++)
	{
		if (src[i] < 0)
		{
			byteCount = ((-1)*src[i]) + 1;
			i++;
			
			for(j = currentDest;j < currentDest + byteCount;j++)
			{
				if (j >= numDestBytes)
				{
					return numDestBytes;
				}
				
				dest[j] = (unsigned char)src[i];
			}
			
			currentDest += byteCount;
		}
		else
		{
			byteCount = src[i] + 1;
			i++;
			
			for(j = currentDest;j < currentDest + byteCount;j++)
			{
				if (j >= numDestBytes)
				{
//					printf("Error at line 52\n");
					return numDestBytes;
				}
				
				dest[j] = (unsigned char)src[i];
				i++;
			}
			
			currentDest += byteCount;
			
			i--;
		}
	}
	// never get here
	// return currentDest;
}

int readEnemy(EnemyPtr enemy, PACKFILE *file)
{
//	VERIFYFREAD(*enemy, file)
	
	VERIFYFREAD(enemy->previousEnemy, file)
	VERIFYFREAD(enemy->nextEnemy, file)
	VERIFYFREAD(enemy->where.top, file)
	VERIFYFREAD(enemy->where.left, file)
	VERIFYFREAD(enemy->where.bottom, file)
	VERIFYFREAD(enemy->where.right, file)
	VERIFYFREAD(enemy->legCounter, file)
	VERIFYFREAD(enemy->legState, file)
	VERIFYFREAD(enemy->health, file)
	VERIFYFREAD(enemy->armorValue, file)
	VERIFYFREAD(enemy->damage, file)
	VERIFYFREAD(enemy->xp, file)
	VERIFYFREAD(enemy->attributes, file)
	VERIFYFREAD(enemy->immunities, file)
	VERIFYFREAD(enemy->damageType, file)
	VERIFYFREAD(enemy->speed, file)
	VERIFYFREAD(enemy->movePhase, file)
	VERIFYFREAD(enemy->gaurdianRange, file)
	VERIFYFREAD(enemy->facing, file)
	VERIFYFREAD(enemy->rateOfFire, file)
	VERIFYFREAD(enemy->originalNumber, file)
	VERIFYFREAD(enemy->spriteRef, file)
	VERIFYFREAD(enemy->movementType, file)
	VERIFYFREAD(enemy->deadItem, file)
	VERIFYFREAD(enemy->target, file)
	VERIFYFREAD(enemy->originalPosition.v, file)
	VERIFYFREAD(enemy->originalPosition.h, file)
	VERIFYFREAD(enemy->firedEnemy, file)
	VERIFYFREAD(enemy->pushableSpeed, file)
	VERIFYFREAD(enemy->disFromUnitCircle, file)
	VERIFYFREAD(enemy->angledCourse.v, file)
	VERIFYFREAD(enemy->angledCourse.h, file)
	VERIFYFREAD(enemy->messageID, file)
	VERIFYFREAD(enemy->stuckCounter, file)
	VERIFYFREAD(enemy->theta, file)
	VERIFYFREAD(enemy->expansion2, file)
	
#ifdef ALLEGRO_LITTLE_ENDIAN
	BITSWAP(enemy->previousEnemy)
	BITSWAP(enemy->nextEnemy)
	BITSWAP(enemy->where.top)
	BITSWAP(enemy->where.left)
	BITSWAP(enemy->where.bottom)
	BITSWAP(enemy->where.right)
	BITSWAP(enemy->legCounter)
	BITSWAP(enemy->legState)
	BITSWAP(enemy->health)
	BITSWAP(enemy->armorValue)
	BITSWAP(enemy->damage)
	BITSWAP(enemy->xp)
	BITSWAP(enemy->attributes)
	BITSWAP(enemy->immunities)
	BITSWAP(enemy->damageType)
	BITSWAP(enemy->speed)
	BITSWAP(enemy->movePhase)
	BITSWAP(enemy->gaurdianRange)
	BITSWAP(enemy->facing)
	BITSWAP(enemy->rateOfFire)
	BITSWAP(enemy->originalNumber)
	BITSWAP(enemy->spriteRef)
	BITSWAP(enemy->movementType)
	BITSWAP(enemy->deadItem)
	BITSWAP(enemy->target)
	BITSWAP(enemy->originalPosition.v)
	BITSWAP(enemy->originalPosition.h)
	BITSWAP(enemy->firedEnemy)
	BITSWAP(enemy->pushableSpeed)
	BITSWAP(enemy->disFromUnitCircle)
	BITSWAP(enemy->angledCourse.v)
	BITSWAP(enemy->angledCourse.h)
	BITSWAP(enemy->messageID)
	BITSWAP(enemy->stuckCounter)
	BITSWAP(enemy->theta)
	BITSWAP(enemy->expansion2)
#endif
	
	return 1;
}

int readMapItem(MapItem *item, PACKFILE *file)
{
	char garbage;
	
//	VERIFYFREAD(*item, file)
	
	VERIFYFREAD(item->modifiers, file)
	VERIFYFREAD(garbage, file)
	VERIFYFREAD(item->special, file)
	VERIFYFREAD(item->spriteRef, file)
	VERIFYFREAD(item->expansion, file)
	
#ifdef ALLEGRO_LITTLE_ENDIAN
//	BITSWAP(item)
	BITSWAP(item->modifiers)
	BITSWAP(item->special)
	BITSWAP(item->spriteRef)
	BITSWAP(item->expansion)
#endif
	
	return 1;
}

int readItem(Item *item, PACKFILE *file)
{
	char garbage;
	
//	VERIFYFREAD(*item, file)
	
	VERIFYFREAD(item->name, file)
	VERIFYFREAD(item->description, file)
	VERIFYFREAD(item->attributes, file)
	VERIFYFREAD(item->armor, file)
	VERIFYFREAD(item->damage, file)
	VERIFYFREAD(item->speed, file)
	VERIFYFREAD(item->rateOfFire, file)
	VERIFYFREAD(item->fireCounter, file)
	VERIFYFREAD(garbage, file)
	VERIFYFREAD(item->charges, file)
	VERIFYFREAD(item->stamina, file)
	VERIFYFREAD(item->damageHealed, file)
	VERIFYFREAD(item->quantity, file)
	VERIFYFREAD(item->spriteRef, file)
	VERIFYFREAD(item->firedMonsterID, file)
	VERIFYFREAD(item->immunities, file)
	VERIFYFREAD(item->damageType, file)
	
	item->name = item->description = NULL;
	
#ifdef ALLEGRO_LITTLE_ENDIAN
	BITSWAP(item->name)
	BITSWAP(item->description)
	BITSWAP(item->attributes)
	BITSWAP(item->armor)
	BITSWAP(item->damage)
	BITSWAP(item->speed)
	BITSWAP(item->rateOfFire)
	BITSWAP(item->fireCounter)
	BITSWAP(item->charges)
	BITSWAP(item->stamina)
	BITSWAP(item->damageHealed)
	BITSWAP(item->quantity)
	BITSWAP(item->spriteRef)
	BITSWAP(item->firedMonsterID)
	BITSWAP(item->immunities)
	BITSWAP(item->damageType)
#endif
	
	return 1;
}

int readSaric(Saric *saric, PACKFILE *file)
{
	char garbage;
	int i;
	
	VERIFYFREAD(saric->where.top, file)
	VERIFYFREAD(saric->where.left, file)
	VERIFYFREAD(saric->where.bottom, file)
	VERIFYFREAD(saric->where.right, file)
	VERIFYFREAD(saric->legCounter, file)
	VERIFYFREAD(garbage, file)
	VERIFYFREAD(saric->health, file)
	VERIFYFREAD(saric->maxHealth, file)
	VERIFYFREAD(saric->armorValue, file)
	VERIFYFREAD(saric->damage, file)
	VERIFYFREAD(saric->legState, file)
	VERIFYFREAD(garbage, file)
	VERIFYFREAD(saric->spriteRef, file)
	VERIFYFREAD(saric->oldPosition.v, file)
	VERIFYFREAD(saric->oldPosition.h, file)
	VERIFYFREAD(saric->facing, file)
	VERIFYFREAD(saric->speed, file)
	VERIFYFREAD(saric->oldSword.v, file)
	VERIFYFREAD(saric->oldSword.h, file)
	VERIFYFREAD(saric->swordOut, file)
	VERIFYFREAD(saric->wasSwordOut, file)
	VERIFYFREAD(saric->logicalWasSwordOut, file)
	VERIFYFREAD(saric->offHandOut, file)
	VERIFYFREAD(saric->logicalOffHandWasOut, file)
	VERIFYFREAD(saric->hadHitEnemy, file)
	VERIFYFREAD(saric->experience, file)
	VERIFYFREAD(saric->nextLevel, file)
	VERIFYFREAD(saric->level, file)
	VERIFYFREAD(saric->woundCounter, file)
	VERIFYFREAD(saric->sitCounter, file)
	VERIFYFREAD(saric->runCounter, file)
	VERIFYFREAD(saric->incrementalDamageCounter, file)
	VERIFYFREAD(saric->wasOnDoor, file)
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		VERIFYFREAD(saric->itemQuantities[i], file)
	}
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		VERIFYFREAD(saric->itemEquipped[i], file)
	}
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		VERIFYFREAD(saric->itemCharges[i], file)
	}
	/*
	if (!readItem(&saric->itemEffects[0], file))
	{
		return 0;
	}
	
	if (!readItem(&saric->itemEffects[1], file))
	{
		return 0;
	}
	
	if (!readItem(&saric->itemEffects[2], file))
	{
		return 0;
	}
	*/
	VERIFYFREAD(saric->itemEffects[0], file);
	VERIFYFREAD(saric->itemEffects[1], file);
	VERIFYFREAD(saric->itemEffects[2], file);
	
	VERIFYFREAD(saric->messageCounter, file)
	VERIFYFREAD(saric->stamina, file)
	VERIFYFREAD(saric->maxStamina, file)
	VERIFYFREAD(saric->money, file)
	VERIFYFREAD(saric->immunities, file)
	VERIFYFREAD(saric->damageType, file)
	
#ifdef ALLEGRO_LITTLE_ENDIAN
	BITSWAP(saric->where.top)
	BITSWAP(saric->where.left)
	BITSWAP(saric->where.bottom)
	BITSWAP(saric->where.right)
	BITSWAP(saric->legCounter)
	BITSWAP(saric->health)
	BITSWAP(saric->maxHealth)
	BITSWAP(saric->armorValue)
	BITSWAP(saric->damage)
	BITSWAP(saric->legState)
	BITSWAP(saric->spriteRef)
	BITSWAP(saric->oldPosition.v)
	BITSWAP(saric->oldPosition.h)
	BITSWAP(saric->facing)
	BITSWAP(saric->speed)
	BITSWAP(saric->oldSword.v)
	BITSWAP(saric->oldSword.h)
	BITSWAP(saric->swordOut)
	BITSWAP(saric->wasSwordOut)
	BITSWAP(saric->logicalWasSwordOut)
	BITSWAP(saric->offHandOut)
	BITSWAP(saric->logicalOffHandWasOut)
	BITSWAP(saric->hadHitEnemy)
	BITSWAP(saric->experience)
	BITSWAP(saric->nextLevel)
	BITSWAP(saric->level)
	BITSWAP(saric->woundCounter)
	BITSWAP(saric->sitCounter)
	BITSWAP(saric->runCounter)
	BITSWAP(saric->incrementalDamageCounter)
	BITSWAP(saric->wasOnDoor)
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		BITSWAP(saric->itemQuantities[i])
		BITSWAP(saric->itemEquipped[i])
		BITSWAP(saric->itemCharges[i])
	}
	
	for(i = 0;i < 2;i++)
	{
		BITSWAP(saric->itemEffects[i].name)
		BITSWAP(saric->itemEffects[i].description)
		BITSWAP(saric->itemEffects[i].attributes)
		BITSWAP(saric->itemEffects[i].armor)
		BITSWAP(saric->itemEffects[i].damage)
		BITSWAP(saric->itemEffects[i].speed)
		BITSWAP(saric->itemEffects[i].rateOfFire)
		BITSWAP(saric->itemEffects[i].fireCounter)
		BITSWAP(saric->itemEffects[i].charges)
		BITSWAP(saric->itemEffects[i].stamina)
		BITSWAP(saric->itemEffects[i].damageHealed)
		BITSWAP(saric->itemEffects[i].quantity)
		BITSWAP(saric->itemEffects[i].spriteRef)
		BITSWAP(saric->itemEffects[i].firedMonsterID)
		BITSWAP(saric->itemEffects[i].immunities)
		BITSWAP(saric->itemEffects[i].damageType)
	}
	
	BITSWAP(saric->messageCounter)
	BITSWAP(saric->stamina)
	BITSWAP(saric->maxStamina)
	BITSWAP(saric->money)
	BITSWAP(saric->immunities)
	BITSWAP(saric->damageType)
#endif
	
	printf("OverallSize: %X\n", overallsize);
	
	return 1;
}

int writeSaric(Saric *thesaric, PACKFILE *file)
{
	Saric thesariccopy = *thesaric;
	Saric *saric = &thesariccopy;
	char garbage;
	int i;
	
#ifdef ALLEGRO_LITTLE_ENDIAN
	BITSWAP(saric->where.top)
	BITSWAP(saric->where.left)
	BITSWAP(saric->where.bottom)
	BITSWAP(saric->where.right)
	BITSWAP(saric->legCounter)
	BITSWAP(saric->health)
	BITSWAP(saric->maxHealth)
	BITSWAP(saric->armorValue)
	BITSWAP(saric->damage)
	BITSWAP(saric->legState)
	BITSWAP(saric->spriteRef)
	BITSWAP(saric->oldPosition.v)
	BITSWAP(saric->oldPosition.h)
	BITSWAP(saric->facing)
	BITSWAP(saric->speed)
	BITSWAP(saric->oldSword.v)
	BITSWAP(saric->oldSword.h)
	BITSWAP(saric->swordOut)
	BITSWAP(saric->wasSwordOut)
	BITSWAP(saric->logicalWasSwordOut)
	BITSWAP(saric->offHandOut)
	BITSWAP(saric->logicalOffHandWasOut)
	BITSWAP(saric->hadHitEnemy)
	BITSWAP(saric->experience)
	BITSWAP(saric->nextLevel)
	BITSWAP(saric->level)
	BITSWAP(saric->woundCounter)
	BITSWAP(saric->sitCounter)
	BITSWAP(saric->runCounter)
	BITSWAP(saric->incrementalDamageCounter)
	BITSWAP(saric->wasOnDoor)
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		BITSWAP(saric->itemQuantities[i])
		BITSWAP(saric->itemEquipped[i])
		BITSWAP(saric->itemCharges[i])
	}
	
	for(i = 0;i < 2;i++)
	{
		BITSWAP(saric->itemEffects[i].name)
		BITSWAP(saric->itemEffects[i].description)
		BITSWAP(saric->itemEffects[i].attributes)
		BITSWAP(saric->itemEffects[i].armor)
		BITSWAP(saric->itemEffects[i].damage)
		BITSWAP(saric->itemEffects[i].speed)
		BITSWAP(saric->itemEffects[i].rateOfFire)
		BITSWAP(saric->itemEffects[i].fireCounter)
		BITSWAP(saric->itemEffects[i].charges)
		BITSWAP(saric->itemEffects[i].stamina)
		BITSWAP(saric->itemEffects[i].damageHealed)
		BITSWAP(saric->itemEffects[i].quantity)
		BITSWAP(saric->itemEffects[i].spriteRef)
		BITSWAP(saric->itemEffects[i].firedMonsterID)
		BITSWAP(saric->itemEffects[i].immunities)
		BITSWAP(saric->itemEffects[i].damageType)
	}
	
	BITSWAP(saric->messageCounter)
	BITSWAP(saric->stamina)
	BITSWAP(saric->maxStamina)
	BITSWAP(saric->money)
	BITSWAP(saric->immunities)
	BITSWAP(saric->damageType)
#endif
	
	VERIFYFWRITE(saric->where.top, file)
	VERIFYFWRITE(saric->where.left, file)
	VERIFYFWRITE(saric->where.bottom, file)
	VERIFYFWRITE(saric->where.right, file)
	VERIFYFWRITE(saric->legCounter, file)
	VERIFYFWRITE(garbage, file)
	VERIFYFWRITE(saric->health, file)
	VERIFYFWRITE(saric->maxHealth, file)
	VERIFYFWRITE(saric->armorValue, file)
	VERIFYFWRITE(saric->damage, file)
	VERIFYFWRITE(saric->legState, file)
	VERIFYFWRITE(garbage, file)
	VERIFYFWRITE(saric->spriteRef, file)
	VERIFYFWRITE(saric->oldPosition.v, file)
	VERIFYFWRITE(saric->oldPosition.h, file)
	VERIFYFWRITE(saric->facing, file)
	VERIFYFWRITE(saric->speed, file)
	VERIFYFWRITE(saric->oldSword.v, file)
	VERIFYFWRITE(saric->oldSword.h, file)
	VERIFYFWRITE(saric->swordOut, file)
	VERIFYFWRITE(saric->wasSwordOut, file)
	VERIFYFWRITE(saric->logicalWasSwordOut, file)
	VERIFYFWRITE(saric->offHandOut, file)
	VERIFYFWRITE(saric->logicalOffHandWasOut, file)
	VERIFYFWRITE(saric->hadHitEnemy, file)
	VERIFYFWRITE(saric->experience, file)
	VERIFYFWRITE(saric->nextLevel, file)
	VERIFYFWRITE(saric->level, file)
	VERIFYFWRITE(saric->woundCounter, file)
	VERIFYFWRITE(saric->sitCounter, file)
	VERIFYFWRITE(saric->runCounter, file)
	VERIFYFWRITE(saric->incrementalDamageCounter, file)
	VERIFYFWRITE(saric->wasOnDoor, file)
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		VERIFYFWRITE(saric->itemQuantities[i], file)
	}
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		VERIFYFWRITE(saric->itemEquipped[i], file)
	}
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		VERIFYFWRITE(saric->itemCharges[i], file)
	}
	
	VERIFYFWRITE(saric->itemEffects[0], file);
	VERIFYFWRITE(saric->itemEffects[1], file);
	VERIFYFWRITE(saric->itemEffects[2], file);
	
	VERIFYFWRITE(saric->messageCounter, file)
	VERIFYFWRITE(saric->stamina, file)
	VERIFYFWRITE(saric->maxStamina, file)
	VERIFYFWRITE(saric->money, file)
	VERIFYFWRITE(saric->immunities, file)
	VERIFYFWRITE(saric->damageType, file)
	
	
	printf("OverallSize: %X\n", overallsize);
	
	return 1;
}

int readSavedGame(SavedGame *game, PACKFILE *file)
{
#ifdef ALLEGRO_LITTLE_ENDIAN
	int i;
#endif
	
	VERIFYFREAD(game->name, file)
	VERIFYFREAD(game->time, file)
	
	if (!readSaric(&game->saric, file))
	{
		return 0;
	}
	
	VERIFYFREAD(game->mapScreen.v, file)
	VERIFYFREAD(game->mapScreen.h, file)
	VERIFYFREAD(game->deathRecord, file)
	
#ifdef ALLEGRO_LITTLE_ENDIAN
	BITSWAP(game->time)
	BITSWAP(game->mapScreen.v)
	BITSWAP(game->mapScreen.h)
	
	for(i = 0;i < NUM_SCREENS;i++)
	{
		BITSWAP(g_DeathRecord[i])
	}
#endif
	
	return 1;
}

int writeSavedGame(SavedGame *thegame, PACKFILE *file)
{
	SavedGame thegamecopy = *thegame;
	SavedGame *game = &thegamecopy;
	
#ifdef ALLEGRO_LITTLE_ENDIAN
	int i;
	
	BITSWAP(game->time)
	BITSWAP(game->mapScreen.v)
	BITSWAP(game->mapScreen.h)
	
	for(i = 0;i < NUM_SCREENS;i++)
	{
		BITSWAP(g_DeathRecord[i])
	}
#endif
	
	VERIFYFWRITE(game->name, file)
	VERIFYFWRITE(game->time, file)
	
	if (!writeSaric(&game->saric, file))
	{
		return 0;
	}
	
	VERIFYFWRITE(game->mapScreen.v, file)
	VERIFYFWRITE(game->mapScreen.h, file)
	VERIFYFWRITE(game->deathRecord, file)
	
	return 1;
}

void newGame(void)
{
	showBitmapCentered(g_StoryBitmap);
	
	fadeFromBlack();
	
	waitForKeyPressed();
	
	fadeToBlack();
	
	initMapStuff();
	initSaric();
	
	g_CurrentScreenX = 12;
	g_CurrentScreenY = 7;
	
	loadScreen((g_CurrentScreenY * 16) + g_CurrentScreenX);
	/*
	if (!createSavedGameData())
	{
		printf("Unable to create game data!\n");
	}
	*/
}

int openGame(char *filename)
{
	PACKFILE *file;
	Point point;
	
	int i;
	
	packfile_password(NULL);
	file = pack_fopen(filename, "r");
	
	if (!file)
	{
		return 0;
	}
	
	overallsize = 0;
	
	if (!readSaric(&g_Saric, file))
	{
		pack_fclose(file);
		
		return 0;
	}
	
	if (pack_fread(&point, sizeof(Point), file) < ((signed)sizeof(Point)))
	{
		pack_fclose(file);
		
		return 0;
	}
	
//	overallsize += sizeof(Point);
	
#ifdef ALLEGRO_LITTLE_ENDIAN
	BITSWAP(point.v)
	BITSWAP(point.h)
#endif
	
	if (pack_fread(g_DeathRecord, 2 * NUM_SCREENS, file) < 2 * NUM_SCREENS)
	{
		printf("Almost made it through file!\n");
		pack_fclose(file);
		
		return 0;
	}
	
#ifdef ALLEGRO_LITTLE_ENDIAN
	for(i = 0;i < NUM_SCREENS;i++)
	{
		BITSWAP(g_DeathRecord[i])
	}
#endif
	
//	overallsize += 2 * NUM_SCREENS;
	
	if (pack_fread(&i, 1, file))
	{
		printf("Stuff left at end of file!\n");
	}
	
	pack_fclose(file);
	
	g_CurrentScreenX = point.h;
	g_CurrentScreenY = point.v;
	
	initMapStuff();
	
//	printf("Overall size read: %d\n", overallsize);
//	printf("Saric's size: %d\n", sizeof(Saric));
//	printf("Loaded screen: (%d, %d)\n", g_CurrentScreenX, g_CurrentScreenY);
	
	loadScreen((g_CurrentScreenY * 16) + g_CurrentScreenX);
	
	playMusic(g_MapRegions[g_CurrentScreen].musicIndex);
	
	return 1;
}

/*
 struct DataFileItem{
	 Str255	name;
	 Str255	description;
	 long	attributes;
	 char	armor;
	 char	damage;
	 char	speed;
	 unsigned char	rateOfFire;
	 unsigned char	fireCounter;
	 short	charges;
	 char	stamina;
	 char	damageHealed;
	 short	quantity;
	 short	spriteRef;
	 short	firedMonsterID;
	 short	immunities;
	 short	damageType;
 };
*/
int readDataFileItem(DataFileItem *item, PACKFILE *file)
{
	/*
	VERIFYFREAD(item->name, file)
	VERIFYFREAD(item->description, file)
	VERIFYFREAD(item->attributes, file)
	VERIFYFREAD(item->armor, file)
	VERIFYFREAD(item->damage, file)
	VERIFYFREAD(item->speed, file)
	VERIFYFREAD(item->rateOfFire, file)
	VERIFYFREAD(item->fireCounter, file)
	VERIFYFREAD(item->charges, file)
	VERIFYFREAD(item->stamina, file)
	VERIFYFREAD(item->damageHealed, file)
	VERIFYFREAD(item->quantity, file)
	VERIFYFREAD(item->spriteRef, file)
	VERIFYFREAD(item->firedMonsterID, file)
	VERIFYFREAD(item->immunities, file)
	VERIFYFREAD(item->damageType, file)
	*/
	VERIFYFREAD((*item), file)
	
#ifdef ALLEGRO_LITTLE_ENDIAN
//	BITSWAP(item->name)
//	BITSWAP(item->description)
	BITSWAP(item->attributes)
	BITSWAP(item->armor)
	BITSWAP(item->damage)
	BITSWAP(item->speed)
	BITSWAP(item->rateOfFire)
	BITSWAP(item->fireCounter)
	BITSWAP(item->charges)
	BITSWAP(item->stamina)
	BITSWAP(item->damageHealed)
	BITSWAP(item->quantity)
	BITSWAP(item->spriteRef)
	BITSWAP(item->firedMonsterID)
	BITSWAP(item->immunities)
	BITSWAP(item->damageType)
#endif
	
	return 1;
}

void writeDatafile(DATAFILE *d)
{
	const char *name = get_datafile_property(d, DAT_ID('N','A','M','E'));
	PACKFILE *file;
	DATAFILE *temp;
	
//	printf("Name: %s\n", name);
	
	temp = find_datafile_object(d, name);
	
	if (!temp)
	{
		return;
	}
	
	packfile_password(NULL);
	file = pack_fopen(name, "w");
	
	if (!file)
	{
		return;
	}
	
	pack_fwrite(temp->dat, temp->size, file);
	
	pack_fclose(file);
	
//	printf("Written!\n");
}

void deleteDatafile(DATAFILE *d)
{
	const char *name = get_datafile_property(d, DAT_ID('N','A','M','E'));
	
	if (delete_file(name))
	{
		printf("Unable to delete: %s\n", name);
	}
}

void createTempFiles(void)
{
	DATAFILE *datafile;
	
	packfile_password("musicman3320");
	datafile = load_datafile_callback("GameData.dat", writeDatafile);
	
	unload_datafile(datafile);
}

void deleteTempFiles(void)
{
	DATAFILE *datafile;
	
	packfile_password("musicman3320");
	datafile = load_datafile_callback("GameData.dat", deleteDatafile);
	
	unload_datafile(datafile);
}

int readItemData(void)
{
	PACKFILE *file;
	IconHeader header;
	DataFileItem tempItem;
	int amountread;
	
	int i, len;
	
//	printf("Size of DataFileItem: %d\n", sizeof(DataFileItem));
	
	// initialize items first
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		g_ItemTemplates[i].name = NULL;
		g_ItemTemplates[i].description = NULL;
		g_ItemTemplates[i].name = 0;
		g_ItemTemplates[i].description = 0;
		g_ItemTemplates[i].attributes = 0;
		g_ItemTemplates[i].armor = 0;
		g_ItemTemplates[i].damage = 0;
		g_ItemTemplates[i].speed = 0;
		g_ItemTemplates[i].rateOfFire = 0;
		g_ItemTemplates[i].fireCounter = 0;
		g_ItemTemplates[i].charges = 0;
		g_ItemTemplates[i].stamina = 0;
		g_ItemTemplates[i].damageHealed = 0;
		g_ItemTemplates[i].quantity = 0;
		g_ItemTemplates[i].spriteRef = 0;
		g_ItemTemplates[i].firedMonsterID = 0;
		g_ItemTemplates[i].immunities = 0;
		g_ItemTemplates[i].damageType = 0;
	}
	
	packfile_password("musicman3320");
	file = pack_fopen("GameData.dat#ItemData", "r");
	
	if (!file)
	{
		printf("Unable to open ItemData\n");
		return 0;
	}
	
	for(;;)
	{
		header.id = 0;
		header.len = 0;
		
		amountread = pack_fread(&header.id, sizeof(short), file);
		
		if (amountread < ((signed)sizeof(short)))
		{
			printf("Stopped in the middle of a header\n");
			break;
		}
		
		amountread = pack_fread(&header.len, sizeof(long), file);
		
		if (amountread < (signed)sizeof(long))
		{
			printf("Stopped in the middle of a header\n");
			break;
		}
		
#ifdef ALLEGRO_LITTLE_ENDIAN
		// fix endian problem (need a better solution, but works for now)
		bitswap(&header.id, sizeof(header.id));
		bitswap(&header.len, sizeof(header.len));
#endif
		
//		printf("id: %d, len: %d\n", header.id, header.len);
		
		// were done
		if (header.id == -1)
		{
//			printf("End of resources reached!\n");
			break;
		}
		// data file item
		else if (header.id >= 16000 && header.id < 16000 + NUM_ITEM_TEMPLATES)
		{
			if (!readDataFileItem(&tempItem, file))
			{
				printf("Error reading Item\n");
				return 0;
			}
			
//			printf("Sprite ref: %d\n", tempItem.spriteRef);
			
			g_ItemTemplates[header.id - 16000].name = (StringPtr)malloc(sizeof(Str255));
			memset(g_ItemTemplates[header.id - 16000].name, 0, sizeof(Str255));
			
			len = tempItem.name[0];
			
			for(i = 1;i <= len;i++)
			{
				/*
				if (!IS_VALID_ASCII_LETTER(tempItem.name[i]))
				{
					g_ItemTemplates[header.id - 16000].name[i - 1] = 0;
					break;
				}
				else if ((header.id - 16000 == 6 && i == 13) || 
					 (header.id - 16000 == 11 && i == 9) || 
					 (header.id - 16000 == 200 && i == 13))
				{
					g_ItemTemplates[header.id - 16000].name[i - 1] = 0;
					break;
				}
				else
				{
				*/
					g_ItemTemplates[header.id - 16000].name[i - 1] = tempItem.name[i];
			//	}
			}
			
			g_ItemTemplates[header.id - 16000].description = (StringPtr)malloc(sizeof(Str255));
			memset(g_ItemTemplates[header.id - 16000].description, 0, sizeof(Str255));
			
			len = tempItem.description[0];
			
			for(i = 1;i <= len;i++)
			{
				g_ItemTemplates[header.id - 16000].description[i - 1] = tempItem.description[i];
			}
			
//			BlockMove((Ptr)tempItem.name, (Ptr)g_ItemTemplates[i].name, tempItem.name[0]+1);
//			BlockMove((Ptr)tempItem.description, (Ptr)g_ItemTemplates[i].description, tempItem.description[0]+1);
			g_ItemTemplates[header.id - 16000].attributes = tempItem.attributes;
			g_ItemTemplates[header.id - 16000].armor = tempItem.armor;
			g_ItemTemplates[header.id - 16000].damage = tempItem.damage;
			g_ItemTemplates[header.id - 16000].speed = tempItem.speed;
			g_ItemTemplates[header.id - 16000].rateOfFire = tempItem.rateOfFire;
			g_ItemTemplates[header.id - 16000].fireCounter = tempItem.fireCounter;
			g_ItemTemplates[header.id - 16000].charges = tempItem.charges;
			g_ItemTemplates[header.id - 16000].stamina = tempItem.stamina;
			g_ItemTemplates[header.id - 16000].damageHealed = tempItem.damageHealed;
			g_ItemTemplates[header.id - 16000].quantity = tempItem.quantity;
			g_ItemTemplates[header.id - 16000].spriteRef = tempItem.spriteRef;
			g_ItemTemplates[header.id - 16000].firedMonsterID = tempItem.firedMonsterID;
			g_ItemTemplates[header.id - 16000].immunities = tempItem.immunities;
			g_ItemTemplates[header.id - 16000].damageType = tempItem.damageType;
		}
		// unknown header id
		else
		{
			printf("Unknown id reached: %d\n", header.id);
			for (i = 0;i < header.len;i++) {
				pack_getc(file);
			}
		}
	}
	
	pack_fclose(file);
	
	printf("Finished loading item data from file!\n");
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		if (g_ItemTemplates[i].name)
		{
			printf("Item Number: %d, Name: %s, Desc: %s, Len: %d, Attributes: %X\n", i, g_ItemTemplates[i].name, g_ItemTemplates[i].description, (int)strlen(g_ItemTemplates[i].name), (int)g_ItemTemplates[i].attributes);
		}
	}
	
	return 1;
}

int readMapData(void)
{
	PACKFILE *file;
	int amountread;
	int i, j;
	
	packfile_password("musicman3320");
	file = pack_fopen("GameData.dat#MapArea", "r");
	
	if (!file)
	{
		return 0;
	}
	
	for(i = 0;i < NUM_SCREENS;i++)
	{
		amountread = pack_fread(&g_MapRegions[i], sizeof(RegionCell), file);
		
		if (amountread < (signed)sizeof(RegionCell))
		{
			pack_fclose(file);
			return 0;
		}
	}
	
	amountread = pack_fread(&j, 1, file);
	
	if (amountread > 0)
	{
		printf("MapArea longer than expected\n");
	}
	
	pack_fclose(file);
	
	printf("Finished loading map area from file!\n");
	
	packfile_password("musicman3320");
	file = pack_fopen("GameData.dat#MapData", "r");
	
	if (!file)
	{
		return 0;
	}
	
	for(i = 0;i < NUM_SCREENS;i++)
	{
		for (j = 0;j < 160;j++) 
		{
			if (!readMapItem(&g_MapScreens[i].tiles[j], file))
			{
				pack_fclose(file);
				return 0;
			}
		}
		
		for(j = 0;j < 16;j++)
		{
			if (!readEnemy(&g_MapScreens[i].enemies[j], file))
			{
				pack_fclose(file);
				return 0;
			}
		}
	}
	
	amountread = pack_fread(&j, 1, file);
	
	if (amountread > 0)
	{
		printf("MapData file longer than expected\n");
	}
	
	pack_fclose(file);
	
	printf("Finished loading map data from file!\n");
	
	return 1;
}

int readTmplEnemyData(void)
{
	PACKFILE *file;
	IconHeader header;
//	DataFileItem tempItem;
	int amountread;
	
	int i;
	
	packfile_password("musicman3320");
	file = pack_fopen("GameData.dat#TmplData", "r");
	
	if (!file)
	{
		printf("Unable to open ItemData\n");
		return 0;
	}
	
	for(;;)
	{
		header.id = 0;
		header.len = 0;
		
		amountread = pack_fread(&header.id, sizeof(short), file);
		
		if (amountread < (signed)sizeof(short))
		{
			printf("Stopped in the middle of a header\n");
			break;
		}
		
		amountread = pack_fread(&header.len, sizeof(long), file);
		
		if (amountread < (signed)sizeof(long))
		{
			printf("Stopped in the middle of a header\n");
			break;
		}
		
#ifdef ALLEGRO_LITTLE_ENDIAN
		// fix endian problem (need a better solution, but works for now)
		bitswap(&header.id, sizeof(header.id));
		bitswap(&header.len, sizeof(header.len));
#endif
		
//		printf("id: %d, len: %d\n", header.id, header.len);
		
		// were done
		if (header.id == -1)
		{
//			printf("End of resources reached!\n");
			break;
		}
		// tmpl enemy (I think these are all enemies, and
		// they will be treated as so until I learn otherwise...
		else if (header.id >= 2000 && header.id < 2000 + NUM_TMPL_ENEMIES)
		{
			if (!readEnemy(&g_TmplEnemies[header.id - 2000], file))
			{
				printf("Error reading Item\n");
				return 0;
			}
			
//			printf("Sprite ref: %d\n", tempItem.spriteRef);
			
		}
		// unknown header id
		else
		{
			printf("Unknown id reached: %d\n", header.id);
			for (i = 0;i < header.len;i++) {
				pack_getc(file);
			}
		}
	}
	
	pack_fclose(file);
	
	printf("Finished loading tmpl enemy data from file!\n");
	
	return 1;
}

int readMessageData(void)
{
	PACKFILE *file;
	int i, j, tempchar;
	
	packfile_password("musicman3320");
	file = pack_fopen("GameData.dat#TextData", "rb");
	
	if (!file)
	{
		printf("Unable to open TextData!\n");
		return 0;
	}
	
	for(i = 0;i < NUM_MESSAGES;i++)
	{
		tempchar = pack_getc(file);
		
		for(j = 0;tempchar != EOF && tempchar != 0;j++, tempchar = pack_getc(file))
		{
			g_Messages[i][j] = tempchar - 126;
		}
		
		g_Messages[i][j] = 0;
	}
	
	pack_fclose(file);
	
	for(i = 0;i < NUM_MESSAGES;i++)
	{
		printf("Message %d: %s\n", i, g_Messages[i]);
	}
	
	return 1;
}

int readPalette(void)
{
	PACKFILE *paletteFile;
	int i;
	
	packfile_password("musicman3320");
	paletteFile = pack_fopen("GameData.dat#SystemPalette", "r");
	
	if (!paletteFile)
	{
		return 0;
	}
	
	for(i = 0;i < 256;i++)
	{
		g_SystemPalette[i].r = (unsigned char)pack_getc(paletteFile) / 4;
		g_SystemPalette[i].g = (unsigned char)pack_getc(paletteFile) / 4;
		g_SystemPalette[i].b = (unsigned char)pack_getc(paletteFile) / 4;
	}
	
	pack_fclose(paletteFile);
	
	set_palette(g_SystemPalette);
	
	g_BlackColor = makecol(0, 0, 0);
	g_RedColor = makecol(63, 0, 0);
	
	printf("Palette loaded from file!\n");	
	
	return 1;
}

int readGraphicsData(void)
{
	IconHeader header;
	unsigned char buff[1040];
	unsigned char *dest;
	
	unsigned char *tileIcons = NULL;
	unsigned char *tileMasks = NULL;
	unsigned char *saricIcons = NULL;
	unsigned char *saricMask = NULL;
	unsigned char *enemyIcons = NULL;
	unsigned char *swordIcons = NULL;
	unsigned char *swordMasks = NULL;
	unsigned char *bossIcons = NULL;
	
	unsigned char *smallIcons = NULL;
	unsigned char *largeIcons = NULL;
	
	PACKFILE *spriteData;
	PACKFILE *bossFile;
	int numUnpacked;
	int amountread;
	int i, j, k;
	
	// Read in Default Palette
	readPalette();
	
	// Read map graphics file
	packfile_password("musicman3320");
	spriteData = pack_fopen("GameData.dat#MapGraphics", "r");
	
	if (!spriteData)
	{
		printf("Unable to open file!\n");
		goto ERROR_BREAK;
	}
	
	tileIcons = (unsigned char *)malloc(1024*NUM_TILE_ICONS);
	tileMasks = (unsigned char *)malloc(1024*NUM_TILE_ICONS);
	
//	printf("Size of IconHeader: %d\n", sizeof(header));
	
	while(1)
	{
		header.id = 0;
		header.len = 0;
		
		amountread = pack_fread(&header.id, sizeof(short), spriteData);
		
		if (amountread < 1)
		{
			printf("Stopped in the middle of a header\n");
			break;
		}
		
		amountread = pack_fread(&header.len, sizeof(long), spriteData);
		
		if (amountread < 1)
		{
			printf("Stopped in the middle of a header\n");
			break;
		}
		
		
#ifdef ALLEGRO_LITTLE_ENDIAN
		// fix endian problem (need a better solution, but works for now)
		bitswap(&header.id, sizeof(header.id));
		bitswap(&header.len, sizeof(header.len));
#endif
		
//		printf("id: %d, len: %d\n", header.id, header.len);
		
		// were done
		if (header.id == -1)
		{
//			printf("End of resources reached!\n");
			break;
		}
		// tile icons
		else if (header.id >= 1000 && header.id < 1000 + NUM_TILE_ICONS)
		{
//			printf("About to do memset\n");
			memset(buff, 0, 1024);
//			printf("About to do fread\n");
			amountread = pack_fread(buff, header.len, spriteData);
			
//			printf("About to do if\n");
			if (amountread < header.len)
			{
				printf("Stopped in the middle of an icon resource\n");
				break;
			}
			
//			printf("About to assign dest\n");
			dest = tileIcons + (1024 * (header.id - 1000));
			
//			printf("About to unpack bits\n");
			numUnpacked = myUnpackBitsSrc((void *)buff, (void *)dest, header.len);
			
			// debug output
//			printf("Resource id: %d, length: %d, length unpacked: %d\n", header.id, header.len, numUnpacked);
		}
		// tile masks
		else if (header.id >= 3000 && header.id < 3000 + NUM_TILE_ICONS)
		{
			memset(buff, 0, 1024);
			amountread = pack_fread(buff, header.len, spriteData);
			
			if (amountread < header.len)
			{
				printf("Stopped in the middle of a mask resource\n");
				break;
			}
			
			dest = tileMasks + (1024 * (header.id - 3000));
			
			numUnpacked = myUnpackBitsSrc((void *)buff, (void *)dest, header.len);
			
			// debug output
//			printf("Resource id: %d, length: %d, length unpacked: %d\n", header.id, header.len, numUnpacked);
		}
		// unknown
		else
		{
//			printf("Unknown resource type reached: %d, len: %d\n", header.id, header.len);
			for(i = 0;i < header.len;i++) 
			{
				pack_getc(spriteData);
			}
		}
	}
	
	pack_fclose(spriteData);
	
	printf("Finished reading map graphics file!\n");
	
	// Read sprite file
	packfile_password("musicman3320");
	spriteData = pack_fopen("GameData.dat#GameSprites", "r");
	
	if (!spriteData)
	{
		printf("Unable to open file!\n");
		goto ERROR_BREAK;
	}
	
	saricIcons = (unsigned char *)malloc(1024*NUM_SARIC_ICONS);
	saricMask = (unsigned char *)malloc(1024*1);
	enemyIcons = (unsigned char *)malloc(1024*NUM_ENEMY_ICONS);
	swordIcons = (unsigned char *)malloc(1024*NUM_SWORD_ICONS);
	swordMasks = (unsigned char *)malloc(1024*NUM_SWORD_ICONS);
	
//	printf("Size of IconHeader: %d\n", sizeof(header));
	
	while(1)
	{
		header.id = 0;
		header.len = 0;
		
		amountread = pack_fread(&header.id, sizeof(short), spriteData);
		
		if (amountread < 1)
		{
			printf("Stopped in the middle of a header\n");
			break;
		}
		
		amountread = pack_fread(&header.len, sizeof(long), spriteData);
		
		if (amountread < 1)
		{
			printf("Stopped in the middle of a header\n");
			break;
		}
		
#ifdef ALLEGRO_LITTLE_ENDIAN
		// fix endian problem (need a better solution, but works for now)
		bitswap(&header.id, sizeof(header.id));
		bitswap(&header.len, sizeof(header.len));
#endif
		
//		printf("id: %d, len: %d\n", header.id, header.len);
		
		// were done
		if (header.id == -1)
		{
//			printf("End of resources reached!\n");
			break;
		}
		// enemy icons
		else if (header.id >= 2000 && header.id < 2000 + NUM_ENEMY_ICONS)
		{
//			printf("About to do memset\n");
			memset(buff, 0, 1024);
//			printf("About to do fread\n");
			amountread = pack_fread(buff, header.len, spriteData);
			
//			printf("About to do if\n");
			if (amountread < header.len)
			{
				printf("Stopped in the middle of an icon resource\n");
				break;
			}
			
//			printf("About to assign dest\n");
			dest = enemyIcons + (1024 * (header.id - 2000));
			
//			printf("About to unpack bits\n");
			numUnpacked = myUnpackBitsSrc((void *)buff, (void *)dest, header.len);
			
			// debug output
//			printf("Resource id: %d, length: %d, length unpacked: %d\n", header.id, header.len, numUnpacked);
		}
		// saric icons
		else if (header.id >= 1000 && header.id < 1000 + NUM_SARIC_ICONS)
		{
			memset(buff, 0, 1024);
			amountread = pack_fread(buff, header.len, spriteData);
			
			if (amountread < header.len)
			{
				printf("Stopped in the middle of a mask resource\n");
				break;
			}
			
			dest = saricIcons + (1024 * (header.id - 1000));
			
			numUnpacked = myUnpackBitsSrc((void *)buff, (void *)dest, header.len);
			
			// debug output
//			printf("Resource id: %d, length: %d, length unpacked: %d\n", header.id, header.len, numUnpacked);
		}
		// sword masks
		else if (header.id >= 995 && header.id < 995 + 4)
		{
			memset(buff, 0, 1024);
			amountread = pack_fread(buff, header.len, spriteData);
			
			if (amountread < header.len)
			{
				printf("Stopped in the middle of a mask resource\n");
				break;
			}
			
			dest = swordMasks + (1024 * (header.id - 995));
			
			numUnpacked = myUnpackBitsSrc((void *)buff, (void *)dest, header.len);
			
			// debug output
//			printf("Resource id: %d, length: %d, length unpacked: %d\n", header.id, header.len, numUnpacked);
		}
		// sword icons
		else if (header.id >= 1500 && header.id < 1500 + 4)
		{
			memset(buff, 0, 1024);
			amountread = pack_fread(buff, header.len, spriteData);
			
			if (amountread < header.len)
			{
				printf("Stopped in the middle of a mask resource\n");
				break;
			}
			
			dest = swordIcons + (1024 * (header.id - 1500));
			
			numUnpacked = myUnpackBitsSrc((void *)buff, (void *)dest, header.len);
			
			// debug output
//			printf("Resource id: %d, length: %d, length unpacked: %d\n", header.id, header.len, numUnpacked);
		}
		// saric mask
		else if (header.id == 999)
		{
			memset(buff, 0, 1024);
			amountread = pack_fread(buff, header.len, spriteData);
			
			if (amountread < header.len)
			{
				printf("Stopped in the middle of a mask resource\n");
				break;
			}
			
			dest = saricMask;
			
			numUnpacked = myUnpackBitsSrc((void *)buff, (void *)dest, header.len);
			
			// debug output
//			printf("Resource id: %d, length: %d, length unpacked: %d\n", header.id, header.len, numUnpacked);
		}
		// unknown
		else
		{
//			printf("Unknown resource type reached: %d, len: %d\n", header.id, header.len);
			for(i = 0;i < header.len;i++) 
			{
				pack_getc(spriteData);
			}
		}
	}
	
	pack_fclose(spriteData);
	
	printf("Finished reading sprite graphics file!\n");
	
	// read bosses
	bossIcons = (unsigned char *)malloc(4096 * NUM_BOSS_ICONS * sizeof(unsigned char));
	
	packfile_password("musicman3320");
	bossFile = pack_fopen("GameData.dat#BossData", "r");
	
	if (!bossFile)
	{
		printf("Error opening boss file!\n");
		goto ERROR_BREAK;
	}
	
	amountread = pack_fread(bossIcons, 4096 * NUM_BOSS_ICONS, bossFile);
	
	if (amountread < 4096 * NUM_BOSS_ICONS)
	{
		printf("Boss file too small!\n");
		goto ERROR_BREAK;
	}
	
	amountread = pack_fread(&i, 1, bossFile);
	
	if (amountread > 0)
	{
		printf("Boss file too big!\n");
		goto ERROR_BREAK;
	}
	
	pack_fclose(bossFile);
	
	printf("Read the bosses successfully!\n");
	
	// Read map graphics file
	packfile_password("musicman3320");
	spriteData = pack_fopen("GameData.dat#IconData", "r");
	
	if (!spriteData)
	{
		printf("Unable to open file!\n");
		goto ERROR_BREAK;
	}
	
	smallIcons = (unsigned char *)malloc(256);
	largeIcons = (unsigned char *)malloc(1024);
	
	memset(smallIcons, 0, 256);
	memset(largeIcons, 0, 1024);
	
	memset(g_ImmunityIcons, 0, NUM_IMMUNITY_ICONS * sizeof(BITMAP *));
	memset(g_SmallIcons, 0, NUM_ITEM_TEMPLATES * sizeof(BITMAP *));
	memset(g_LargeIcons, 0, NUM_ITEM_TEMPLATES * sizeof(BITMAP *));
	
//	printf("Size of IconHeader: %d\n", sizeof(header));
	
	while(1)
	{
		header.id = 0;
		header.len = 0;
		
		amountread = pack_fread(&header.id, sizeof(short), spriteData);
		
		if (amountread < (signed)sizeof(short))
		{
			printf("Stopped in the middle of a header\n");
			break;
		}
		
		amountread = pack_fread(&header.len, sizeof(long), spriteData);
		
		if (amountread < (signed)sizeof(long))
		{
			printf("Stopped in the middle of a header\n");
			break;
		}
		
		
#ifdef ALLEGRO_LITTLE_ENDIAN
		// fix endian problem (need a better solution, but works for now)
		bitswap(&header.id, sizeof(header.id));
		bitswap(&header.len, sizeof(header.len));
#endif
		
//		printf("id: %d, len: %d\n", header.id, header.len);
		
		// were done
		if (header.id == -1)
		{
//			printf("End of resources reached!\n");
			break;
		}
		// immunity icons
		else if (header.id >= 132 && header.id < 132 + NUM_IMMUNITY_ICONS)
		{
			amountread = pack_fread(smallIcons, header.len, spriteData);
			
			if (amountread < header.len)
			{
				printf("Stopped in the middle of an icon resource\n");
				break;
			}
			
			g_ImmunityIcons[header.id - 132] = create_bitmap(16, 16);
			
			for(j = 0;j < 16;j++)
			{
				for(k = 0;k < 16;k++)
				{
					putpixel(g_ImmunityIcons[header.id - 132], j, k, smallIcons[(k * 16) + j]);
				}
			}
			
			// debug output
//			printf("Resource id: %d, length: %d, length unpacked: %d\n", header.id, header.len, numUnpacked);
		}
		// small icons
		else if (header.id >= 15000 && header.id < 15000 + NUM_ITEM_TEMPLATES)
		{
			amountread = pack_fread(smallIcons, header.len, spriteData);
			
			if (amountread < header.len)
			{
				printf("Stopped in the middle of an icon resource\n");
				break;
			}
			
			g_SmallIcons[header.id - 15000] = create_bitmap(16, 16);
			
			for(j = 0;j < 16;j++)
			{
				for(k = 0;k < 16;k++)
				{
					putpixel(g_SmallIcons[header.id - 15000], j, k, smallIcons[(k * 16) + j]);
				}
			}
			
			// debug output
//			printf("Resource id: %d, length: %d, length unpacked: %d\n", header.id, header.len, numUnpacked);
		}
		// tile masks
		else if (header.id >= 16000 && header.id < 16000 + NUM_ITEM_TEMPLATES)
		{
			amountread = pack_fread(largeIcons, header.len, spriteData);
			
			if (amountread < header.len)
			{
				printf("Stopped in the middle of a mask resource\n");
				break;
			}
			
			g_LargeIcons[header.id - 16000] = create_bitmap(32, 32);
			
			for(j = 0;j < 32;j++)
			{
				for(k = 0;k < 32;k++)
				{
					putpixel(g_LargeIcons[header.id - 16000], j, k, largeIcons[(k * 32) + j]);
				}
			}
			
			// debug output
//			printf("Resource id: %d, length: %d, length unpacked: %d\n", header.id, header.len, numUnpacked);
		}
		// unknown
		else
		{
//			printf("Unknown resource type reached: %d, len: %d\n", header.id, header.len);
			for(i = 0;i < header.len;i++) 
			{
				pack_getc(spriteData);
			}
		}
	}
	
	pack_fclose(spriteData);
	
	printf("Finished reading icon graphics file!\n");
	
	// save tile icons into bitmaps
	for(i = 0;i < NUM_TILE_ICONS;i++)
	{
		g_TileIcons[i] = create_bitmap(32, 32);
		
		for(j = 0;j < 32;j++)
		{
			for(k = 0;k < 32;k++)
			{
				putpixel(g_TileIcons[i], j, k, tileIcons[(i * 1024) + (k * 32) + j]);
			}
		}
	}
	
	// save boss icons into bitmaps
	for(i = 0;i < NUM_BOSS_ICONS;i++)
	{
		g_BossIcons[i] = create_bitmap(64, 64);
		
		for(j = 0;j < 64;j++)
		{
			for(k = 0;k < 64;k++)
			{
				putpixel(g_BossIcons[i], j, k, bossIcons[(i * 4096) + (k * 64) + j]);
				g_BossMasks[(i * 4096) + (k * 64) + j] = bossIcons[(i * 4096) + (k * 64) + j] ? 255 : 0;
			}
		}
	}
	
	// save enemy icons into bitmaps
	for(i = 0;i < NUM_ENEMY_ICONS;i++)
	{
		g_EnemyIcons[i] = create_bitmap(32, 32);
		
		for(j = 0;j < 32;j++)
		{
			for(k = 0;k < 32;k++)
			{
				putpixel(g_EnemyIcons[i], j, k, enemyIcons[(i * 1024) + (k * 32) + j]);
				g_EnemyMasks[(i * 1024) + (k * 32) + j] = enemyIcons[(i * 1024) + (k * 32) + j] ? 255 : 0;
			}
		}
	}
	
	// save saric icons into bitmaps
	for(i = 0;i < NUM_SARIC_ICONS;i++)
	{
		g_SaricIcons[i] = create_bitmap(32, 32);
		
		for(j = 0;j < 32;j++)
		{
			for(k = 0;k < 32;k++)
			{
				putpixel(g_SaricIcons[i], j, k, saricIcons[(i * 1024) + (k * 32) + j]);
			}
		}
	}
	
	// save sword icons into bitmaps
	for(i = 0;i < NUM_SWORD_ICONS;i++)
	{
		g_SwordIcons[i] = create_bitmap(32, 32);
		
		for(j = 0;j < 32;j++)
		{
			for(k = 0;k < 32;k++)
			{
				putpixel(g_SwordIcons[i], j, k, swordIcons[(i * 1024) + (k * 32) + j]);
			}
		}
	}
	
	// save tile masks
	memset(g_TileMasks, 0, 1024 * NUM_TILE_ICONS);
	
	for(i = 0;i < 1024 * NUM_TILE_ICONS;i++)
	{
		g_TileMasks[i] = tileMasks[i];
	}
	
	// save saric's mask
	memset(g_SaricMask, 0, 1024);
	
	for(i = 0;i < 1024;i++)
	{
		g_SaricMask[i] = saricMask[i];
	}
	
	// save sword masks
	memset(g_SwordMasks, 0, 1024 * NUM_SWORD_ICONS);
	
	for(i = 0;i < 1024 * NUM_SWORD_ICONS;i++)
	{
		g_SwordMasks[i] = swordMasks[i];
	}
	
	printf("Finished storing the bitmaps and masks!\n");
	
	// free temporary buffers
	free(smallIcons);
	free(largeIcons);
	free(tileIcons);
	free(tileMasks);
	free(enemyIcons);
	free(saricIcons);
	free(saricMask);
	free(swordIcons);
	free(swordMasks);
	free(bossIcons);
	
	// success
	return 1;
	
	// failure
ERROR_BREAK:
	
	if (tileIcons)
	{
		free(tileIcons);
	}
	if (tileMasks)
	{
		free(tileMasks);
	}
	if (enemyIcons)
	{
		free(enemyIcons);
	}
	if (saricIcons)
	{
		free(saricIcons);
	}
	if (saricMask) 
	{
		free(saricMask);
	}
	if (swordIcons)
	{
		free(swordIcons);
	}
	if (swordMasks)
	{
		free(swordMasks);
	}
	if (bossIcons)
	{
		free(bossIcons);
	}
	
	return 0;
}

int readFontData(void)
{
	/*
	BITMAP *bitmapFont = load_pcx("Times New Roman 14.pcx", NULL);
	
	if (!bitmapFont)
	{
		return 0;
	}
	
	g_Font = grab_font_from_bitmap(bitmapFont);
	destroy_bitmap(bitmapFont);
	
	if (!g_Font)
	{
		return 0;
	}
	
	printf("Read the fonts successfully!\n");
	printf("Is font colored?  %s\n", is_color_font(g_Font) ? "Yes!!!" : "No!!!");
	printf("Is font mono?  %s\n", is_mono_font(g_Font) ? "Yes!!!" : "No!!!");
	
	
	return 1;
	*/
//	g_Font = font;
	
	PACKFILE *file;
	char *buff;
	int size;
	
	if (alfont_init() != ALFONT_OK)
	{
		return 0;
	}
	
	packfile_password("musicman3320");
	file = pack_fopen("GameData.dat#FontData", "r");
	
	if (!file)
	{
		return 0;
	}
	
	if (pack_fread(&size, 4, file) < 4)
	{
		pack_fclose(file);
		
		return 0;
	}
	
#ifdef ALLEGRO_LITTLE_ENDIAN
	bitswap(&size, sizeof(size));
#endif
	
	buff = (char *)malloc(size * sizeof(char));
	
	if (pack_fread(buff, size, file) < size)
	{
		free(buff);
		pack_fclose(file);
		
		return 0;
	}
	
	g_Font = alfont_load_font_from_mem(buff, size);
	g_DialogFont = alfont_load_font_from_mem(buff, size);
	g_LargeFont = alfont_load_font_from_mem(buff, size);
	
	free(buff);
	pack_fclose(file);
	
	if (!g_Font || !g_DialogFont || !g_LargeFont)
	{
		return 0;
	}
	
	printf("Is Font Scalable: %d\n", alfont_is_scalable_font(g_Font));
	
	if (alfont_set_font_size(g_Font, 6) != ALFONT_OK || 
	    alfont_set_font_size(g_DialogFont, 6) != ALFONT_OK || 
	    alfont_set_font_size(g_LargeFont, 15) != ALFONT_OK)
	{
		return 0;
	}
	
	return 1;
}

int readStoreData(void)
{
	PACKFILE *file;
	int i, j;
	
	memset(g_Stores, 0, sizeof(g_Stores));
	
	packfile_password("musicman3320");
	file = pack_fopen("GameData.dat#StoreData", "rb");
	
	if (!file)
	{
		printf("1\n");
		return 0;
	}
	
	for(i = 0;i < NUM_STORES;i++)
	{
	//	printf("Sizeof Str255: %ld, Sizeof StoreItem: %ld\n", sizeof(Str255), sizeof(StoreItem));
		
		memset(g_Stores[i].quote, 0, sizeof(Str255));
		
		if ((j = pack_fread(g_Stores[i].quote, sizeof(Str255), file)) < (signed)sizeof(Str255))
		{
			if (pack_feof(file))
			{
				printf("Early end of file! Bytes read: %d\n", j);
			}
			else
			{
				printf("2\n");
			}
			
			pack_fclose(file);
			
			return 0;
		}
		
		printf("Quote Read: %s\n", &g_Stores[i].quote[1]);
		
		if (pack_fread(&g_Stores[i].count, sizeof(g_Stores[i].count), file) < (signed)sizeof(g_Stores[i].count))
		{
			printf("3\n");
			pack_fclose(file);
			
			return 0;
		}
		
#ifdef ALLEGRO_LITTLE_ENDIAN
		BITSWAP(g_Stores[i].count)
#endif

		g_Stores[i].item = (StoreItem *)malloc(g_Stores[i].count * sizeof(StoreItem));
		
		for(j = 0;j < g_Stores[i].count;j++)
		{
			if (pack_fread(&g_Stores[i].item[j], sizeof(StoreItem), file) < (signed)sizeof(StoreItem))
			{
				printf("4\n");
				pack_fclose(file);
				
				return 0;
			}
			
#ifdef ALLEGRO_LITTLE_ENDIAN
			BITSWAP(g_Stores[i].item[j].index)
			BITSWAP(g_Stores[i].item[j].price)
#endif
		}
	}
	
	pack_fclose(file);
	
	return 1;
}

int readAnimData(void)
{
	PACKFILE *infile;
	int i, currentBitmap = 0;
	unsigned short r, g, b;
	short readSize;
	Rect readRect;
	Point readPoint;
	int polygonPoints[10];
	int numpoints;
	
	int firstx, firsty;
	
	memset(g_SwordAnimData, 0, sizeof(g_SwordAnimData));
	
	packfile_password("musicman3320");
	infile = pack_fopen("GameData.dat#AnimData", "rb");
	
	if (!infile)
	{
		return 0;
	}
	
	for(currentBitmap = 0;currentBitmap < NUM_SWORD_FRAMES && !pack_feof(infile);currentBitmap++)
	{
	//	g_SwordAnimData[currentBitmap] = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
		g_SwordAnimData[currentBitmap] = create_bitmap(300, 300);
		clear_to_color(g_SwordAnimData[currentBitmap], makecol(0, 0, 0));
		
	//	printf("Reading a bitmap...\n");
		
		while((i = pack_getc(infile)) != 0 && i != EOF)
		{
		//	printf("Reading a polygon: %d...\n", i);
			if (i <= 36 && i > 6)
			{
				VERIFYFREAD(r, infile)
				VERIFYFREAD(g, infile)
				VERIFYFREAD(b, infile)
				VERIFYFREAD(readSize, infile)
				VERIFYFREAD(readRect.top, infile)
				VERIFYFREAD(readRect.left, infile)
				VERIFYFREAD(readRect.bottom, infile)
				VERIFYFREAD(readRect.right, infile)
				
#if defined(ALLEGRO_LITTLE_ENDIAN)
				BITSWAP(r)
				BITSWAP(g)
				BITSWAP(b)
				BITSWAP(readSize)
				BITSWAP(readRect.top)
				BITSWAP(readRect.left)
				BITSWAP(readRect.bottom)
				BITSWAP(readRect.right)
#endif
				
				i -= 6;
				
			//	printf("ReadSize: %d\n", readSize);
				
				if (readSize != i)
				{
					printf("Incompatable sizes!!!\n");
					pack_fclose(infile);
					return 0;
				}
				
				numpoints = (readSize - 2 - 8) / 4;
				
				for(i = 0;i < numpoints;i++)
				{
					VERIFYFREAD(readPoint.v, infile)
					VERIFYFREAD(readPoint.h, infile)
					
#if defined(ALLEGRO_LITTLE_ENDIAN)
					BITSWAP(readPoint.h)
					BITSWAP(readPoint.v)
#endif
					
					polygonPoints[i * 2] = readPoint.h + 150;
					polygonPoints[(i * 2) + 1] = readPoint.v + 150;
					
					if (i == 0)
					{
						firstx = polygonPoints[0];
						firsty = polygonPoints[1];
					}
				}
				
				polygonPoints[i * 2] = firstx;
				polygonPoints[(i * 2) + 1] = firsty;
				
				polygon(g_SwordAnimData[currentBitmap], numpoints + 1, polygonPoints, makecol(r / 256, g / 256, b / 256));
			}
			else
			{
				printf("This file is messed up!\n");
				pack_fclose(infile);
				return 0;
			}
		}
	}
	
//	printf("Finished...\n");
	
	pack_fclose(infile);
	
	return 1;
}

int createSavedGameData(void)
{
	SavedGame game;
	PACKFILE *file;
	int i;
	
	newGame();
	
	memset(game.name, 0, sizeof(game.name));
	sprintf(&game.name[1], "New Game");
	game.name[0] = 8;
	game.time = 0;
	game.saric = g_Saric;
	game.mapScreen.h = g_CurrentScreenX;
	game.mapScreen.v = g_CurrentScreenY;
	memset(game.deathRecord, 0xFF, sizeof(game.deathRecord));
	
	packfile_password(NULL);
	file = pack_fopen("SavedOriginal.dat", "w");
	
	if (!file)
	{
		return 0;
	}
	
	for(i = 0;i < MAX_SAVED_GAMES;i++)
	{
		if (!writeSavedGame(&game, file))
		{
			pack_fclose(file);
			return 0;
		}
	}
	
	pack_fclose(file);
	
	return 1;
}

int readSavedGameData(void)
{
	PACKFILE *file;
	int i;
	
	packfile_password(NULL);
	file = pack_fopen("Saved.dat", "r");
	
	if (!file)
	{
		return 0;
	}
	
	for(i = 0;i < MAX_SAVED_GAMES;i++)
	{
		if (!readSavedGame(&g_SavedGames[i], file))
		{
			pack_fclose(file);
			return 0;
		}
		
		printf("Read saved game!\n");
		printf("Coordinates: %d, %d\n", g_SavedGames[i].mapScreen.h, g_SavedGames[i].mapScreen.v);
	}
	
	pack_fclose(file);
	
	return 1;
}

int writeSavedGameData(void)
{
	PACKFILE *file;
	int i;
	
	packfile_password(NULL);
	file = pack_fopen("Saved.dat", "w");
	
	if (!file)
	{
		return 0;
	}
	
	for(i = 0;i < MAX_SAVED_GAMES;i++)
	{
		if (!writeSavedGame(&g_SavedGames[i], file))
		{
			pack_fclose(file);
			return 0;
		}
	}
	
	pack_fclose(file);
	
	return 1;
}

void importSavedGame(int game)
{
	int i;
	
	initMapStuff();
	
	g_Saric = g_SavedGames[game].saric;
	g_GameplayTime = g_SavedGames[game].time;
	
	for(i = 0;i < NUM_SCREENS;i++)
	{
		g_DeathRecord[i] = g_SavedGames[game].deathRecord[i];
	}
	
	g_CurrentScreenX = g_SavedGames[game].mapScreen.h;
	g_CurrentScreenY = g_SavedGames[game].mapScreen.v;
	
	loadScreen((g_CurrentScreenY * 16) + g_CurrentScreenX);
	
	playMusic(g_MapRegions[g_CurrentScreen].musicIndex);
}

void exportSavedGame(int game, Str255 name)
{
	int i;
	
	for(i = 0;i < 256;i++)
	{
		g_SavedGames[game].name[i] = name[i];
	}
	
	g_SavedGames[game].saric = g_Saric;
	g_SavedGames[game].time = g_GameplayTime;
	
	for(i = 0;i < NUM_SCREENS;i++)
	{
		g_SavedGames[game].deathRecord[i] = g_DeathRecord[i];
	}
	
	g_SavedGames[game].mapScreen.h = g_CurrentScreen % 16;
	g_SavedGames[game].mapScreen.v = g_CurrentScreen / 16;
	
	writeSavedGameData();
}

int readImagesData(void)
{
	DATAFILE *dfile;
	DATAFILE *temp;
	BITMAP *bitmap;
	
	g_WinBitmap = NULL;
	g_LoseBitmap = NULL;
	g_StoryBitmap = NULL;
	g_MantraBitmap = NULL;
	g_CursorBitmap = NULL;
	g_HelpBitmap = NULL;
	
	packfile_password("musicman3320");
	dfile = load_datafile("Images.dat");
	
	if (!dfile)
	{
		printf("Main datafile not found!\n");
		return 0;
	}
	
	temp = find_datafile_object(dfile, "win");
	
	if (!temp)
	{
		unload_datafile(dfile);
		return 0;
	}
	
	bitmap = temp->dat;
	g_WinBitmap = create_bitmap(bitmap->w, bitmap->h);
	blit(bitmap, g_WinBitmap, 0, 0, 0, 0, bitmap->w, bitmap->h);
	
	temp = find_datafile_object(dfile, "lose");
	
	if (!temp)
	{
		unload_datafile(dfile);
		return 0;
	}
	
	bitmap = temp->dat;
	g_LoseBitmap = create_bitmap(bitmap->w, bitmap->h);
	blit(bitmap, g_LoseBitmap, 0, 0, 0, 0, bitmap->w, bitmap->h);
	
	temp = find_datafile_object(dfile, "story");
	
	if (!temp)
	{
		unload_datafile(dfile);
		return 0;
	}
	
	bitmap = temp->dat;
	g_StoryBitmap = create_bitmap(bitmap->w, bitmap->h);
	blit(bitmap, g_StoryBitmap, 0, 0, 0, 0, bitmap->w, bitmap->h);
	
	temp = find_datafile_object(dfile, "mantra");
	
	if (!temp)
	{
		unload_datafile(dfile);
		return 0;
	}
	
	bitmap = temp->dat;
	g_MantraBitmap = create_bitmap(bitmap->w, bitmap->h);
	blit(bitmap, g_MantraBitmap, 0, 0, 0, 0, bitmap->w, bitmap->h);
	
	temp = find_datafile_object(dfile, "cursor");
	
	if (!temp)
	{
		unload_datafile(dfile);
		return 0;
	}
	
	bitmap = temp->dat;
	g_CursorBitmap = create_bitmap(bitmap->w, bitmap->h);
	blit(bitmap, g_CursorBitmap, 0, 0, 0, 0, bitmap->w, bitmap->h);
	
	temp = find_datafile_object(dfile, "help");
	
	if (!temp)
	{
		unload_datafile(dfile);
		return 0;
	}
	
	bitmap = temp->dat;
	g_HelpBitmap = create_bitmap(bitmap->w, bitmap->h);
	blit(bitmap, g_HelpBitmap, 0, 0, 0, 0, bitmap->w, bitmap->h);
	
	unload_datafile(dfile);
	
	return 1;
}


void freeItemData(void)
{
	int i;
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		if (g_ItemTemplates[i].name) {
			free(g_ItemTemplates[i].name);
			g_ItemTemplates[i].name = NULL;
		}
		
		if (g_ItemTemplates[i].description)
		{
			free(g_ItemTemplates[i].description);
			g_ItemTemplates[i].description = NULL;
		}
	}
}

void freeMapData(void)
{
	// not needed as of now...
}

void freeTmplEnemyData(void)
{
	// not needed as of now...
}

void freeMessageData(void)
{
	// not needed as of now...
}

void freeGraphicsData(void)
{
	int i;
	
	// for now, only bitmaps need to be freed...
	for (i = 0;i < NUM_TILE_ICONS;i++) {
		if (g_TileIcons[i])
		{
			destroy_bitmap(g_TileIcons[i]);
			g_TileIcons[i] = NULL;
		}
	}
	
	for(i = 0;i < NUM_BOSS_ICONS;i++)
	{
		if (g_BossIcons[i]) {
			destroy_bitmap(g_BossIcons[i]);
			g_BossIcons[i] = NULL;
		}
	}
	
	for(i = 0;i < NUM_SARIC_ICONS;i++)
	{
		if (g_SaricIcons[i])
		{
			destroy_bitmap(g_SaricIcons[i]);
			g_SaricIcons[i] = NULL;
		}
	}
	
	for(i = 0;i < NUM_SWORD_ICONS;i++)
	{
		if (g_SwordIcons[i])
		{
			destroy_bitmap(g_SwordIcons[i]);
			g_SwordIcons[i] = NULL;
		}
	}
	
	for(i = 0;i < NUM_ENEMY_ICONS;i++)
	{
		if (g_EnemyIcons[i])
		{
			destroy_bitmap(g_EnemyIcons[i]);
			g_EnemyIcons[i] = NULL;
		}
	}
	
	for(i = 0;i < NUM_IMMUNITY_ICONS;i++)
	{
		if (g_ImmunityIcons[i])
		{
			destroy_bitmap(g_ImmunityIcons[i]);
			g_ImmunityIcons[i] = NULL;
		}
	}
	
	for(i = 0;i < NUM_ITEM_TEMPLATES;i++)
	{
		if (g_SmallIcons[i])
		{
			destroy_bitmap(g_SmallIcons[i]);
			g_SmallIcons[i] = NULL;
		}
		
		if (g_LargeIcons[i])
		{
			destroy_bitmap(g_LargeIcons[i]);
			g_LargeIcons[i] = NULL;
		}
	}
}

void freeFontData(void)
{
	/*
	if (g_Font && g_Font != font)
	{
		destroy_font(g_Font);
		g_Font = 0;
	}
	*/
	if (g_Font)
	{
		alfont_destroy_font(g_Font);
		g_Font = NULL;
	}
	
	if (g_DialogFont)
	{
		alfont_destroy_font(g_DialogFont);
		g_DialogFont = NULL;
	}
	
	if (g_LargeFont)
	{
		alfont_destroy_font(g_LargeFont);
		g_LargeFont = NULL;
	}
	
	alfont_exit();
}

void freeStoreData(void)
{
	int i;
	
	for(i = 0;i < NUM_STORES;i++)
	{
		if (g_Stores[i].item)
		{
			free(g_Stores[i].item);
		}
	}
}

void freeAnimData(void)
{
	int currentBitmap;
	
	for(currentBitmap = 0;currentBitmap < NUM_SWORD_FRAMES;currentBitmap++)
	{
		if (g_SwordAnimData[currentBitmap])
		{
			destroy_bitmap(g_SwordAnimData[currentBitmap]);
			g_SwordAnimData[currentBitmap] = NULL;
		}
	}
}

void freeSavedGameData(void)
{
	// for now do nothing...
}

void freeImagesData(void)
{
	if (g_WinBitmap)
	{
		destroy_bitmap(g_WinBitmap);
		g_WinBitmap = NULL;
	}
	
	if (g_LoseBitmap)
	{
		destroy_bitmap(g_LoseBitmap);
		g_LoseBitmap = NULL;
	}
	
	if (g_StoryBitmap)
	{
		destroy_bitmap(g_StoryBitmap);
		g_StoryBitmap = NULL;
	}
	
	if (g_MantraBitmap)
	{
		destroy_bitmap(g_MantraBitmap);
		g_MantraBitmap = NULL;
	}
	
	if (g_CursorBitmap)
	{
		destroy_bitmap(g_CursorBitmap);
		g_CursorBitmap = NULL;
	}
}


