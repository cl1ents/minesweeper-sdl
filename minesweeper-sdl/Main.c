#include <SDL.h>
#include <SDL_ttf.h>
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

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_Point point[5];

		float dt = (SDL_GetTicks() - last) / 1000.0f;
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
		dest.y += 24;
		sprintf_s(buf, 100, "Average FPS: %f", 1000.0f / ((float)totalFrameTicks / totalFrames));
		renderText(buf, dest, renderer);

		// sprintf(buf, "Current Perf: %f", framePerf);

		
		SDL_RenderPresent(renderer);
		last = SDL_GetTicks();
	}

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
	font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);

	SDL_SetWindowTitle(window, "Hello world!");
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