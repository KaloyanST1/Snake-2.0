#define main SDL_main
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "MemoryManage.h"
#include "Logging.h"
#include "Recource finder.h"
#include "settings.h"
#include "Controller.h"
#include "Apple.h"

using namespace std;
const int WIN_POS_X = 100;
const int WIN_POS_Y = 100;
const int WIN_W = 720;
const int WIN_H = 480;
const char* WIN_TITLE = "Snake v0.1";


// Loads an image into a texture on the rendering device

SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren) {
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr) {
		logSDLError(cout, "LoadTexture");
	}
	return texture;
}

// Draw the render on the desired position
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

//Keep the texture while Rendering
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}

void renderBackground(SDL_Texture *tex, SDL_Renderer *ren) {
	int x, y = 0;
	for (int i = 0; i < Settings::TILES_NUM; ++i)
	{
		x = (i % Settings::TILES_HOR) * Settings::TILE_SIZE;
		y = (i / Settings::TILES_HOR) * Settings::TILE_SIZE;
		renderTexture(tex, ren, x, y);
	}
}


TTF_Font* loadFont(const string &fontFile, int fontSize) {
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr) {
		logSDLError(cout, "TTF_OpenFont");
		return nullptr;
	}
	return font;
}


SDL_Texture* renderText(const string &message, TTF_Font* font,
	SDL_Color color, SDL_Renderer *renderer)
{
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr) {
		TTF_CloseFont(font);
		logSDLError(cout, "TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr) {
		logSDLError(cout, "CreateTexture");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	return texture;
}

void displayScore(int score, TTF_Font* font,
	SDL_Color color, SDL_Renderer *renderer) {
	const string message = "Score: " + to_string(score);
	SDL_Texture *texture = renderText(message, font, color, renderer);
	renderTexture(texture, renderer, 0, 0);
}



int main(int argc, char const **argv)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(cout, "SDL_Init");
		return 1;
	}

	if (TTF_Init() != 0) {
		logSDLError(cout, "TTF_Init");
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow(WIN_TITLE, WIN_POS_X, WIN_POS_Y, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
	if (win == nullptr)
	{
		logSDLError(cout, "SDL_CreateWindow");
		cleanup(win);
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		logSDLError(cout, "SDL_CreateRenderer");
		cleanup(win, renderer);
		SDL_Quit();
		return 1;
	}

	const string resPath = getResourcePath();

	SDL_Texture *tile_tex = loadTexture(resPath + "tile.png", renderer);
	if (tile_tex == nullptr)
	{
		logSDLError(cout, "loadTexture");
		cleanup(win, renderer, tile_tex);
		return 1;
	}


	SDL_Texture *snake_tex = loadTexture(resPath + "snake.png", renderer);
	if (snake_tex == nullptr)
	{
		logSDLError(cout, "loadTexture");
		cleanup(win, renderer, tile_tex, snake_tex);
		return 1;
	}

	SDL_Texture *apple_tex = loadTexture(resPath + "apple.png", renderer);
	if (apple_tex == nullptr)
	{
		logSDLError(cout, "loadTexture");
		cleanup(win, renderer, tile_tex, snake_tex);
		return 1;
	}

	TTF_Font *font = loadFont(resPath + "ScoreFont.ttf", 12);
	SDL_Color color = { 255, 255, 255, 255 };

	SnakeController snake;
	InitApple apple;

	unsigned int lastTime = 0, currentTime;
	SDL_Event e;
	bool quit = false;
	int currentDirection = Settings::direction::NONE;
	int score = 0;
	while (!quit) {
		
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					quit = true;
					break;
				case SDLK_w:
					if (currentDirection != Settings::direction::DOWN) currentDirection = Settings::direction::UP;
					break;
				case SDLK_s:
					if (currentDirection != Settings::direction::UP) currentDirection = Settings::direction::DOWN;
					break;
				case SDLK_a:
					if (currentDirection != Settings::direction::RIGHT) currentDirection = Settings::direction::LEFT;
					break;
				case SDLK_d:
					if (currentDirection != Settings::direction::LEFT) currentDirection = Settings::direction::RIGHT;
					break;
				default:
					break;
				}
			}
		}

		//Do most checks
		currentTime = SDL_GetTicks();
		if (currentTime > lastTime + 600) {

			quit = snake.moveSnake(currentDirection);

			if (snake.foundApple(apple.pos_x, apple.pos_y))
			{
				snake.addSnakePart();
				apple.placeApple();
				score++;
			}

			SDL_RenderClear(renderer);
			renderBackground(tile_tex, renderer);
			displayScore(score, font, color, renderer);
			snake.renderSnake(snake_tex, renderer, &renderTexture);
			apple.renderApple(apple_tex, renderer, &renderTexture);
			SDL_RenderPresent(renderer);
			lastTime = currentTime;
		}
	}

	cleanup(win, renderer, tile_tex, snake_tex, apple_tex);
	TTF_CloseFont(font);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	cout << "Your score is: " << score << endl;

	return 0;
}