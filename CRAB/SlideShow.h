#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include "button.h"

class SlideShow
{
	Sint16 x,y;
	std::vector<std::string> img;
	Button prev, next;
	Uint16 cur_image;
	bool usekeyboard;

public:
	SlideShow()
	{
		x = 0;
		y = 0;
		img.clear();
		cur_image = 0;
		usekeyboard = false;
	}

	void Load(rapidxml::xml_node<char> *node);
	void Draw(Button &back);
	bool HandleEvents(Button &back, SDL_Event Event);
};

extern SlideShow ss_help;

#endif //SLIDESHOW_H