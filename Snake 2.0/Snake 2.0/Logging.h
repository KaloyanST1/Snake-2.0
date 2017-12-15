#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>
#include <string>
#include <SDL.h>
using namespace std;
/*
* Log an SDL error with some error message to the output stream of our choice

*/
void logSDLError(ostream &os, const string &msg) {
	os << msg << " The Error is: " << SDL_GetError() << endl;
}

#endif#pragma once
