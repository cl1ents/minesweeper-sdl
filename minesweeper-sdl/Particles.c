#include <SDL.h>
#include <SDL_mixer.h>

#include "Particles.h"
#include "Sprite.h"
#include "Resources.h"

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
	particles[index].seed = rand();

	particles[index].x = initial->x;
	particles[index].y = initial->y;
	particles[index].w = initial->w;
	particles[index].h = initial->h;

	particles[index].ix = initial->x;
	particles[index].iy = initial->y;
	particles[index].iw = initial->w;
	particles[index].ih = initial->h;

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
			if (particles[i].step(&particles[i], (time - particles[i].startTime) > particles[i].lifetime, dt) == -1)
			{
				particles[i].active = 0;
			}
			else {
				RenderSprite(renderer, &particles[i], particles[i].sprite, particles[i].frame);
			};
		}
	}
}

int bubble(Particle* p, int last, float dt)
{
	if (last)
	{
		Mix_PlayChannel(-1, bubbles[rand() % 7], 0);
		return -1;
	}

	p->frame = (SDL_GetTicks() - p->startTime) > (p->lifetime - 100);
	if (!p->frame)
	{
		p->y -= dt / 75;
		p->x = p->ix + sin((float)(SDL_GetTicks() - p->startTime + ((float)p->seed) / 2000) / 100) * 20 * (p->seed % 10) / 10.0;
	}

	return 1;
}