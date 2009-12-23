#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "GameDefines.h"

typedef char Str255[256];
typedef char *StringPtr;

typedef unsigned char *Ptr;

typedef unsigned char Boolean;

#define false 0
#define true 1

typedef struct {
	short v;
	short h;
} Point;

typedef struct {
	short top;
	short left;
	short bottom;
	short right;
} Rect;

enum {
	MANTRA_KEY_0 = 0,
	MANTRA_KEY_1 = 1,
	MANTRA_KEY_2 = 2,
	MANTRA_KEY_3 = 3,
	MANTRA_KEY_4 = 4,
	MANTRA_KEY_5 = 5,
	MANTRA_KEY_6 = 6,
	MANTRA_KEY_7 = 7,
	MANTRA_KEY_UP = 8,
	MANTRA_KEY_DOWN = 9,
	MANTRA_KEY_RIGHT = 10,
	MANTRA_KEY_LEFT = 11,
	MANTRA_KEY_S = 12 ,
	MANTRA_KEY_I = 13,
	MANTRA_KEY_Q = 14,
	MANTRA_KEY_P = 15,
	MANTRA_KEY_ESC = 16,
	MANTRA_KEY_H = 17,
	MANTRA_KEY_D = 18,
	MANTRA_KEY_E = 19,
	MANTRA_KEY_T = 20,
	MANTRA_KEY_B = 21,
	MANTRA_KEY_N = 22,
	MANTRA_KEY_O = 23,
	MANTRA_KEY_R = 24,
	MANTRA_KEY_SPACE = 25,
	MANTRA_KEY_ENTER = 26,
	MANTRA_KEY_SHIFT = 27,
	MANTRA_KEY_ALT = 28
};

enum
{
	MANTRA_MOUSE_LEFT, 
	MANTRA_MOUSE_RIGHT
};

enum
{
	MANTRA_MOUSE_X,
	MANTRA_MOUSE_Y
};

enum
{
	MANTRA_MOUSE_WHEEL_H,
	MANTRA_MOUSE_WHEEL_V
};

enum
{
	dialogClose = 0,
	dialogStore = 1,
	dialogStats = 2,
	dialogItems = 3
};

// data types for stores
typedef struct
{
	short index;
	short price;
} StoreItem;

typedef struct
{
	Str255 quote;
	short count;
	StoreItem *item;
} Store;

// data types from the original game
struct Item {
	StringPtr	name;
	StringPtr	description;
	long	attributes;
	char	armor;
	char	damage;
	char	speed;
	unsigned char	rateOfFire;
	unsigned char	fireCounter;
//	char	fill0;
	short	charges;
	char	stamina;
	char	damageHealed;
	short	quantity;
	short	spriteRef;
	short	firedMonsterID;
	short	immunities;
	short	damageType;
};

struct DataFileItem {
	Str255	name;
	Str255	description;
	long	attributes;
	char	armor;
	char	damage;
	char	speed;
	unsigned char	rateOfFire;
	unsigned char	fireCounter;
//	char	fill0;
	short	charges;
	char	stamina;
	char	damageHealed;
	short	quantity;
	short	spriteRef;
	short	firedMonsterID;
	short	immunities;
	short	damageType;
};

typedef	struct DataFileItem DataFileItem, **DataFileItemHandle;
//attributes for items
// these were redone by Dustin 6/18/94, and the Map Editor and templates will be upgraded
enum {						// action when selected:
	isSword = 1,			// deselect others of this type
	isArmor = 2,			// deselect others of this type
	isMoney = 4,			// no checks
	isMessage = 8,			// no checks
	isNotSelectable = 16,	// no checks					|
	isSelectable = 32,		// toggle checkmark				|-	these are all in the
	isSpecialItem = 64,		// deselect other special items	|	"other" list onscreen
	isMissle = 128,			// this and below are flags to actions
	hasCharges = 256,
	hasSpecialRoutine = 512
};
	
//item 'tmpl' = 16000+item#
	typedef struct Item Item, *ItemPtr, **ItemHandle;

struct MapItem
{
	char	modifiers;
//	char	fill0;
	short	special;	//	this needs to be at least a short: doors need room & square ref
	short	spriteRef;	//	Unless the GWorld is more than 8,192 pixels: we’ll stick with a char.  // tile icon # (g_TileIcons[this-1000])
	short	expansion;	// I'm throwing this in just in case                                           // not used
};

typedef struct MapItem MapItem;

//Modifiers: by bit
enum{
	standable = 1,			//implemented
	isDoor = 2,				//implemented	//uses special if not to underworld
	doesDamage = 4,			//implemented	//uses special
	leadsToCastle = 8,						//uses special
	leadsToUnderWorld = 16	//implemented
};

/*game data file format:
	overworld data: per room
		160 7-byte MapItems (1120 bytes) arranged in rows (x,y)
		16	preset enemies	(560 bytes)
		1680 bytes/room
		
		assuming a 16*8 room world, this is 210k for the data file.
*/

/*game save format:
	saric data:
		this will probably just be the saric struct itself (16 or so bytes)
	equipment data:
		one byte per item: 0 means no item, number is number of items
	enemy and map data:
		only data for special enemies,
		map should be up to 16 squares per room permanently changeable.
			this makes about 56k : possibly consider changing to be less mod squares
*/

struct Saric {
	Rect	where;						//implemented
	char	legCounter;					//implemented
//	char	fill0;
	short	health;						//implemented
	short	maxHealth;					//implemented
	char	armorValue;					//implemented
	char	damage;						//implemented
	Boolean	legState;					//implemented
//	char	fill1;
	short	spriteRef;					//implemented
	Point	oldPosition;				//implemented
	char	facing;						//implemented
	char	speed;						//implemented
	Point	oldSword;					//implemented
	Boolean	swordOut;					//implemented
	Boolean wasSwordOut;				//implemented
	Boolean	logicalWasSwordOut;
	Boolean offHandOut;
	Boolean logicalOffHandWasOut;
	Boolean hadHitEnemy;				//implemented
	long	experience;					//implemented
	long	nextLevel;					//implemented
	short	level;
	short	woundCounter;				//implemented
	char	sitCounter;					//DJMimplemented 6/18/94	= time not running
	char	runCounter;					//DJMimplemented 6/18/94	= time running
	char	incrementalDamageCounter;	//implemented
	Boolean	wasOnDoor;					//implemented
	short	itemQuantities[250];		//implemented
	Boolean	itemEquipped[250];			//implemented
	short	itemCharges[250];
	Item	itemEffects[3];
	short	messageCounter;				//implemented
	short	stamina;					//implemented
	short	maxStamina;					//implemented
	long	money;						//implemented
	short	immunities;
	short	damageType;
};

typedef	struct Saric Saric, SaricPtr;

typedef	struct Enemy Enemy, *EnemyPtr, **EnemyHandle;

struct Enemy {
	EnemyHandle	previousEnemy;			//implemented
	EnemyHandle	nextEnemy;				//implemented
	Rect	where;						//implemented
	unsigned char	legCounter;			//implemented
	Boolean	legState;					//implemented
	short	health;						//implemented
	char	armorValue;					//implemented
	char	damage;						//implemented
	unsigned short	xp;					//implemented
	long	attributes;					//implemented
	short	immunities;
	short	damageType;
	char	speed;						//implemented
	char	movePhase;
	char	gaurdianRange;				//implemented
	char	facing;						//implemented
	char	rateOfFire;					//implemented
	char	originalNumber;				//implemented
	short	spriteRef;					//implemented
	short	movementType;				//implemented
	unsigned char	deadItem;			//implemented
	char	target;		//may also store message number
	Point	originalPosition;			//implemented
	short	firedEnemy;					//implemented
	short	pushableSpeed;				//implemented
	short	disFromUnitCircle;			//implemented
	Point	angledCourse;				//implemented
	short	messageID;
	char	stuckCounter;
	unsigned char	theta;				//implemented
	short	expansion2;
};	// this struct is 64 bytes long

// enemy attributes: by bit
enum {
	isEnemy = 1,			//implemented
	canBeHeld = 2,			//implemented
	killable = 4,			//implemented
	canFire = 8,			//implemented		//uses special
	pushable = 16,			//implemented		//uses special
	insubstantial = 32,		//implemented
	permanent = 64,			//implemented
	isMissile = 128,		//implemented
	originalToRoom = 256,	//implemented
	isMultiFacing = 512,	//implemented
	isBossEnemy = 1024
//immunity attributes
};

// different enumerations of enemy movement types
enum {
	none = 0,				//implemented
	randomMovement = 1,				//implemented
	homing = 2,				//implemented
	smart = 3,
	gaurdian = 4,			//implemented
	circular = 5,			
	bumpTurn = 6,			//implemented
	semihoming = 7,			//implemented
	linear = 8,				//implemented
	semibumpTurn = 9,		//implemented
	waitingForTime = 10,
	waitingForSaric = 11,
	directFire = 12,		//implemented
	dyingEnemy = 13, 	//ID 1056	//implemented
//	worm = 14,	deactivated
	doorEnemy = 15,
	hiveBoss = 50,
	crabBoss = 51,
//	lizardBoss = 52, deactivated
	blobBoss = 53,
	sentryBoss = 54,
	linearBoss = 55,
	rhinoBoss = 56,
	elementalBoss = 57,
	finalBoss = 58
};

enum {
	testSaricIntercept = 0,
	testSwordIntercept = 1,
	testEnemyIntercept = 2,
	testMapIntercept = 3,
	testBossIntercept = 4
};

typedef struct RegionCell
{
	char musicIndex;
	char nameIndex;
} RegionCell, RegionCellPtr;

typedef struct
{
	Str255 name;
	LONG_LONG time;
	Saric saric;
	Point mapScreen;
	unsigned short deathRecord[NUM_SCREENS];
} SavedGame;

#endif

