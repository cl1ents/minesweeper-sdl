typedef SDL_Renderer SDL_Renderer;
typedef SDL_Rect SDL_Rect;
typedef GameGrid GameGrid;

void updateGrid(SDL_Renderer* renderer, GameGrid* game);
void renderGrid(SDL_Renderer* renderer, SDL_Rect* Place, GameGrid* game);