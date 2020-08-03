#ifndef SLIDER_H
#define SLIDER_H

#include "AssetManager.h"
#include "button.h"
#include "Caption.h"

class Slider
{
	//The value of the slider
	int value;

	//The slider bar
	Sint16 x, y;
	Uint16 w, h;

	//The slider knob
	Uint16 kx,ky,kw,kh;

	//The image of the slider background and slider knob
	std::string background, knob, knob_s;

	//Have we grabbed the knob?
	bool grabbed;

	//Caption for the slider
	Caption caption;

public:
	Slider()
	{
		x = 0;
		y = 0;
		w = 0;
		h = 0;

		kx = 0;
		ky = 0;
		kw = 0;
		kh = 0;
		value = 50;

		grabbed = false;
	}
	~Slider(){}

	void Load(rapidxml::xml_node<char> * node, Uint8 val = 100);

	//Return true if volume changed
	bool HandleEvents(SDL_Event Event);
	void Draw();

	int Value() { return value; }
	void Value(int val)
	{
		value = val;
		Clamp(value,0,MIX_MAX_VOLUME);
		kx = x+ (w*value/MIX_MAX_VOLUME);
	}
};

#endif //SLIDER_G