#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

int setWindowColor(SDL_Renderer* renderer, SDL_Color color)
{
	if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
		return -1;
	if (SDL_RenderClear(renderer) < 0)
		return -1;
	return 0;
}

TTF_Font* font;
void renderText(char* text, SDL_Rect dest, SDL_Renderer *renderer) {
	SDL_Color fg = { 0, 0, 0 };
	SDL_Surface* surf = TTF_RenderText_Solid(font, text, fg);

	printf("%s", TTF_GetError());

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
		fprintf(stderr, "Error SDL_Init : %s", SDL_GetError());
		return -1;
	}

	SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_SHOWN, window, renderer);
	if (window == NULL || *window == NULL)
	{
		fprintf(stderr, "Error SDL_CreateWindow : %s", SDL_GetError());
		return -1;
	}

	if (renderer == NULL || *renderer == NULL)
	{
		fprintf(stderr, "Error SDL_CreateWindow : %s", SDL_GetError());
		return -1;
	}

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
	while (running) {
		totalFrames++;
		Uint32 startTicks = SDL_GetTicks();
		Uint64 startPerf = SDL_GetPerformanceCounter();

		SDL_Event e;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		// Event loop
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case SDL_QUIT:
				running = 0;
				break;
				/*case SDL_MOUSEBUTTONDOWN:
					square s;
					s.x = e.button.x;
					s.y = e.button.y;
					s.w = rand() % 50 + 25;
					s.h = rand() % 50 + 25;
					s.yvelocity = -500;
					s.xvelocity = rand() % 500 - 250;
					s.lastUpdate = SDL_GetTicks();
					s.born = SDL_GetTicks();
					squares.push_back(s);
					break;*/
			}
		}


		int w;
		int h;

		SDL_GetWindowSize(window, &w, &h);

		int centerX = w / 2;
		int centerY = h / 2;

		{ // Animation Test

			int smallest = min(w * .2, h * .2);

			int rw = smallest;
			int rh = smallest;

			int frame = (SDL_GetTicks() % (frameDuration * frameCount)) / frameDuration;

			SDL_Rect rec = { 2, 2, rw, rh };
			SDL_Rect trans = { 0, frame * 256, 256, 256 };

			//for (int x = 0; x < 10; );

			//SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(renderer, &rec);
			SDL_RenderCopy(renderer, transplosion, &trans, &rec);
		};

		{ // Grid test
			int gridSize = 20;
			int smallest = min(w * .8, h * .8);
			SDL_Rect Place = { 
				centerX-(smallest/2), centerY - (smallest / 2),
				smallest, smallest
			};

			int slotSize = Place.w / gridSize;

			SDL_Rect Slot;
			Slot.x = Place.x;
			Slot.y = Place.y;
			Slot.w = slotSize;
			Slot.h = slotSize;

			int a = 0;
			for (int x = 0; x < gridSize; x++)
			{
				a = !a;
				for (int y = 0; y < gridSize; y++) {
					a = !a;
					SDL_SetRenderDrawColor(renderer, a ? 255 : 0, a ? 0 : 255, 0, 255);

					Slot.x = Place.x + slotSize * x;
					Slot.y = Place.y + slotSize * y;

					SDL_RenderFillRect(renderer, &Slot);
				}
			}
				
		};

		/*
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_Point point[5];

		
		float offsety = sin(SDL_GetTicks() / 1000.0f) * 50;
		float offsetx = cos(SDL_GetTicks() / 1000.0f) * 50;

		point[0].x = 100 + offsetx;
		point[0].y = 100 + offsety;
		point[1].x = 200 + offsetx;
		point[1].y = 100 + offsety;
		point[2].x = 200 + offsetx;
		point[2].y = 200 + offsety;
		point[3].x = 100 + offsetx;
		point[3].y = 200 + offsety;
		point[4].x = 100 + offsetx;
		point[4].y = 100 + offsety;
		SDL_RenderDrawLines(renderer, point, 5);
		*/

		float dt = (SDL_GetTicks() - last) / 1000.0f;
		if (dt < 6)
			SDL_Delay(6);

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

		// sprintf(buf, "Current Perf: %f", framePerf);

		
		SDL_RenderPresent(renderer);
		last = SDL_GetTicks();
	}
	SDL_DestroyTexture(transplosion);
	return 0;
}

int main(int argc, char* argv[])
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	int status = EXIT_FAILURE;

	SDL_SetMainReady();
	if (initWindow(&window, &renderer) != 0)
	{
		fprintf(stderr, "Error initWindow : %s", SDL_GetError());
		goto Quit;
	}

	TTF_Init();
	font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 10);

	SDL_SetWindowTitle(window, "Hello world!");
	SDL_SetWindowResizable(window, SDL_TRUE);
	setWindowColor(renderer, (SDL_Color){ 255, 255, 255, 255 });

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

	loop(renderer, window);

	//SDL_RenderPresent(renderer);
	//SDL_Delay(3000);
	SDL_DestroyWindow(window);
	Quit:
		if (renderer != NULL)
			SDL_DestroyRenderer(renderer);
		if (window != NULL)
			SDL_DestroyWindow(window);
		SDL_Quit();
		return status;


	return EXIT_SUCCESS;
}