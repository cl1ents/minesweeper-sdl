#include <SDL.h>
#include <stdlib.h>
#include <SDL_image.h>

#include "Resources.h"

SDL_Rect FullSize;

void RenderBackground(SDL_Renderer* renderer, int w, int h)
{
	FullSize.w = w;
	FullSize.h = w / 16 * 9;
	if (h > FullSize.h) {
		FullSize.w = h/9*16;
		FullSize.h = h;
	}
	
	FullSize.x = w/2 - FullSize.w/2;
	FullSize.y = h/2 - FullSize.h/2;

	SDL_RenderCopy(renderer, background_images[0], NULL, &FullSize);
}

void RenderForeground(SDL_Renderer* renderer, int w, int h)
{
	SDL_RenderCopy(renderer, background_images[1], NULL, &FullSize);
}