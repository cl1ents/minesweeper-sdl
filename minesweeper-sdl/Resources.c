#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

SDL_Texture* sprites;
int spriteSize;

SDL_Texture** ui_elements;
SDL_Texture* cursor;
SDL_Texture** background_elements;

TTF_Font* font;

void initResources(SDL_Renderer *renderer) {
	cursor = IMG_LoadTexture(renderer, "./res/cursor.png");
	sprites = IMG_LoadTexture(renderer, "./res/sprites.png");

	SDL_QueryTexture(sprites, NULL, NULL, NULL, &spriteSize);
}

void releaseResources() {
	SDL_DestroyTexture(cursor);
	SDL_DestroyTexture(sprites);
}