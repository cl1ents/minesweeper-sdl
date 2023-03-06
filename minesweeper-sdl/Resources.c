#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Sprite.h"

SDL_Texture* spritesTex;
Sprite sprites;

SDL_Texture* seashellTex;
Sprite seashell;

SDL_Texture* cursor;
SDL_Texture** ui_elements;
SDL_Texture** background_elements;

TTF_Font* font;

void initResources(SDL_Renderer *renderer) {
	cursor = IMG_LoadTexture(renderer, "./res/cursor.png");
	spritesTex = IMG_LoadTexture(renderer, "./res/sprites.png");
	seashellTex = IMG_LoadTexture(renderer, "./res/seashell.png");

	sprites.Texture = spritesTex;
	sprites.w = 32;
	sprites.h = 32;
	initSprite(&sprites);

	seashell.Texture = seashellTex;
	seashell.w = 64;
	seashell.h = 64;
	initSprite(&seashell);
	// SDL_QueryTexture(sprites, NULL, NULL, NULL, &spriteSize);

	font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 10);
}

void releaseResources() {
	SDL_DestroyTexture(cursor);
	SDL_DestroyTexture(spritesTex);
}