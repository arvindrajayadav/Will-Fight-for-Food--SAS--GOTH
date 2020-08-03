#ifndef HOVERINFO_H
#define HOVERINFO_H

#include "TextManager.h"

//Tooltip for buttons
class HoverInfo
{
	bool enabled;
	std::string text;
	Sint16 x, y;
	SDL_Color col;
	Uint16 font;
	TextAlign align;

public:
	HoverInfo(void)
	{
		x = 0;
		y = 0;
		col = black;
		font = 0;
		align = ALIGN_LEFT;
		enabled = false;
	}
	~HoverInfo(void){}

	void Init(std::string str, Sint16 X, Sint16 Y, SDL_Color Col, Uint16 Font, TextAlign Align)
	{
		text = str;
		x = X;
		y = Y;
		col = Col;
		font = Font;
		align = Align;
		enabled = true;
	}

	void Load(rapidxml::xml_node<char> *node);
	void Draw();

	std::string Text() { return text; }
	void Text(std::string str) { text = str; }
};

#endif //HOVERINFO_H