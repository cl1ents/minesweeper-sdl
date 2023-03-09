#pragma once
#define MAX_PARTICLES 1024
#include "Sprite.h"

typedef SDL_Texture SDL_Texture;
typedef SDL_Renderer SDL_Renderer;
typedef SDL_Rect SDL_Rect;
typedef Sprite Sprite;

typedef struct Particle {
	struct SDL_Rect;
	int startTime;
	int lifetime;
	int active;

	int ix;
	int iy;
	int iw;
	int ih;

	int index;
	int seed;

	Sprite* sprite;
	int frame;

	int (*step)(void*, int, float);
} Particle;

int particleCount;
Particle particles[MAX_PARTICLES];

void createParticle(SDL_Rect* initial, int lifetime, Sprite* sprite, void *step);
void particleStep(SDL_Renderer* renderer, float dt);

// Particle functions
int bubble(Particle* p, float dt);