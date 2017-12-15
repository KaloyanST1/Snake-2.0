#ifndef Recource_Finder_H
#define Recource_Finder_H

#include <iostream>
#include <string>
#include <SDL.h>
using namespace std;

// !!! This code is not mine, Full credibility is available to the author - check References for details. Or contact me. !!!

string getResourcePath(const std::string &subDir = "") {

#ifdef _WIN32
	const char PATH_SEP = '\\';
#else
	const char PATH_SEP = '/';
#endif

	static string baseRes;
	if (baseRes.empty()) {
		//SDL_GetBasePath will return NULL if something went wrong in retrieving the path
		char *basePath = SDL_GetBasePath();
		if (basePath) {
			baseRes = basePath;
			SDL_free(basePath);
		}
		else {
			cerr << "Error getting resource path: " << SDL_GetError() << endl;
			return "";
		}
		//We replace the last bin/ with res/ to get the the resource path
		size_t pos = baseRes.find_last_of("bin") - 2;
		baseRes = baseRes.substr(0, pos) + "res" + PATH_SEP;
	}
	//If we want a specific subdirectory path in the resource directory
	//append it to the base path. This would be something like Lessons/res/Lesson0
	return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
}

#endif#pragma once
