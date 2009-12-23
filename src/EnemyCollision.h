/*
 *  EnemyCollision.h
 *  Mantra
 *
 *  Created by Chris O'Neill on 2/9/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
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
