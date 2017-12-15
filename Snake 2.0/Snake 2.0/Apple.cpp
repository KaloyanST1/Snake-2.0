#include "Apple.h"
#include "Settings.h"
#include <stdlib.h>    
#include <time.h> 

InitApple::InitApple() {
	srand(time(NULL));
	placeApple();
}

void InitApple::renderApple(SDL_Texture *tex, SDL_Renderer *ren, RenderCallback callback) {
	int x = (pos_x - 1) * Settings::TILE_SIZE;
	int y = (pos_y - 1) * Settings::TILE_SIZE;
	callback(tex, ren, x, y);
}

void InitApple::placeApple() {
	int pos_x_old = pos_x;
	int pos_y_old = pos_y;
	while (pos_x_old == pos_x && pos_y_old == pos_y) {
		pos_x = rand() % 10 + 1;
		pos_y = rand() % 10 + 1;
	}
}