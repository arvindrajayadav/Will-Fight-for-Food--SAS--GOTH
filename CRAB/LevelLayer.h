#ifndef LEVELLAYER_H
#define LEVELLAYER_H

#include "AssetManager.h"
#include "timer.h"

class LevelLayer
{
	enum LayerType { LAYER_IMG, LAYER_TILE };

protected:
	//The position of the layer
	Sint16 x, y;

	//The velocity of the clip rectangle
	Sint16 vx, vy;

	//The clip rectangle
	SDL_Rect clip;

	//Delay between the movement of clip rectangle
	Uint16 delay;
	Timer timer;

	//The image in the layer
	std::string image;

	//Should the layer repeat scrolling or stop?
	bool repeat;

public:
	LevelLayer(void)
	{
		x = 0; y = 0;
		vx = 0; vy = 0;
		timer.stop();
		delay = 0;
		repeat = false;

		clip.x = 0;
		clip.y = 0;
		clip.w = 0;
		clip.h = 0;
	}
	~LevelLayer(void){}

	void Load(rapidxml::xml_node<char> *node);
	void Draw(SDL_Rect camera);
	void InternalEvents();
};

#endif //LEVELLAYER_H