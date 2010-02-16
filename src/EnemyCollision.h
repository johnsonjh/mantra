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

#ifndef ENEMY_COLLISION_H
#define ENEMY_COLLISION_H

// note about possibly using old source:
// all the funcs in this file were taken from
// the "old" source, and should be checked to
// make sure they have not changed
// other things to be checked: 
//   all of EnemyUpdate.c
//   all of Saric.c
//   updateEnemies() in Enemies.c

// So, todo list (why this file?):
//   check above functions for sameness
//   "translate" functions in EnemyUpdate.c
//   Copy in the following routines and do the same:
//      CheckKeys (we call it readKeyboardInput())
//      FireEnemy, BossFireEnemy, SaricFireEnemy
//      RunItemSpecialRoutine, PowerMantraItem, 
//          KeySpecialItem
//   Look at WindowFX.c for possibly more funcs
//      to implement (not copyable, like fading)...
// That should be it for in game play.  Then, it
// should be all about the menus and intro screen
// (check out Menus.c for functions, etc)...

// We will not worry about the preferences stuff, 
// since all it does is save sound volumes...

Boolean testIntercept(short sprite1, short sprite2, Rect *r1, Rect *r2, int type1, int type2, long isBoss1, long isBoss2);
Boolean	standableRect(Rect rect);
Boolean checkEnemyInterceptWithSaric(EnemyHandle currentEnemy);
Boolean checkEnemyInterceptWithMap(EnemyHandle currentEnemy);
Boolean checkEnemyInterceptWithEnemies(EnemyHandle currentEnemy);
Boolean checkProximityToSword(EnemyHandle currentEnemy);
Boolean checkEnemyPushing(EnemyHandle currentEnemy);
Boolean	enemyStandableRect(EnemyHandle currentEnemy);

#endif
