/*
 *  LoadData.h
 *  Mantra
 *
 *  Created by Chris O'Neill on 1/14/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef LOAD_DATA_H
#define LOAD_DATA_H

// Utility functions
void bitswap(void *vdata, unsigned int len);
int myUnpackBitsSrc(void *vsrc, void *vdest, unsigned int numSrcBytes);
int myUnpackBitsDest(void *vsrc, void *vdest, unsigned int numDestBytes);

// Reading utility functions
int readEnemy(EnemyPtr enemy, PACKFILE *file);
int readMapItem(MapItem *item, PACKFILE *file);
int readItem(Item *item, PACKFILE *file);
int readSaric(Saric *saric, PACKFILE *file);
int writeSaric(Saric *saric, PACKFILE *file);
int readSavedGame(SavedGame *game, PACKFILE *file);
int writeSavedGame(SavedGame *game, PACKFILE *file);

void newGame(void);

int openGame(char *filename);

int readDataFileItem(DataFileItem *item, PACKFILE *file);

// Data loading functions
void createTempFiles(void);
int readItemData(void);
int readMapData(void);
int readTmplEnemyData(void);
int readMessageData(void);
int readGraphicsData(void);
int readFontData(void);
int readStoreData(void);
int readAnimData(void);
int createSavedGameData(void);
int readSavedGameData(void);
int writeSavedGameData(void);
void importSavedGame(int game);
void exportSavedGame(int game, Str255 name);
int readImagesData(void);

// Remove when done
void deleteTempFiles(void);
void freeItemData(void);
void freeMapData(void);
void freeTmplEnemyData(void);
void freeMessageData(void);
void freeGraphicsData(void);
void freeFontData(void);
void freeStoreData(void);
void freeAnimData(void);
void freeSavedGameData(void);
void freeImagesData(void);

#endif
