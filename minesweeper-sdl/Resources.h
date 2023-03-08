#include "Sprite.h"
void initResources(SDL_Renderer *renderer);
void releaseResources();

typedef SDL_Texture SDL_Texture;
typedef struct _TTF_Font TTF_Font;
typedef struct Mix_Chunk Mix_Chunk;

Sprite sprites;
Sprite seashell;

Sprite particles_sprites[2];
#define BUBBLES_SPRITE particles_sprites[0]
#define EXPLOSION_SPRITE particles_sprites[1]

SDL_Texture* cursor;
SDL_Texture* ui_elements[2];
SDL_Texture* background_images[2];

Mix_Chunk* hitmarker;
Mix_Chunk* bubbles[6];

TTF_Font* font;
TTF_Font* uiFont;