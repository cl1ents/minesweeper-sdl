#include <SDL.h>

#include "Particles.h"
#include "Sprite.h"

int particleCount;
Particle particles[MAX_PARTICLES];

int findFirstFreeIndex()
{
	for (int i = 0; i < MAX_PARTICLES; i++) {
		if (!particles[i].active) {
			return i;
		}
	}
	return -1;
}

void createParticle(SDL_Rect* initial, int lifetime, Sprite* sprite, void* step) {
	int index = findFirstFreeIndex();

	if (index < 0) return;

	particles[index].index = index;

	particles[index].x = initial->x;
	particles[index].y = initial->y;
	particles[index].w = initial->w;
	particles[index].h = initial->h;

	particles[index].initial = initial;

	particles[index].startTime = SDL_GetTicks();
	particles[index].lifetime = lifetime;
	particles[index].sprite = sprite;
	particles[index].step = step;
	particles[index].frame = 0;

	particles[index].active = 1;

	/*
	struct SDL_Rect;
	int startTime;
	int lifetime;
	int active;

	int index;

	Sprite* sprite;
	int frame;

	int (*step)(void*, float);
	*/
}

void particleStep(SDL_Renderer* renderer, float dt) {
	int time = SDL_GetTicks();
	for (int i = 0; i < MAX_PARTICLES; i++) {
		if (particles[i].active) {
			if (particles[i].step(&particles[i], dt) == -1 || (time - particles[i].startTime) > particles[i].lifetime)
			{
				particles[i].active = 0;
			}
			else {
				RenderSprite(renderer, &particles[i], particles[i].sprite, particles[i].frame);
			};
		}
	}
}