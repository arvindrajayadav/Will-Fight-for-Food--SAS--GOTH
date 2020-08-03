#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include "timer.h"
#include "ImageData.h"

class LoadingScreen
{
	//The background image
	ImageData background;

	//The images in the animation
	ImageData anim[4];

	//The current animation frame
	Uint8 frame;

	//The dimensions of loading animation
	float w, h;

	//Time our animation!
	Timer timer;
public:
	LoadingScreen(){ frame = 0; w = 0; h = 0; };
	~LoadingScreen(){};

	void Init();
	void Draw();
	void Quit();
};

extern LoadingScreen gLoadScreen;

#endif //LOADINGSCREEN_H