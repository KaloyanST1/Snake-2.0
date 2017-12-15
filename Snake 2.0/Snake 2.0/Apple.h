#ifndef APPLE_H
#define APPLE_H

#include <SDL.h>

class InitApple
{
public:
	InitApple();

	int pos_x;
	int pos_y;

	void placeApple();

	typedef void(*RenderCallback)(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
	void renderApple(SDL_Texture *tex, SDL_Renderer *ren, RenderCallback callback);
};




#endif#pragma once
