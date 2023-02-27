#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	SDL_Window* window = NULL;

	if (0 != SDL_Init(SDL_INIT_EVERYTHING))
		return EXIT_FAILURE;

	window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		640, 480, SDL_WINDOW_SHOWN);
	if (NULL == window)
	{
		fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Delay(3000);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}

/*
int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL initialization failed. SDL Error: %s", SDL_GetError());
	}
	else
	{
		printf("SDL initialization succeeded!");
	}

	(void)getchar();
    return 0;
}
*/