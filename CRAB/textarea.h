//=============================================================================
// Author:   Arvind
// Purpose:  A box for entering text
//=============================================================================
#ifndef TEXTAREA_H
#define TEXTAREA_H

#include "TextManager.h"
#include "HoverInfo.h"

class TextArea
{
	Sint16 x,y;
	Uint16 max_len;
	std::string text;
	Uint16 font;
	SDL_Color color;
	ImageData img;
	HoverInfo title;

public:
	TextArea(void)
	{
		x = 0;
		y = 0;
		max_len = 7;
		text = "";
		font = 0;
		color = black;
	}
	~TextArea(void){}

	void Load(rapidxml::xml_node<char> *node)
	{
		if(NodeValid(node))
		{
			LoadXY(x, y, node);
			LoadStr(text, "text", node);
			LoadNum(max_len, "size", node);
			LoadNum(font, "font", node);
			LoadColor(color,node);
			title.Load(node->first_node("caption"));
		}
	}

	bool HandleEvents(SDL_Event Event);
	void Draw();

	void Text(std::string str) { text = str; }
	std::string Text() { return text; }

	int X() {return x;}
	void X(int val) { x = val; }

	int Y() {return y;}
	void Y(int val) { y = val; }
};

#endif //TEXTAREA_H