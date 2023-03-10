#include "Sprite.h"
void initResources(SDL_Renderer *renderer);
void releaseResources();

typedef SDL_Texture SDL_Texture;
typedef struct _TTF_Font TTF_Font;
typedef struct Mix_Chunk Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

Sprite GRID_SPRITES;
Sprite SEASHELL_SPRITE;

Sprite BUBBLES_SPRITE;

Sprite UIBUTTONS_SPRITE;
Sprite UIDIFFICULTY_SPRITE;

Sprite WINBANNER_SPRITE;
Sprite LOSEBANNER_SPRITE;

SDL_Texture* cursor;
SDL_Texture* logo;
SDL_Texture* background_images[2];

Mix_Chunk* hitmarker;
Mix_Chunk* bubbles[6];

Mix_Music* bgm[3];

TTF_Font* font;
TTF_Font* uiFont;