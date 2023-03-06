#include <SDL.h>
#include <SDL_image.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Resources.h"
#include "Array.h"
#include "GameGrid.h"
#include "MouseState.h"
#include "Sprite.h"

const SDL_Color GridColors[2][2] = {
	{
		{92, 138, 255, 255},
		{100, 180, 255, 255}
	},
	{
		{25, 71, 188, 255},
		{24, 105, 179, 255}
	}
};

SDL_Texture* gridTex;

SDL_Rect Slot;
SDL_Rect spriteSlice;

void updateGrid(SDL_Renderer* renderer, GameGrid* game)
{
	if (gridTex)
		SDL_DestroyTexture(gridTex);

	gridTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, sprites.w * game->gridSize, sprites.h * game->gridSize);

	Slot.x = 0;
	Slot.y = 0;
	Slot.w = sprites.w;
	Slot.h = sprites.h;

	SDL_SetRenderTarget(renderer, gridTex);

	int a = 0;
	for (int x = 0; x < game->gridSize; x++)
	{
		for (int y = 0; y < game->gridSize; y++) {
			int index = x + y * game->gridSize;
			int display = game->displayGrid->array[index];
			SDL_Color color = GridColors[display == 0 || display == 12][a];
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

			Slot.x = Slot.w * x;
			Slot.y = Slot.h * y;

			//spriteSlice.x = spriteSize * display;

			SDL_RenderFillRect(renderer, &Slot);
			RenderSprite(renderer, &Slot, &sprites, display);
			//SDL_RenderCopy(renderer, sprites, &spriteSlice, &Slot);

			a = !a;
		}
		if (!(game->gridSize % 2))
			a = !a;
	}

	SDL_SetRenderTarget(renderer, NULL);
}

void renderGrid(SDL_Renderer* renderer, SDL_Rect* Place, GameGrid* game, MouseState mouse) 
{ // Grid test
	SDL_RenderCopy(renderer, gridTex, NULL, Place);

	/*
	// int gridSize = game.gridSize;
	int slotSize = Place->h / game->gridSize;

	Slot.x = Place->x;
	Slot.y = Place->x;
	Slot.w = slotSize;
	Slot.h = slotSize;

	//spriteSlice.w = spriteSize;
	//spriteSlice.h = spriteSize;

	int a = 0;
	for (int x = 0; x < game->gridSize; x++)
	{
		for (int y = 0; y < game->gridSize; y++) {
			int index = x + y * game->gridSize;
			int display = game->displayGrid->array[index];
			SDL_Color color = GridColors[display == 0 || display == 12][a];
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

			Slot.x = Place->x + Slot.w * x;
			Slot.y = Place->y + Slot.h * y;

			//spriteSlice.x = spriteSize * display;

			SDL_RenderFillRect(renderer, &Slot);
			RenderSprite(renderer, &Slot, &sprites, display);
			//SDL_RenderCopy(renderer, sprites, &spriteSlice, &Slot);

			a = !a;
		}
		if (!(game->gridSize % 2))
			a = !a;
	}
	*/

	/*
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 50);
	SDL_RenderFillRect(renderer, &Place);
	*/
}