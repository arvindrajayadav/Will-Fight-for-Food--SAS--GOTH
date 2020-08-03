#ifndef APP_H
#define APP_H

#include <time.h>
#include "gamestates.h"
#include "AssetManager.h"
#include "MusicManager.h"
#include "timer.h"
#include "splash.h"
#include "mainmenu.h"
#include "game.h"
#include <steam/steam_api.h>

#ifdef __APPLE__
#import <mach-o/dyld.h>
#endif

class App
{
	SDL_Surface *screen;

public:
	App(void)
	{
		screen = NULL;

#ifdef __APPLE__
		//Set working path to resources directory
		char path[1024];
		uint32_t size = sizeof(path);
		if (_NSGetExecutablePath(path, &size) == 0)
			printf("executable path is %s\n", path);
		else
			printf("buffer too small; need size %u\n", size);

		unsigned found = std::string(path).find_last_of("/");
		std::string final = std::string(path).substr(0,found);
		found = final.find_last_of("/");
		final = std::string(path).substr(0,found) + std::string("/Resources/");
		chdir(final.c_str());
#endif
	}
	~App(void);

	bool Init();
	void Run();
};

#endif //APP_H