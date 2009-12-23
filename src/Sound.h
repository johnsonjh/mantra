/*
 *  Sound.h
 *  Mantra
 *
 *  Created by Chris O'Neill on 2/23/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SOUND_H
#define SOUND_H

#define NUM_MUSIC_SONGS 9
#define NUM_SOUND_EFFECTS 11
#define MIN_VOLUME 0
#define MAX_VOLUME 7

int initSoundData();
void freeSoundData();

void playMusic(int song);
void stopMusic();

int musicVolume();
void setMusicVolume(int vol);
void setMusicFadePercent(int percent);

void playSoundEffect(int num);

void updateSound();

#endif

