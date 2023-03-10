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
#include "Particles.h"

// CONSTS //

GameGrid game;
SDL_Color white = { 255, 255, 255 };
SDL_Color shadow = { 0, 0, 0, 150 };
int difficulties[] = {
	10, 8, 5, 4
};

int sizes[] = {
	10, 15, 20, 30
};

#ifdef NDEBUG
#define DEBUG 0
#else
#define DEBUG 1
#endif



// FUNCTIONS //

void renderDebugText(char* text, SDL_Rect dest, SDL_Renderer *renderer) {
	if (!DEBUG) return;
	SDL_Color fg = { 0, 0, 0 };
	SDL_Surface* surf = TTF_RenderText_Blended(font, text, fg);

	// printf("Error renderText : %s\n", TTF_GetError());

	dest.w = surf->w;
	dest.h = surf->h;

	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_RenderCopy(renderer, tex, NULL, &dest);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(surf);
}

void renderFontText(TTF_Font *font, char* text, SDL_Color fg, SDL_Rect dest, SDL_Renderer* renderer) {
	SDL_Surface* surf = TTF_RenderText_Blended(font, text, fg);

	dest.w = surf->w;
	dest.h = surf->h;

	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_RenderCopy(renderer, tex, NULL, &dest);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(surf);
}

int CurrentBGM = -1;
void setBGM(int index)
{
	if (index != CurrentBGM || !Mix_PlayingMusic())
		Mix_PlayMusic(bgm[index], 0);

	CurrentBGM = index;
}

int initWindow(SDL_Window** window, SDL_Renderer** renderer)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error SDL_Init : %s\n", SDL_GetError());
		return -1;
	}

	*window = SDL_CreateWindow("Underwater Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
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

	SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);

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

	Mix_Volume(-1, 50);
	Mix_VolumeMusic(30);
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

	int bannerStart = SDL_GetTicks();

	updateGrid(renderer, &game);
	int ingame = 0;
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
				case SDLK_ESCAPE:
					if (ingame)
						ingame = 0;
					else
						running = 0;
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

		if (ingame)
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

			if (game.state == 0 && clicked && SDL_PointInRect(&mousePosition, &Place))
			{
				slotX = (mousePosition.x - Place.x) / slotSize;
				slotY = (mousePosition.y - Place.y) / slotSize;
				slotIndex = slotX + slotY * game.gridSize;

				click.x = slotX;
				click.y = slotY;
				click.flag = flag;

				int cleared = 0;
				switch (handleClick(&game, &click))
				{
				case 1:
					cleared = 1;
					break; // CLEAR
				case 2:
					break; // LOOSE
				}

				onGridClick(&game);
				updateGrid(renderer, &game);
				bannerStart = SDL_GetTicks();
				clicked = 0;
			}

			renderGrid(renderer, &Place, &game);

			Place.h = slotSize;
			Place.w = slotSize;
			Place.y -= slotSize;

			RenderSprite(renderer, &Place, &GRID_SPRITES, 12);

			Place.y += slotSize;
			Place.x += slotSize + 5;

			char buf[100];
			sprintf_s(buf, 100, "Flags left: %d", game.bombCount-game.flagCount);

			Place.y -= 23;
			Place.x += 2;
			renderFontText(uiFont, buf, shadow, Place, renderer);

			Place.y -= 2;
			Place.x -= 2;
			renderFontText(uiFont, buf, white, Place, renderer);

			Place.w = w;
			Place.h = w/2;
			Place.x = 0;
			Place.y = h / 2 - Place.h / 2;

			if (game.state != 0)
			{
				if (clicked)
					ingame = 0;

				int bannerFrame;
				switch (game.state)
				{
				case 1: // Lost
					bannerFrame = SDL_clamp((SDL_GetTicks() - bannerStart) / (1000 / 24) /*24FPS*/, 0, LOSEBANNER_SPRITE.count - 1);
					if (bannerFrame >= LOSEBANNER_SPRITE.count - 2)
						bannerFrame = LOSEBANNER_SPRITE.count - 2 + ((SDL_GetTicks() - bannerStart) / 500) % 2;
					RenderSprite(renderer, &Place, &LOSEBANNER_SPRITE, bannerFrame);
					break;
				case 2: // Win
					bannerFrame = SDL_clamp((SDL_GetTicks() - bannerStart) / (1000 / 24) /*24FPS*/, 0, WINBANNER_SPRITE.count - 1);
					if (bannerFrame >= WINBANNER_SPRITE.count - 2)
						bannerFrame = WINBANNER_SPRITE.count - 2 + ((SDL_GetTicks() - bannerStart) / 500) % 2;
					RenderSprite(renderer, &Place, &WINBANNER_SPRITE, bannerFrame);
					break;
				}
			}

			setBGM(game.state);
		}
		else 
		{
			float smallest = min(w * .9, h * .9);

			SDL_Rect uiBounds = {
				centerX - (smallest / 2.0), centerY - (smallest / 2.0),
				smallest, smallest
			};

			SDL_Rect Place;
			{ // Play button
				Place.h = smallest * .15;
				Place.w = smallest * .5;
				Place.x = uiBounds.x + smallest * .5 - Place.w * .5;
				Place.y = uiBounds.y + smallest * .9 - Place.h;

				//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				//SDL_RenderFillRect(renderer, &Place);

				if (SDL_PointInRect(&mousePosition, &Place))
				{
					RenderSprite(renderer, &Place, &UIBUTTONS_SPRITE, 0 + UIBUTTONS_SPRITE.count/2);
					if (clicked)
					{
						initGrid(&game);
						initGridRenderer(renderer, &game);
						updateGrid(renderer, &game);
						ingame = 1;
					}
				} 
				else
					RenderSprite(renderer, &Place, &UIBUTTONS_SPRITE, 0);
			}

			{ // Difficulties
				Place.h = smallest * .15;
				Place.w = smallest * .15;
				Place.x = uiBounds.x + smallest * .5;
				Place.y = uiBounds.y + smallest * .74 - Place.h;

				Place.x -= Place.w * 2;
				for (int i = 0; i < 4; i++) {
					if (SDL_PointInRect(&mousePosition, &Place) && clicked)
					{
						game.gridSize = sizes[i];
						game.difficulty = difficulties[i];
						game.arraySize = game.gridSize * game.gridSize;
					}

					RenderSprite(renderer, &Place, &UIDIFFICULTY_SPRITE, i + (game.difficulty == difficulties[i]) * (UIDIFFICULTY_SPRITE.count / 2));
					Place.x += Place.w;
				}
			}

			// Logo!
			uiBounds.h = uiBounds.h * .3;
			SDL_RenderCopy(renderer, logo, NULL, &uiBounds);

			setBGM(0);
		}

		RenderForeground(renderer, w, h);

		float dt = (SDL_GetTicks() - last);
		if (dt < 4)
			SDL_Delay(4);


		// Show stats
		Uint32 endTicks = SDL_GetTicks();
		Uint64 endPerf = SDL_GetPerformanceCounter();
		Uint64 framePerf = endPerf - startPerf;
		dt = (endTicks - startTicks);
		totalFrameTicks += endTicks - startTicks;
		particleStep(renderer, dt);

		SDL_Rect dest = { 10, 10, 0, 0 };
		char buf[100];
		sprintf_s(buf, 100, "Current FPS: %f", 1.0f / dt);
		renderDebugText(buf, dest, renderer);
		dest.y += 10;
		sprintf_s(buf, 100, "Average FPS: %f", 1000.0f / ((float)totalFrameTicks / totalFrames));
		renderDebugText(buf, dest, renderer);
		dest.y += 10;
		sprintf_s(buf, 100, "Frametime: %fms", dt);
		renderDebugText(buf, dest, renderer);
		dest.y += 10;
		sprintf_s(buf, 100, "Slot X: %d, Slot Y: %d, Slot index: %d", slotX, slotY, slotIndex);
		renderDebugText(buf, dest, renderer);

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

	game.gridSize = sizes[1];
	game.difficulty = difficulties[1];
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
	initGridRenderer(renderer, &game);
	initSceneRenderer(renderer);

	loop(renderer, window);

	releaseResources();
	TTF_Quit();
	Mix_Quit();
	free(displayGrid.array);
	free(bombGrid.array);
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
