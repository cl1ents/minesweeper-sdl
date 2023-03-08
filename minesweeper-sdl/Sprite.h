#pragma once

typedef SDL_Texture SDL_Texture;
typedef SDL_Renderer SDL_Renderer;
typedef SDL_Rect SDL_Rect;

typedef struct Sprite {
	struct SDL_Rect;
	SDL_Texture * Texture;
	int iw;
	int ih;
	int count;
} Sprite;

void initSprite(Sprite* sprite);
void RenderSprite(SDL_Renderer* renderer, SDL_Rect* rect, Sprite* sprite, int index);
