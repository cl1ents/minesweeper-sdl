#pragma once

typedef SDL_Renderer SDL_Renderer;
typedef SDL_Rect SDL_Rect;

void initSceneRenderer(SDL_Renderer* renderer);

void RenderForeground(SDL_Renderer* renderer, int w, int h);
void RenderBackground(SDL_Renderer* renderer, int w, int h);