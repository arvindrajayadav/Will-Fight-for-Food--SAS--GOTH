#ifndef SCREENSETTINGS_H
#define SCREENSETTINGS_H

#ifdef __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#elif __GNUC__
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#endif

#include "loaders.h"

struct Dimension
{
	Uint16 w, h;
	Dimension() { w = 1280; h = 720; }
};

//Screen attributes
class ScreenSettings
{
	//Bits per pixel and frames per second
	Uint16 bpp, fps;

	//The default dimensions are the dimensions you develop you assets in, current dimension is the screen resolution
	Dimension def, cur, desktop;

	//Are we fullscreen or not
	bool fullscreen;

public:
	ScreenSettings()
	{
		bpp = 32;
		fps = 60;

		fullscreen = false;
	}

	~ScreenSettings() {}
	void Load(rapidxml::xml_node<char> *node, Uint32 &videoflags);

	Uint16 FramesPerSecond() { return fps; }
	Uint16 Width() { return def.w; }
	Uint16 Height() { return def.h; }

	Uint16 CurW() { return cur.w; }
	Uint16 CurH() { return cur.h; }
	Uint16 BitsPerPixel() { return bpp; }

	bool FullScreen() { return fullscreen; }
	void FullScreen(bool val) { fullscreen = val; }

	Dimension Desktop() { return desktop; }
	void Desktop(Uint16 w, Uint16 h) { desktop.w = w; desktop.h = h; }

	bool ValidDimension(Dimension d) { return d.w <= desktop.w && d.h <= desktop.h; }
};

extern ScreenSettings gScreenSettings;

#endif //SCREENSETTINGS_H