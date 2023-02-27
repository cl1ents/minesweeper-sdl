#include <SDL.h>
#include <stdio.h>

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