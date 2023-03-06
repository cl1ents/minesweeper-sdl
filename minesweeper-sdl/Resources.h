#include "Sprite.h"
void initResources(SDL_Renderer *renderer);
void releaseResources();

typedef SDL_Texture SDL_Texture;
typedef struct _TTF_Font TTF_Font;

SDL_Texture* spritesTex;
Sprite sprites;

SDL_Texture* seashellTex;
Sprite seashell;

SDL_Texture* cursor;
SDL_Texture** ui_elements;
SDL_Texture** background_elements;

TTF_Font* font;