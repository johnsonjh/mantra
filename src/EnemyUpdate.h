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

#ifndef ENEMY_UPDATE_H
#define ENEMY_UPDATE_H

void standingMonster(EnemyHandle currentEnemy);
void waitingForSaricMonster(EnemyHandle currentEnemy);
void dyingMonster(EnemyHandle currentEnemy);
void directFireMonster(EnemyHandle currentEnemy);
void randomMonster(EnemyHandle currentEnemy);
void homingMonster(EnemyHandle currentEnemy);
void bumpTurnMonster(EnemyHandle currentEnemy);
void semihomingMonster(EnemyHandle currentEnemy);
void gaurdianMonster(EnemyHandle currentEnemy);
void circlingMonster(EnemyHandle currentEnemy);
void smartMonster(EnemyHandle currentEnemy);
void linearMonster(EnemyHandle currentEnemy);
void blobBossMonster(EnemyHandle currentEnemy);
void hiveBossMonster(EnemyHandle currentEnemy);
void crabBossMonster(EnemyHandle currentEnemy);
void sentryBossMonster(EnemyHandle currentEnemy);
void linearBossMonster(EnemyHandle currentEnemy);
void rhinoBossMonster(EnemyHandle currentEnemy);
void elementalBossMonster(EnemyHandle currentEnemy);
void finalBossMonster(EnemyHandle currentEnemy);

#endif

