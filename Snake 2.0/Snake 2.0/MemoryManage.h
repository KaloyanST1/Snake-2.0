#pragma once
#ifndef CLEANUP_H
#define CLEANUP_H

#include <SDL.h>
using namespace std;
//Basic memory cleanup 
//One of the reasons i dislike c++

void cleanup(SDL_Window *win) {
	SDL_DestroyWindow(win);
}
void cleanup(SDL_Renderer *ren) {
	SDL_DestroyRenderer(ren);
}
void cleanup(SDL_Texture *tex) {
	SDL_DestroyTexture(tex);
}
void cleanup(SDL_Surface *surf) {
	SDL_FreeSurface(surf);
}

template<typename T, typename... Args>
void cleanup(T *t, Args&&... args) {
	cleanup(t);
	cleanup(forward<Args>(args)...);
}

#endif