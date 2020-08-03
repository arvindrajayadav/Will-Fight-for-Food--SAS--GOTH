#ifndef CAPTION_H
#define CAPTION_H

#include "TextManager.h"

//Caption for buttons
class Caption
{
	bool enabled;
	std::string text;
	Sint16 x, y;
	SDL_Color col, col_s;
	Uint16 font;
	TextAlign align;

public:
	Caption(void)
	{
		x = 0;
		y = 0;
		col = black;
		col_s = white;
		font = 0;
		align = ALIGN_LEFT;
		enabled = false;
	}
	~Caption(){}

	void Init(std::string str, Sint16 X, Sint16 Y, SDL_Color Col, SDL_Color Col_s, Uint16 Font, TextAlign Align)
	{
		text = str;
		x = X;
		y = Y;
		col = Col;
		col_s = Col_s;
		font = Font;
		align = Align;
		enabled = true;
	}

	void Load(rapidxml::xml_node<char> *node);
	void Draw(bool selected);

	void Draw(Sint16 XOffset, Sint16 YOffset, std::string str, bool selected);
};

#endif //CAPTION_H