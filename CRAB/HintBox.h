#ifndef HINTBOX_H
#define HINTBOX_H

#include "AssetManager.h"
#include "TextManager.h"

struct HintBox
{
	Sint16 x,y;
	std::string bg;

	struct
	{
		Sint16 x, y, inc;
		Uint16 font;
		SDL_Color col;
	} text;

	HintBox()
	{
		x = 0;
		y = 0;

		text.x = 0;
		text.y = 0;
		text.col = black;
		text.inc = 0;
		text.font = 0;
	}
	void Load(rapidxml::xml_node<char> *node);
	void Draw(std::vector<std::string> &hint);
};

#endif //HINTBOX_H