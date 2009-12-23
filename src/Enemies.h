/*
 *  Enemies.h
 *  Mantra
 *
 *  Created by Chris O'Neill on 1/23/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ENEMIES_H
#define ENEMIES_H

void clearEnemies(void);
void drawEnemies(void);
void drawEnemiesWithBitmap(BITMAP *bitmap);

void updateEnemies(void);

void fireEnemy(EnemyHandle currentEnemy);
Boolean bossFireEnemy(EnemyHandle currentEnemy, Point *whereIndex, short firedNum, Boolean testStuck, Boolean createdIsBoss);
void saricFireEnemy(short id);

#endif

