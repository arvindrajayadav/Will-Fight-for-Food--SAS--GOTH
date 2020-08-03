//=============================================================================
// Author:   Arvind
// Purpose:  Cursor object
//=============================================================================
#ifndef CURSOR_H
#define CURSOR_H

#include "ImageData.h"

class Cursor
{
	Uint16 motion_x, motion_y, button_x, button_y;
	ImageData img;

public:
	Cursor(){ Reset(); }
	~Cursor() { };
	void HandleEvents( SDL_Event Event );
	void Reset()
	{
		motion_x = 0;
		motion_y = 0;

		button_x = 0;
		button_y = 0;
	}
	void Init(std::string filename) { img.Load(filename); }

	Uint16 MotionX() { return motion_x; }
	Uint16 MotionY() { return motion_y; }

	Uint16 ButtonX() { return button_x; }
	Uint16 ButtonY() { return button_y; }
};

extern Cursor gMouse;

#endif //CURSOR_H