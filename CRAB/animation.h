#ifndef ANIMATION_H
#define ANIMATION_H

#include "AssetManager.h"
#include "timer.h"

struct AnimationData
{
	Sint16 x,y;
	std::vector<std::string> img;
	Uint16 interval;

	AnimationData()
	{
		x = 0;
		y = 0;
		img.clear();
		interval = 0;
	}

	void Load(rapidxml::xml_node<char> *node);
};

class AnimationHandler
{
	AnimationData data;
	Uint8 cur_image;
	Timer timer;

public:
	AnimationHandler(void)
	{
		cur_image = 0;
		timer.stop();
	}
	~AnimationHandler(void){}

	void Data(AnimationData dat) { data = dat; cur_image = 0; timer.stop(); }

	void Draw();
	void InternalEvents();
};

#endif //ANIMATION_H