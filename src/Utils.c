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

#if defined(MANTRA_WINDOWS)
//#include "StdAfx.h"
#include <allegro.h>
#else
#include <Allegro/allegro.h>
#endif

#include <string.h>

#include "GameTypes.h"
#include "GameConstants.h"
#include "Sound.h"
#include "Map.h"
#include "Input.h"
#include "Utils.h"
/*
int abs(int a)
{
	return a >= 0 ? a : -a;
}
*/
void offsetRect(Rect *rect, int h, int v)
{
	rect->left += h;
	rect->right += h;
	rect->top += v;
	rect->bottom += v;
}

int generateRand()
{
	return (int)(rand() & 0xffffffff);
}

short shortRand()
{
	return (short)generateRand();
}

void **mallocHandle(unsigned int bytes)
{
	void **retHandle = (void **)malloc(sizeof(void *));
	(*retHandle) = malloc(bytes);
	
	return retHandle;
}

void freeMallocHandle(void **handle)
{
	free(*handle);
	free(handle);
}

int createWindow()
{
	// for now, we play in a window
	// will mess with full screen later...
	if (set_gfx_mode(GFX_AUTODETECT, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0))
	{
		printf("Error setting 512x320x8 gfx mode:\n%s\n", allegro_error);
		return 0;
	}
	
//	clear_to_color(screen, makecol(255, 255, 255));
	
	g_ScreenBuffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	if (!g_ScreenBuffer)
	{
		printf("Unable to create screen buffer...\n");
		return 0;
	}
	
	return 1;
}

void drawBufferToScreen()
{
	blit(g_ScreenBuffer, screen, 0, 0, 0, 0, g_ScreenBuffer->w, g_ScreenBuffer->h);
}

void getCharLengths(ALFONT_FONT *f, int lengths[256])
{
	char buff[2];
	int i;
	
	buff[1] = 0;
	
	for(i = 0;i < 256;i++)
	{
		buff[0] = (char)i;
		lengths[i] = alfont_text_length(f, buff);
	}
}

typedef void (*ParagraphCallback_ex)(BITMAP *, ALFONT_FONT *, const char *, int, int, int, int);

// utility function
// does everything with a callback!
int textout_paragraph_callback_ex(BITMAP *bmp, ALFONT_FONT *f, const char *str, int x, int y, int color, int backg, int pixlen, ParagraphCallback_ex func)
{
	int charLengths[256];
	char *buffer;
	int height = 2 * alfont_text_height(f);
	int len = strlen(str);
	int currentline = 0;
	int linelength = 0;
	int lastspace = -1;
	int lastnewline = 0;
	int extraspace = alfont_get_char_extra_spacing(f);
	char savechar;
	int i;
	
	getCharLengths(f, charLengths);
	
	buffer = (char *)malloc((len + 1) * sizeof(char));
	strncpy(buffer, str, len + 1);
	
//	printf("About to enter loop...\n");
//	printf("Pixlen: %d\n", pixlen);
//	fflush(stdout);
	
	for(i = 0;i < len;i++)
	{
	//	printf("Iterating...\n");
	//	fflush(stdout);
		
		if (buffer[i] == ' ')
		{
			lastspace = i;
		}
		
		linelength += charLengths[(int)buffer[i]];
		
		if (linelength >= pixlen || buffer[i] == '\n' || i == len - 1)
		{
			if (buffer[i] == '\n')
			{
				// do nothing for now
			}
			else if (linelength >= pixlen)
			{
				if (lastspace != -1)
				{
					i = lastspace;
				}
				else
				{
					i--;
				}
			}
			else if (i == len - 1)
			{
				i++;
			}
			
			savechar = buffer[i];
			
			buffer[i] = 0;
			
			func(bmp, f, &buffer[lastnewline], x, y + (height * currentline), color, backg);
			
			buffer[i] = savechar;
			
			currentline++;
			lastspace = -1;
			
			while((buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n') && buffer[i] != 0)
			{
				i++;
			}
			
		//	i--;
			lastnewline = i;
			
			linelength = 0;
		}
		else
		{
			linelength += extraspace;
		}
	}
	
	free(buffer);
	
	return currentline + 1;
}

int textout_paragraph_ex(BITMAP *bmp, ALFONT_FONT *f, const char *str, int x, int y, int color, int backg, int pixlen)
{
	return textout_paragraph_callback_ex(bmp, f, str, x, y, color, backg, pixlen, alfont_textout_ex);
}

int textout_paragraph_aa_ex(BITMAP *bmp, ALFONT_FONT *f, const char *str, int x, int y, int color, int backg, int pixlen)
{
	return textout_paragraph_callback_ex(bmp, f, str, x, y, color, backg, pixlen, alfont_textout_aa_ex);
}

void drawStats()
{
	// not implemented yet...
	// will be done soon
	
	// note: DrawStats in the old Game.h
	// has some secret stuff that was
	// apparently removed...  Apparently
	// they had a name for each region, 
	// and displayed it at the bottom with
	// the stats... Might be fun to re-add
	
	int blackColor = makecol(0, 0, 0);
	int grayColor = makecol(80, 80, 80);
	int yellowColor = makecol(128, 255, 0);
	int greenColor = makecol(0, 255, 0);
	int redColor = makecol(255, 0, 0);
	
	char buff[20];
	
	rectfill(g_ScreenBuffer, 0, 10*32, 16*32, 10*32 + STATS_HEIGHT, blackColor);
	vline(g_ScreenBuffer, 100, 10*32, 10*32 + STATS_HEIGHT, grayColor);
	vline(g_ScreenBuffer, 256, 10*32, 10*32 + STATS_HEIGHT, grayColor);
	vline(g_ScreenBuffer, 412, 10*32, 10*32 + STATS_HEIGHT, grayColor);
	
	memset(buff, 0, 20 * sizeof(char));
	sprintf(buff, "Money: %ld", g_Saric.money);
	alfont_textout_ex(g_ScreenBuffer, g_Font, buff, 6, 32*10 + ((STATS_HEIGHT - alfont_text_height(g_Font)) / 2) + TEXT_HEIGHT_EXTRA, yellowColor, -1);
	
	rect(g_ScreenBuffer, 105, 324, 251, 338, greenColor);
	
	if (g_Saric.maxStamina)
	{
		rectfill(g_ScreenBuffer, 107, 326, (107 + (((g_Saric.stamina >= 0) ? g_Saric.stamina : 0) * 142) / g_Saric.maxStamina), 336, greenColor);
	}
	
	memset(buff, 0, 20 * sizeof(char));
	sprintf(buff, "%d", g_Saric.stamina);
	alfont_textout_ex(g_ScreenBuffer, g_Font, buff, 111, 327 + ((10 - alfont_text_height(g_Font)) / 2) + TEXT_HEIGHT_EXTRA, blackColor, -1);
	
	memset(buff, 0, 20 * sizeof(char));
	sprintf(buff, "Exp: %ld", g_Saric.experience);
	alfont_textout_ex(g_ScreenBuffer, g_Font, buff, 418, 32*10 + ((STATS_HEIGHT - alfont_text_height(g_Font)) / 2) + TEXT_HEIGHT_EXTRA, yellowColor, -1);
	
	rect(g_ScreenBuffer, 261, 324, 407, 338, redColor);
	
	if (g_Saric.maxHealth)
	{
		rectfill(g_ScreenBuffer, 263, 326, (263 + (((g_Saric.health >= 0) ? g_Saric.health : 0) * 142) / g_Saric.maxHealth), 336, redColor);
	}
	
	memset(buff, 0, 20 * sizeof(char));
	sprintf(buff, "%d", g_Saric.health);
	alfont_textout_ex(g_ScreenBuffer, g_Font, buff, 267, 327 + ((10 - alfont_text_height(g_Font)) / 2) + TEXT_HEIGHT_EXTRA, blackColor, -1);
}

void drawFrame()
{
	
}

void showHelp()
{
	BITMAP *temp = create_bitmap(g_ScreenBuffer->w, g_ScreenBuffer->h);
	int color = getpixel(g_HelpBitmap, 0, 0);
	
	blit(g_ScreenBuffer, temp, 0, 0, 0, 0, g_ScreenBuffer->w, g_ScreenBuffer->h);
//	fadeToBlack();
	clear_to_color(g_ScreenBuffer, color);
	blit(g_HelpBitmap, g_ScreenBuffer, 0, 0, (g_ScreenBuffer->w - g_HelpBitmap->w) / 2, (g_ScreenBuffer->h - g_HelpBitmap->h) / 2, g_HelpBitmap->w, g_HelpBitmap->h);
	drawBufferToScreen();
//	showBitmapCentered(g_HelpBitmap);
//	fadeFromBlack();
	waitForKeyPressed();
//	fadeToBlack();
	blit(temp, g_ScreenBuffer, 0, 0, 0, 0, g_ScreenBuffer->w, g_ScreenBuffer->h);
//	fadeFromBlack();
}

void showCredits()
{
//	int count;
	BITMAP *creditBitmap;
	int x, y, width, height;
	int whiteColor = makecol(255, 255, 255);
	int i;
	
	const int extragap = 5;
	
	char *creditStrings[] = {
	//	"By The Syzygy Cult", 
		"", 
		"Programmers:", 
		"------------", 
		"Jake Beal", 
		"Dustin Mitchell", 
		"Macneil Shonle", 
		"Christopher O'Neill**", 
		"", 
		"", 
		"Graphics:", 
		"---------", 
		"Brett Thayer", 
		"Ernest Liu**"
		"", 
		"", 
		"Game Design:", 
		"------------", 
		"Gabe Ganberg", 
		"", 
		"", 
		"Music:", 
		"------", 
		"Ben Birney", 
		"", 
		"", 
		"** Worked on new version", 
		NULL, 
	};
	
//	for(count = 0;creditStrings[count];count++);
	
//	printf("Number of strings: %d\n", count);
	
	width = g_MantraBitmap->w;
	height = g_MantraBitmap->h;
	
	for(i = 0;creditStrings[i];i++)
	{
		int tempWidth = alfont_text_length(g_Font, creditStrings[i]);
		
		if (width < tempWidth)
		{
			width = tempWidth;
		}
		
		height += alfont_text_height(g_Font) + TEXT_HEIGHT_EXTRA + extragap;
	}
	
	creditBitmap = create_bitmap(width, height);
	
	clear_to_color(creditBitmap, g_BlackColor);
	blit(g_MantraBitmap, creditBitmap, 0, 0, (creditBitmap->w - g_MantraBitmap->w) / 2, 0, g_MantraBitmap->w, g_MantraBitmap->h);
	
	for(i = 0, y = g_MantraBitmap->h;creditStrings[i];i++)
	{
		alfont_textout_centre_ex(creditBitmap, g_Font, creditStrings[i], creditBitmap->w / 2, y, whiteColor, -1);
		
		y += alfont_text_height(g_Font) + TEXT_HEIGHT_EXTRA + extragap;
	}
	
	x = (g_ScreenBuffer->w - width) / 2;
	y = g_ScreenBuffer->h;
	
	for(;y + height >= 0;y -= 1)
	{
		clear_to_color(g_ScreenBuffer, g_BlackColor);
		blit(creditBitmap, g_ScreenBuffer, 0, 0, x, y, creditBitmap->w, creditBitmap->h);
		drawBufferToScreen();
		
		pollKeys();
		pollMouseButtons();
		
		if (anyKeyPressed() || anyMouseButtonPressed())
		{
			fadeToBlack();
			break;
		}
		
		rest(NUM_MILLISECONDS_BETWEEN_REFRESH / 3);
	}
	
	destroy_bitmap(creditBitmap);
}

void fadeInFromColor(RGB color)
{
	PALETTE solidPalette;
	PALETTE tempPalette;
	int i;
	
	for(i = 0;i < 256;i++)
	{
		solidPalette[i].r = color.r;
		solidPalette[i].g = color.g;
		solidPalette[i].b = color.b;
	}
	
	for(i = 0;i <= FADE_LENGTH;i++)
	{
		fade_interpolate(solidPalette, g_SystemPalette, tempPalette, (i * 64) / FADE_LENGTH, 0, 255);
		set_palette(tempPalette);
		drawBufferToScreen();
		
		setMusicFadePercent((i * 100) / FADE_LENGTH);
		updateSound();
		rest(FADE_REST_MILLISECONDS);
	}
	
	set_palette(g_SystemPalette);
}

void fadeOutToColor(RGB color)
{
	PALETTE solidPalette;
	PALETTE tempPalette;
	int i;
	
	for(i = 0;i < 256;i++)
	{
		solidPalette[i].r = color.r;
		solidPalette[i].g = color.g;
		solidPalette[i].b = color.b;
	}
	
	for(i = 0;i <= FADE_LENGTH;i++)
	{
		fade_interpolate(solidPalette, g_SystemPalette, tempPalette, ((FADE_LENGTH - i) * 64) / FADE_LENGTH, 0, 255);
		set_palette(tempPalette);
		drawBufferToScreen();
		
		setMusicFadePercent(((FADE_LENGTH - i) * 100) / FADE_LENGTH);
		updateSound();
		rest(FADE_REST_MILLISECONDS);
	}
	
//	clear_to_color(g_ScreenBuffer, color);
	set_palette(solidPalette);
}

void fadeFromColorToColor(RGB from, RGB to)
{
	PALETTE solidFromPalette;
	PALETTE solidToPalette;
	PALETTE tempPalette;
	int i;
	
	for(i = 0;i < 256;i++)
	{
		solidFromPalette[i].r = from.r;
		solidFromPalette[i].g = from.g;
		solidFromPalette[i].b = from.b;
		solidToPalette[i].r = to.r;
		solidToPalette[i].g = to.g;
		solidToPalette[i].b = to.b;
	}
	
	for(i = 0;i <= FADE_LENGTH;i++)
	{
		fade_interpolate(solidToPalette, solidFromPalette, tempPalette, ((FADE_LENGTH - i) * 64) / FADE_LENGTH, 0, 255);
		set_palette(tempPalette);
		drawBufferToScreen();
		
		updateSound();
		rest(FADE_REST_MILLISECONDS);
	}
	
	//	clear_to_color(g_ScreenBuffer, color);
	set_palette(solidToPalette);
}

void fadeFromBlack()
{
	RGB black;
	
	black.r = black.g = black.b = 0;
	
	fadeInFromColor(black);
}

void fadeToBlack()
{
	RGB black;
	
	black.r = black.g = black.b = 0;
	
	fadeOutToColor(black);
	
	clear_to_color(g_ScreenBuffer, g_BlackColor);
	set_palette(g_SystemPalette);
	drawBufferToScreen();
}

void showBitmapCentered(BITMAP *pic)
{
//	int blackColor;
	/*
	set_palette(g_SystemPalette);
	blackColor = makecol(0, 0, 0);
	unselect_palette();
	
	fade_out(8);
	*/
	clear_to_color(g_ScreenBuffer, g_BlackColor);
	blit(pic, g_ScreenBuffer, 0, 0, (g_ScreenBuffer->w - pic->w) / 2, (g_ScreenBuffer->h - pic->h) / 2, pic->w, pic->h);
//	drawBufferToScreen();
	
//	fade_in(g_SystemPalette, 8);
	
//	waitForKeyPressed();
}

void winGame()
{
	PALETTE bwPalette;
//	RGB color;
	int i;
	
//	color.r = color.g = color.b = 0;
	
	memset(bwPalette, 0, sizeof(PALETTE));
	
	for(i = 0;i < 256;i++)
	{
		int tempcount = 0;
		
		if (g_SystemPalette[i].r > 32)
		{
			tempcount++;
		}
		
		if (g_SystemPalette[i].g > 32)
		{
			tempcount++;
		}
		
		if (g_SystemPalette[i].b > 32)
		{
			tempcount++;
		}
		
		bwPalette[i].r = bwPalette[i].g = bwPalette[i].b = (tempcount > 1) ? 63 : 0;
	}
	
	stopMusic();
	
	playSoundEffect(131);
	playSoundEffect(138);
	
	fade_from(g_SystemPalette, bwPalette, 8);
	
	rest(300);
	
	fade_from(bwPalette, black_pallete, 8);
//	fadeToBlack();
	
	showBitmapCentered(g_WinBitmap);
	
	fadeFromBlack();
	
	waitForKeyPressed();
	
	fadeToBlack();
	
	g_GameInProgress = 0;
	
	/*
	GWorldPtr	oldGW;
	GDHandle	oldGD;
	
	long		dummy;
	
	GetGWorld( &oldGW, &oldGD );
	
	MusicDisable();
	
	PlaySndAsynchChannel( 131, kChan2, kHighPriority );	// Enemy sound, goes on chan 2
	PlaySndAsynchChannel( 138, kChan4, kHighPriority );	// special sound, chan 4
	
	FadeToPalette( 60, 130 );	// fade to black and white
	
	Delay( 15L, &dummy );		// wait a little bit
	
	FadeToPalette( 1, 8 );
	FadeToBlack( 15 );			// fade to black
	
	SetPort( window );
	PaintRect( &window->portRect );
	
	// Ugly but does the trick!
	
	// Draw that picture!
	{	PicHandle picture = (PicHandle)GetResource( 'PICT', 1002 );
		
		Rect rect;
		rect.left = (512/2) - (242/2);
		rect.right = rect.left + 242;
		rect.top = (342/2) - (106/2);
		rect.bottom = rect.top + 106;
		
		DrawPicture( picture, &rect );
		ReleaseResource( (Handle)picture );
		
		FadeToPalette( 15, 8 );		// fade up to normal
		
		Delay( 60L, &dummy );
		
		// Open ’er up!
		{	Rect scrollOne, scrollTwo;
			long	daTicks;
			int		uniqueI;
			
			SetRect( &scrollOne, rect.left, rect.top, (512/2), rect.bottom );
			scrollTwo = scrollOne;
			scrollTwo.left = (512/2);
			scrollTwo.right = rect.right;
			
			for( uniqueI=0; uniqueI < 14; uniqueI++ )
			{	Rect eraseRect;	// wrong, there is no flicker
				
				daTicks = TickCount() + 8;
				
				ScrollRect( &scrollOne, -10, 0, nil );
				ScrollRect( &scrollTwo, 10, 0, nil );
				
				scrollOne.right -= 10;
				scrollTwo.left += 10;
				
				eraseRect = scrollOne;
				eraseRect.left = scrollOne.right;
				eraseRect.right = scrollTwo.left;
				PaintRect( &eraseRect );
				
				while( daTicks > TickCount() )
					;
			}
		}
	}
	
	{	PicHandle	saricGuy = (PicHandle)GetResource( 'PICT', 133 );
		PicHandle	saricGuy2 = (PicHandle)GetResource( 'PICT', 131 );
		Rect		saricGuyRect;
		
		saricGuyRect.left = (512/2) - (24/2);
		saricGuyRect.right = saricGuyRect.left + 24;
		saricGuyRect.top = (342/2) - (35/2);
		saricGuyRect.bottom = saricGuyRect.top + 35;
		
		while( true )
		{	for( dummy=TickCount()+ 30; dummy>TickCount(); )
		{	if( KeyMouseDown() )
		{	goto out;
		}
		}
			DrawPicture( saricGuy, &saricGuyRect );
			
			for( dummy=TickCount()+ 30; dummy>TickCount(); )
			{	if( KeyMouseDown() )
			{	goto out;
			}
			}
			DrawPicture( saricGuy2, &saricGuyRect );
		}
		
out:;
		
		ReleaseResource( (Handle)saricGuy );
		ReleaseResource( (Handle)saricGuy2 );
	}
	
	done = true;
	isGameCurrentlyOpen = false;
	
	SetGWorld( oldGW, oldGD );
	*/
}

void loseGame()
{
//	PALETTE redPalette;
	RGB red, black;
//	int i;
	
	red.r = 63;
	red.g = red.b = 0;
	
	black.r = black.g = black.b = 0;
	/*
	memset(redPalette, 0, sizeof(PALETTE));
	
	for(i = 0;i < 256;i++)
	{
		redPalette[i].r = 63;
	}
	*/
	stopMusic();
	
//	fade_from(g_SystemPalette, redPalette, 8);
	
	fadeOutToColor(red);
	
	rest(150);
	
	fadeFromColorToColor(red, black);
	
	clear_to_color(g_ScreenBuffer, g_BlackColor);
	drawBufferToScreen();
	
	showBitmapCentered(g_LoseBitmap);
	
	fadeFromBlack();
	
	waitForKeyPressed();
	
	fadeToBlack();
	
	g_GameInProgress = 0;
	
	/*
	GWorldPtr	oldGW;
	GDHandle	oldGD;
	
	long		dummy;
	
	GetGWorld( &oldGW, &oldGD );
	
	MusicDisable();
	
	FadeToPalette( 60, 129 );	// fade to red
	
	Delay( 15L, &dummy );		// wait a little bit
	
	FadeToPalette( 1, 8 );
	FadeToBlack( 15 );			// fade to black
	
	SetPort( window );
	PaintRect( &window->portRect );
	
	// Ugly but does the trick!
	
	// Draw that picture!
	{	PicHandle picture = (PicHandle)GetResource( 'PICT', 1003 );
		
		Rect rect;
		rect.left = (512/2) - (242/2);
		rect.right = rect.left + 242;
		rect.top = (342/2) - (106/2);
		rect.bottom = rect.top + 106;
		
		DrawPicture( picture, &rect );
		ReleaseResource( (Handle)picture );
		
		FadeToPalette( 15, 8 );		// fade up to normal
		
		Delay( 60L, &dummy );
		
		// Open ’er up!
		{	Rect scrollOne, scrollTwo;
			long	daTicks;
			int		uniqueI;
			
			SetRect( &scrollOne, rect.left, rect.top, (512/2), rect.bottom );
			scrollTwo = scrollOne;
			scrollTwo.left = (512/2);
			scrollTwo.right = rect.right;
			
			for( uniqueI=0; uniqueI < 14; uniqueI++ )
			{	Rect eraseRect;	// wrong, there is no flicker
				
				daTicks = TickCount() + 8;
				
				ScrollRect( &scrollOne, -10, 0, nil );
				ScrollRect( &scrollTwo, 10, 0, nil );
				
				scrollOne.right -= 10;
				scrollTwo.left += 10;
				
				eraseRect = scrollOne;
				eraseRect.left = scrollOne.right;
				eraseRect.right = scrollTwo.left;
				PaintRect( &eraseRect );
				
				while( daTicks > TickCount() )
					;
			}
		}
	}
	
	done = true;
	isGameCurrentlyOpen = false;
	
	SetGWorld( oldGW, oldGD );
	*/
}


