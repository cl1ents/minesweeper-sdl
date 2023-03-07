#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "Resources.h"
#include "GameGrid.h"
#include "GridRenderer.h"
#include "Input.h"
#include "Array.h"
#include "MouseState.h"
#include "SceneRenderer.h"

// CONSTS //

GameGrid game;

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

	*window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	//SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_SHOWN, window, renderer);
	
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

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512) == -1)
	{
		return -1;
	}

	//Initialize SDL_mixer
	if (Mix_AllocateChannels(256) == -1)
	{
		return -1;
	}

	TTF_Init();

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
	SDL_Texture* transplosion = IMG_LoadTexture(renderer, "./res/images/transplosion.png");
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

	int fullscreen = 0;
	int toPlay = 0;

	updateGrid(renderer, &game);
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

			case SDL_WINDOWEVENT:
				switch (e.window.event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED:
				case SDL_WINDOWEVENT_RESIZED:
					SDL_GetWindowSize(window, &w, &h);
					break;
				}
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_RETURN:
					resetGrid(&game);
					initGridRenderer(&game);
					updateGrid(renderer, &game);
					break;
				case SDLK_F11:
					fullscreen = !fullscreen;
					SDL_SetWindowFullscreen(window, fullscreen*4097);
					updateGrid(renderer, &game);
					break;
				case SDLK_x:
					Mix_PlayChannel(-1, hitmarker, 0);
					break;
				case SDLK_w:
					Mix_PlayChannel(-1, hitmarker, 0);
					break;
				}
				break;
			};
		}

		SDL_GetMouseState(&cursorRect.x, &cursorRect.y);
		mousePosition.x = cursorRect.x;
		mousePosition.y = cursorRect.y;

		float centerX = w / 2.0;
		float centerY = h / 2.0;

		RenderBackground(renderer, w, h);

		{
			int smallest = min(w * .8, h * .8);

			int slotSize = smallest / game.gridSize;
			int correctSize = slotSize * game.gridSize;

			SDL_Rect Place = {
				centerX - (correctSize / 2), centerY - (correctSize / 2),
				correctSize, correctSize
			};

			MouseState State = {
				mousePosition.x, mousePosition.y, 0
			};


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

				toPlay += onGridClick(&game);
				updateGrid(renderer, &game);
			}

			renderGrid(renderer, &Place, &game);
		}

		if (toPlay > 0 && (rand() % 3) == 0) {
			Mix_PlayChannel(-1, bubbles[rand() % 7], 0);
			toPlay -= rand() % 15;
			toPlay = max(toPlay, 0);
		}

		RenderForeground(renderer, w, h);

		float dt = (SDL_GetTicks() - last) / 1000.0f;
		if (dt < 4)
			SDL_Delay(4);

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

		//RenderSprite(renderer, &cursorRect, &seashell, (SDL_GetTicks() / 30) % seashell.count);

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
	initGridRenderer(&game);

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
	releaseResources();
	SDL_DestroyWindow(window);

	Quit:
		free(displayGrid.array);
		free(bombGrid.array);
		if (renderer != NULL)
			SDL_DestroyRenderer(renderer);
		if (window != NULL)
			SDL_DestroyWindow(window);
		Mix_Quit();
		SDL_Quit();
		return status;

	return EXIT_SUCCESS;
}