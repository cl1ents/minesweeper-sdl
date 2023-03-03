#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "Resources.h"
#include "GameGrid.h"
#include "Input.h"
#include "Array.h"
#include "MouseState.h"

// CONSTS //

GameGrid game;
const SDL_Color SlotColors[] = {
	{128, 128, 128, 255},
	{192, 192, 192, 255},
	{0, 0, 255, 255},
	{0, 255, 0, 255},
	{255, 0, 0, 255},
	{0, 0, 128, 255},
	{0, 129, 127, 255},
	{0, 0, 0, 255},
	{50, 50, 50, 255},
	{255, 255, 255, 255},
	{255, 255, 255, 255},
	{0, 0, 0, 255},
	{50, 0, 0, 255}
};


// FUNCTIONS //

void renderText(char* text, SDL_Rect dest, SDL_Renderer *renderer) {
	SDL_Color fg = { 0, 0, 0 };
	SDL_Surface* surf = TTF_RenderText_Solid(font, text, fg);

	// printf("Error renderText : %s\n", TTF_GetError());

	dest.w = surf->w;
	dest.h = surf->h;

	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_RenderCopy(renderer, tex, NULL, &dest);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(surf);
}

int initWindow(SDL_Window** window, SDL_Renderer** renderer)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error SDL_Init : %s\n", SDL_GetError());
		return -1;
	}

	SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_SHOWN, window, renderer);
	if (window == NULL || *window == NULL)
	{
		fprintf(stderr, "Error SDL_CreateWindow : %s\n", SDL_GetError());
		return -1;
	}

	if (renderer == NULL || *renderer == NULL)
	{
		fprintf(stderr, "Error SDL_CreateWindow : %s\n", SDL_GetError());
		return -1;
	}

	TTF_Init();
	font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 10);

	SDL_SetWindowTitle(*window, "Hello world!");
	SDL_SetWindowResizable(*window, SDL_TRUE);
	SDL_ShowCursor(0);

	return 0;
}

int loop(SDL_Renderer* renderer, SDL_Window* window)
{

	int running = 1;
	Uint32 totalFrameTicks = 0;
	Uint32 totalFrames = 0;

	Uint32 last = SDL_GetTicks();
	SDL_Texture* transplosion = IMG_LoadTexture(renderer, "./res/transplosion.png");
	int frameCount = 18;
	int frameDuration = 70;
	int moves = 0;

	SDL_Rect cursorRect;
	SDL_Point mousePosition;
	SDL_QueryTexture(cursor, NULL, NULL, &cursorRect.w, &cursorRect.h);

	int w;
	int h;

	SDL_GetWindowSize(window, &w, &h);

	Input click;
	int slotX = -1;
	int slotY = -1;
	int slotIndex = -1;

	while (running) {
		totalFrames++;
		Uint32 startTicks = SDL_GetTicks();
		Uint64 startPerf = SDL_GetPerformanceCounter();

		SDL_Event e;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		int clicked = 0;
		int flag = 0;

		// Event loop
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case SDL_QUIT:
				running = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				clicked = 1;
				flag = e.button.button == SDL_BUTTON_RIGHT;
				break;
			case SDL_WINDOWEVENT_RESIZED:
				SDL_GetWindowSize(window, &w, &h);
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_RETURN)
					resetGrid(&game);
				break;
			};
		}

		SDL_GetMouseState(&cursorRect.x, &cursorRect.y);
		mousePosition.x = cursorRect.x;
		mousePosition.y = cursorRect.y;

		float centerX = w / 2.0;
		float centerY = h / 2.0;
		SDL_GetWindowSize(window, &w, &h);

		/*
		{ // Animation Test

			int smallest = min(w * .2, h * .2);

			int rw = smallest;
			int rh = smallest;

			int frame = (SDL_GetTicks() % (frameDuration * frameCount)) / frameDuration;

			SDL_Rect rec = { mousePosition.x - rw / 2, mousePosition.y - rh/2, rw, rh };
			SDL_Rect trans = { 0, frame * 256, 256, 256 };

			//for (int x = 0; x < 10; );

			//SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(renderer, &rec);
			SDL_RenderCopy(renderer, transplosion, &trans, &rec);
		};
		*/

		{
			int smallest = min(w * .9, h * .9);

			int slotSize = smallest / game.gridSize;
			int correctSize = slotSize * game.gridSize;

			SDL_Rect Place = {
				centerX - (correctSize / 2), centerY - (correctSize / 2),
				correctSize, correctSize
			};

			MouseState State = {
				mousePosition.x, mousePosition.y, 0
			};

			renderGrid(renderer, &Place, &game, &State);

			if (SDL_PointInRect(&mousePosition, &Place) && clicked)
			{
				slotX = (mousePosition.x - Place.x) / slotSize;
				slotY = (mousePosition.y - Place.y) / slotSize;
				slotIndex = slotX + slotY * game.gridSize;

				click.x = slotX;
				click.y = slotY;
				click.flag = flag;

				if (handleClick(&game, &click))
					completeGrid(&game);
			}
		}


		float dt = (SDL_GetTicks() - last) / 1000.0f;
		if (dt < 6)
			SDL_Delay(6);

		// Show stats
		Uint32 endTicks = SDL_GetTicks();
		Uint64 endPerf = SDL_GetPerformanceCounter();
		Uint64 framePerf = endPerf - startPerf;
		float frameTime = (endTicks - startTicks) / 1000.0f;
		totalFrameTicks += endTicks - startTicks;

		SDL_Rect dest = { 10, 10, 0, 0 };
		char buf[100];
		sprintf_s(buf, 100, "Current FPS: %f", 1.0f / frameTime);
		renderText(buf, dest, renderer);
		dest.y += 10;
		sprintf_s(buf, 100, "Average FPS: %f", 1000.0f / ((float)totalFrameTicks / totalFrames));
		renderText(buf, dest, renderer);
		dest.y += 10;
		sprintf_s(buf, 100, "Frametime: %fms", frameTime * 1000);
		renderText(buf, dest, renderer);
		dest.y += 10;
		sprintf_s(buf, 100, "Slot X: %d, Slot Y: %d, Slot index: %d", slotX, slotY, slotIndex);
		renderText(buf, dest, renderer);

		SDL_RenderCopy(renderer, cursor, NULL, &cursorRect);
		SDL_RenderPresent(renderer);
		last = SDL_GetTicks();
	}
	SDL_DestroyTexture(transplosion);
	return 0;
}

// MAIN //

int main(int argc, char* argv[])
{
	// Initialize resources

	// Setup game

	Array bombGrid;
	game.bombGrid = &bombGrid;
	Array displayGrid;
	game.displayGrid = &displayGrid;

	game.gridSize = 20;
	game.difficulty = 8;
	game.arraySize = game.gridSize * game.gridSize;

	initArraySize(game.displayGrid, game.arraySize);
	initArraySize(game.bombGrid, game.arraySize);

	initGrid(&game);

	// Other

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	int status = EXIT_FAILURE;

	SDL_SetMainReady();
	if (initWindow(&window, &renderer) != 0)
	{
		fprintf(stderr, "Error initWindow : %s\n", SDL_GetError());
		goto Quit;
	}

	initResources(renderer);
	loop(renderer, window);
	
	SDL_DestroyWindow(window);

	Quit:
		free(displayGrid.array);
		free(bombGrid.array);
		if (renderer != NULL)
			SDL_DestroyRenderer(renderer);
		if (window != NULL)
			SDL_DestroyWindow(window);
		SDL_Quit();
		return status;


	return EXIT_SUCCESS;
}