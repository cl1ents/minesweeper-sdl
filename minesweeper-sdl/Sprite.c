#include <SDL_image.h>
#include <SDL.h>
#include <stdio.h>

#include "Sprite.h"

void initSprite(Sprite* sprite) {
	SDL_QueryTexture(sprite->Texture, NULL, NULL, &sprite->iw, &sprite->ih);
	printf("%d %d", sprite->iw, sprite->ih);
	sprite->count = sprite->iw / sprite->w * sprite->ih / sprite->h;
};

void RenderSprite(SDL_Renderer* renderer, SDL_Rect* rect, Sprite* sprite, int index) {
	sprite->x = (sprite->w * index);
	sprite->y = (sprite->x / sprite->iw) * sprite->h;
	sprite->x %= sprite->iw;

	SDL_RenderCopy(renderer, sprite->Texture, sprite, rect);
};