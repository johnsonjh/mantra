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
