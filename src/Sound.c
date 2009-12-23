/*
 *  Sound.c
 *  Mantra
 *
 *  Created by Chris O'Neill on 2/23/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#if defined(MANTRA_WINDOWS)
//#include "StdAfx.h"
#include <allegro.h>
#else
#include <Allegro/allegro.h>
#endif

#include "aldumb.h"

#include "GameTypes.h"
#include "Sound.h"

char *g_SongFiles[NUM_MUSIC_SONGS];
long g_SongFileLengths[NUM_MUSIC_SONGS];
int g_CurrentSong;

int g_MusicVolume;
int g_MusicFadePercent;

DUMBFILE *g_CurrentDumbFile;
DUH *g_CurrentDuh;
AL_DUH_PLAYER *g_DuhMusicPlayer;

DATAFILE *g_SoundEffectsDatafile;


int initSoundData()
{
	DATAFILE *datafile;
	DATAFILE *tempfile;
	char buff[4];
	int i;
	
	g_CurrentDumbFile = NULL;
	g_CurrentDuh = NULL;
	g_DuhMusicPlayer = NULL;
	g_SoundEffectsDatafile = NULL;
	
	memset(g_SongFiles, 0, sizeof(g_SongFiles));
	
	packfile_password("musicman3320");
	datafile = load_datafile("Music.dat");
	
	if (!datafile)
	{
		return 0;
	}
	
	for(i = 0;i < NUM_MUSIC_SONGS;i++)
	{
		sprintf(buff, "%d", i + 128);
		tempfile = find_datafile_object(datafile, buff);
		
		if (!tempfile)
		{
			unload_datafile(datafile);
			return 0;
		}
		
		g_SongFiles[i] = (char *)malloc(tempfile->size * sizeof(char));
		memcpy(g_SongFiles[i], tempfile->dat, tempfile->size * sizeof(char));
		g_SongFileLengths[i] = tempfile->size;
	}
	
	unload_datafile(datafile);
	
	packfile_password("musicman3320");
	g_SoundEffectsDatafile = load_datafile("Sound.dat");
	
	if (!g_SoundEffectsDatafile)
	{
		return 0;
	}
	
	g_CurrentSong = 0;
	g_MusicVolume = MAX_VOLUME;
	g_MusicFadePercent = 100;
	
	set_volume_per_voice(2);
	
	if (install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL))
	{
		return 0;
	}
	
	return 1;
}

void freeSoundData()
{
	int i;
	
	for(i = 0;i < NUM_MUSIC_SONGS;i++)
	{
		if (g_SongFiles[i])
		{
			free(g_SongFiles[i]);
			g_SongFiles[i] = 0;
		}
	}
	
	if  (g_DuhMusicPlayer)
	{
		al_stop_duh(g_DuhMusicPlayer);
		g_DuhMusicPlayer = NULL;
	}
	
	if  (g_CurrentDuh)
	{
		unload_duh(g_CurrentDuh);
		g_CurrentDuh = NULL;
	}
	
	if (g_CurrentDumbFile)
	{
		dumbfile_close(g_CurrentDumbFile);
		g_CurrentDumbFile = NULL;
	}
	
	dumb_exit();
	
	if (g_SoundEffectsDatafile)
	{
		unload_datafile(g_SoundEffectsDatafile);
		g_SoundEffectsDatafile = NULL;
	}
}

void playMusic(int song)
{
	if (song != g_CurrentSong)
	{
	//	printf("Playing song!\n");
		
		if  (g_DuhMusicPlayer)
		{
			al_stop_duh(g_DuhMusicPlayer);
			g_DuhMusicPlayer = NULL;
		}
		
		if  (g_CurrentDuh)
		{
			unload_duh(g_CurrentDuh);
			g_CurrentDuh = NULL;
		}
		
		if (g_CurrentDumbFile)
		{
			dumbfile_close(g_CurrentDumbFile);
			g_CurrentDumbFile = NULL;
		}
		
		g_CurrentDumbFile = dumbfile_open_memory(g_SongFiles[song - 1], g_SongFileLengths[song - 1]);
		
		if (!g_CurrentDumbFile)
		{
			printf("Unable to load dumbfile!\n");
			return;
		}
		
		g_CurrentDuh = dumb_read_mod(g_CurrentDumbFile);
		
		if (!g_CurrentDuh)
		{
			printf("Unable to load duh!\n");
			return;
		}
		
		g_DuhMusicPlayer = al_start_duh(g_CurrentDuh, 2, 0, (1.0f * g_MusicVolume * g_MusicFadePercent) / (MAX_VOLUME * 100), 4096, 44100);
		
		if (!g_DuhMusicPlayer)
		{
			printf("Unable to load duh player!\n");
			return;
		}
		
		g_CurrentSong = song;
	}
}

void stopMusic()
{
	if  (g_DuhMusicPlayer)
	{
		al_stop_duh(g_DuhMusicPlayer);
		g_DuhMusicPlayer = NULL;
	}
	
	if  (g_CurrentDuh)
	{
		unload_duh(g_CurrentDuh);
		g_CurrentDuh = NULL;
	}
	
	if (g_CurrentDumbFile)
	{
		dumbfile_close(g_CurrentDumbFile);
		g_CurrentDumbFile = NULL;
	}
	
	g_CurrentSong = 0;
}

int musicVolume()
{
	return g_MusicVolume;
}

void setMusicVolume(int vol)
{
	g_MusicVolume = vol;
	al_duh_set_volume(g_DuhMusicPlayer, (1.0f * g_MusicVolume * g_MusicFadePercent) / (MAX_VOLUME * 100));
}

void setMusicFadePercent(int percent)
{
	g_MusicFadePercent = percent;
	al_duh_set_volume(g_DuhMusicPlayer, (1.0f * g_MusicVolume * g_MusicFadePercent) / (MAX_VOLUME * 100));
}

void playSoundEffect(int num)
{
	DATAFILE *tempfile;
	char buff[4];
	
	sprintf(buff, "%d", num);
	
	tempfile = find_datafile_object(g_SoundEffectsDatafile, buff);
	
	if (tempfile)
	{
		play_sample(tempfile->dat, (255 * g_MusicVolume) / MAX_VOLUME, 128, 1000, 0);
	}
}

void updateSound()
{
	if (g_DuhMusicPlayer)
	{
		al_poll_duh(g_DuhMusicPlayer);
	}
}
