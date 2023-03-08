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

Sprite sprites;
Sprite seashell;

Sprite particles_sprites[2];

SDL_Texture* cursor;
SDL_Texture* ui_elements[2];
SDL_Texture* background_images[2];

Mix_Chunk* hitmarker;
Mix_Chunk* bubbles[6];

TTF_Font* font;
TTF_Font* uiFont;

void initResources(SDL_Renderer *renderer) {
	cursor = IMG_LoadTexture(renderer, "./res/images/cursor.png");

	background_images[0] = IMG_LoadTexture(renderer, "./res/images/bg.png");
	background_images[1] = IMG_LoadTexture(renderer, "./res/images/fg.png");

	sprites.Texture = IMG_LoadTexture(renderer, "./res/images/sprites.png");
	sprites.w = 32;
	sprites.h = 32;
	initSprite(&sprites);

	seashell.Texture = IMG_LoadTexture(renderer, "./res/images/seashell.png");
	seashell.w = 64;
	seashell.h = 64;
	initSprite(&seashell);

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

	hitmarker = Mix_LoadWAV("./res/sounds/hitmarker.mp3");
	char str[50];
	for (int i = 0; i < 6; i++)
	{
		sprintf_s(str, 50, "./res/sounds/bubble%d.mp3", i);
		bubbles[i] = Mix_LoadWAV(str);
	}

	font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 10);
	uiFont = TTF_OpenFont("./res/Kavoon-Regular.ttf", 20);
}

void releaseResources() {
	SDL_DestroyTexture(cursor);

	SDL_DestroyTexture(sprites.Texture);
	SDL_DestroyTexture(seashell.Texture);

	SDL_DestroyTexture(background_images[0]);
	SDL_DestroyTexture(background_images[1]);
}