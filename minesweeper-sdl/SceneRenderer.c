#include <SDL.h>
#include <stdlib.h>
#include <SDL_image.h>

#include "Resources.h"

SDL_Texture *renderedBackground;
SDL_Texture *renderedForeground;

SDL_Rect FullSize;

SDL_Rect seashell1_pos = { 351, 176, 64, 64 };
int seashellFrame = 0;

SDL_Rect TextureSize;

void initSceneRenderer(SDL_Renderer* renderer)
{
	SDL_QueryTexture(background_images[0], NULL, NULL, &TextureSize.w, &TextureSize.h);
	renderedBackground = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TextureSize.w, TextureSize.h);
	renderedForeground = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TextureSize.w, TextureSize.h);
}

void RenderBackground(SDL_Renderer* renderer, int w, int h)
{
	// Background animation
	SDL_SetRenderTarget(renderer, renderedBackground);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, background_images[0], NULL, NULL);

	seashellFrame = ((SDL_GetTicks() / 30) % 300) - 150;
	//printf("%d; %d\n", seashellFrame, max(abs(seashellFrame), seashell.count-1));
	RenderSprite(renderer, &seashell1_pos, &SEASHELL_SPRITE, SEASHELL_SPRITE.count-min(abs(seashellFrame), SEASHELL_SPRITE.count-1)-1);

	{ // Wave
		SDL_Rect Origin = {0, 0, TextureSize.w, 1 };
		SDL_Rect Destination = { 0, 0, TextureSize.w, 1 };
		for (float y = 0.0; y < TextureSize.h; y++)
		{
			Origin.y = y;
			Destination.y = y;
			Destination.x = sin(y/20.0+(float)SDL_GetTicks()/1000.0)*5.0;
			SDL_RenderCopy(renderer, renderedBackground, &Origin, &Destination);
		}
	}

	// Rendering onto screen
	SDL_SetRenderTarget(renderer, NULL);

	FullSize.w = w;
	FullSize.h = w / 16 * 9;
	if (h > FullSize.h) {
		FullSize.w = h / 9 * 16;
		FullSize.h = h;
	}

	FullSize.x = w / 2 - FullSize.w / 2;
	FullSize.y = h / 2 - FullSize.h / 2;

	SDL_RenderCopy(renderer, renderedBackground, NULL, &FullSize);
}

void RenderForeground(SDL_Renderer* renderer, int w, int h)
{
	SDL_RenderCopy(renderer, background_images[1], NULL, &FullSize);
}