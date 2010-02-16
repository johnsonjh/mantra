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

