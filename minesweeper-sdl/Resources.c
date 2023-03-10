#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Sprite.h"
#include "Resources.h"

Sprite GRID_SPRITES;
Sprite SEASHELL_SPRITE;

Sprite BUBBLES_SPRITE;
Sprite EXPLOSION_SPRITE;

Sprite UIBUTTONS_SPRITE;
Sprite UIDIFFICULTY_SPRITE;

SDL_Texture* cursor;
SDL_Texture* logo;
SDL_Texture* background_images[2];

Mix_Chunk* hitmarker;
Mix_Chunk* bubbles[6];

Mix_Music* bgm[3];

TTF_Font* font;
TTF_Font* uiFont;

void initResources(SDL_Renderer *renderer) {
	cursor = IMG_LoadTexture(renderer, "./res/images/cursor.png");
	logo = IMG_LoadTexture(renderer, "./res/images/logo.png");

	background_images[0] = IMG_LoadTexture(renderer, "./res/images/bg.png");
	background_images[1] = IMG_LoadTexture(renderer, "./res/images/fg.png");

	GRID_SPRITES.Texture = IMG_LoadTexture(renderer, "./res/images/sprites.png");
	GRID_SPRITES.w = 32;
	GRID_SPRITES.h = 32;
	initSprite(&GRID_SPRITES);

	SEASHELL_SPRITE.Texture = IMG_LoadTexture(renderer, "./res/images/seashell.png");
	SEASHELL_SPRITE.w = 64;
	SEASHELL_SPRITE.h = 64;
	initSprite(&SEASHELL_SPRITE);

	// Bubbles
	BUBBLES_SPRITE.Texture = IMG_LoadTexture(renderer, "./res/images/bubbles.png");
	BUBBLES_SPRITE.w = 9;
	BUBBLES_SPRITE.h = 9;
	initSprite(&BUBBLES_SPRITE);

	// Explosion
	EXPLOSION_SPRITE.Texture = IMG_LoadTexture(renderer, "./res/images/explosion.png");
	EXPLOSION_SPRITE.w = 32;
	EXPLOSION_SPRITE.h = 32;
	initSprite(&EXPLOSION_SPRITE);

	// Ui Buttons
	UIBUTTONS_SPRITE.Texture = IMG_LoadTexture(renderer, "./res/images/button.png");
	UIBUTTONS_SPRITE.w = 115;
	UIBUTTONS_SPRITE.h = 35;
	initSprite(&UIBUTTONS_SPRITE);

	// Ui Difficulty
	UIDIFFICULTY_SPRITE.Texture = IMG_LoadTexture(renderer, "./res/images/difficulty.png");
	UIDIFFICULTY_SPRITE.w = 35;
	UIDIFFICULTY_SPRITE.h = 35;
	initSprite(&UIDIFFICULTY_SPRITE);

	// Win Banner
	WINBANNER_SPRITE.Texture = IMG_LoadTexture(renderer, "./res/images/winBanner.png");
	WINBANNER_SPRITE.w = 456;
	WINBANNER_SPRITE.h = 150;
	initSprite(&WINBANNER_SPRITE);

	// Lose Banner
	LOSEBANNER_SPRITE.Texture = IMG_LoadTexture(renderer, "./res/images/loseBanner.png");
	LOSEBANNER_SPRITE.w = 455;
	LOSEBANNER_SPRITE.h = 250;
	initSprite(&LOSEBANNER_SPRITE);

	// Sounds

	hitmarker = Mix_LoadWAV("./res/sounds/hitmarker.mp3");
	char str[50];
	for (int i = 0; i < 6; i++)
	{
		sprintf_s(str, 50, "./res/sounds/bubble%d.mp3", i);
		bubbles[i] = Mix_LoadWAV(str);
	}

	// Music
	bgm[0] = Mix_LoadMUS("./res/music/bgm.mp3");
	bgm[1] = Mix_LoadMUS("./res/music/lose.mp3");
	bgm[2] = Mix_LoadMUS("./res/music/win.mp3");

	font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 10);
	uiFont = TTF_OpenFont("./res/Kavoon-Regular.ttf", 20);
}

void releaseResources() {
	SDL_DestroyTexture(cursor);

	SDL_DestroyTexture(GRID_SPRITES.Texture);
	SDL_DestroyTexture(SEASHELL_SPRITE.Texture);

	SDL_DestroyTexture(background_images[0]);
	SDL_DestroyTexture(background_images[1]);
}