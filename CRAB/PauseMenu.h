#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "menu.h"
#include "slider.h"
#include "textarea.h"
#include "GameSaveMenu.h"
#include "OptionMenu.h"
#include "SlideShow.h"

enum PauseSignal { PS_NONE, PS_SAVE, PS_LOAD, PS_QUIT, PS_RESUME };

class PauseMenu
{
	enum PauseState
	{
		STATE_NORMAL,
		STATE_SAVE,
		STATE_OPTION,
		STATE_HELP,
		STATE_LOAD
	} state;

	//The pause menu background
	Sint16 x, y;
	std::string bg;
	
	//The save background
	Sint16 save_x, save_y;
	std::string save_bg;


	TextArea save_prompt;

	Menu menu;
	Button back;

public:
	PauseMenu(void)
	{
		x = 0;
		y = 0; 
		save_x = 0;
		save_y = 0;
		state = STATE_NORMAL;
	}
	~PauseMenu(void){}

	void Load(rapidxml::xml_node<char> *node);
	void Draw();
	PauseSignal HandleEvents(SDL_Event Event);

	std::string SaveFile() { return save_prompt.Text(); }
};

#endif //PAUSEMENU_H