#ifndef LEVELEXIT_H
#define LEVELEXIT_H

#include "AssetManager.h"

struct LevelExit
{
	std::string name;
	SDL_Rect dim;
	Sint16 entry_x, entry_y;

	//We draw a little floaty sprite to signal where the exit is
	Direction dir;

	LevelExit()
	{
		dim.x = 0;
		dim.y = 0;
		dim.w = 0;
		dim.h = 0;

		entry_x = -1;
		entry_y = -1;
		dir = DIR_NONE;
	}

	void Load(rapidxml::xml_node<char> *node);

	void Draw(SDL_Rect camera);
};

#endif //LEVELEXIT_H