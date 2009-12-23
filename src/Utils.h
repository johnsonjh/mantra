/*
 *  Utils.h
 *  Mantra
 *
 *  Created by Chris O'Neill on 1/20/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef UTILS_H
#define UTILS_H

// useful macros
//#define ABS(a) (((a)<0)?(-(a)):(a))
#define CHECK_IMMUNITIES(a,b) ((b)&(~(a)))

// useful functions
//int abs(int a);

void offsetRect(Rect *rect, int h, int v);

// in case we port this, only this function will change
int generateRand();

// reimplementation of Random(), uses generateRand()
short shortRand();

// deal with handles
#define freeHandle(a) freeMallocHandle((void **)(a))

void **mallocHandle(unsigned int bytes);
void freeMallocHandle(void **handle);

// always use these to draw in case we change screen size
int createWindow();
void drawBufferToScreen();

// better versions of text output functions
// (these can handle newlines...)
void getCharLengths(ALFONT_FONT *f, int lengths[256]);
int textout_paragraph_ex(BITMAP *bmp, ALFONT_FONT *f, const char *s, int x, int y, int color, int backg, int pixlen);
int textout_paragraph_aa_ex(BITMAP *bmp, ALFONT_FONT *f, const char *s, int x, int y, int color, int backg, int pixlen);

// graphics functions... this definitely needs its own file
void drawStats();
void drawFrame();
void showHelp();
void showCredits();

void fadeInFromColor(RGB color);
void fadeOutToColor(RGB color);
void fadeFromBlack();
void fadeToBlack();
void fadeFromColorToColor(RGB from, RGB to);

void showBitmapCentered(BITMAP *pic);

void winGame();
void loseGame();

#endif

