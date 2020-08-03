#ifndef POPUP_H
#define POPUP_H

#include "timer.h"
#include "TextManager.h"

struct PopUpData
{
	Uint16 delay;
	Sint16 x,y,speed;
	SDL_Color col;
	Uint16 font;

	PopUpData()
	{
		delay = 0;
		x = 0; y= 0; speed = 0;
		font = 0;
		col = black;
	}

	void Load(rapidxml::xml_node<char> *node)
	{
		if(NodeValid(node))
		{
			LoadXY(x, y, node);
			LoadNum(speed, "speed", node);
			LoadNum(delay, "delay", node);
			LoadNum(font, "font", node);
			LoadColor(col,node);
		}
	}
};

class PopUpHandler
{
	Timer t;
	PopUpData param;

public:
	PopUpHandler(void){ t.stop(); }
	~PopUpHandler(void){}

	void Param(PopUpData p){ param = p; }
	void Param(Sint16 x, Sint16 y, PopUpData p) { param = p; param.x += x; param.y += y; }

	bool InternalEvents();
	void Draw(std::string text);
};

#endif //POPUP_H