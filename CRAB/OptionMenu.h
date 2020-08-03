#ifndef OPTIONMENU_H
#define OPTIONMENU_H

#include "KeyBindMenu.h"
#include "ResolutionMenu.h"
#include "slider.h"

class OptionMenu
{
	std::string bg;
	Slider vol_music, vol_effects;
	bool LoadDone;
	Button fs, save;
	KeyBindMenu keybind;

	//Image of fullscreen button when game is windowed or fullscreen
	ButtonImage img_windowed, img_fullscreen;

	//The notice for restarting game
	HoverInfo restart;

	//The buttons and menus for changing resolution
	ResolutionMenu resolution;

	//The current fullscreen state
	bool fullscreen;

public:
	OptionMenu(void){ LoadDone = false; fullscreen = false; }
	~OptionMenu(void){}

	void Reset() { keybind.Reset(); }

	void Load(rapidxml::xml_node<char> *node);
	void Draw(Button &back);
	bool HandleEvents(Button &back, SDL_Event Event);

	void SaveState();
};

extern OptionMenu option_menu;

#endif //OPTIONMENU_H