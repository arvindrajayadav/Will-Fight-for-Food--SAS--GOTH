#ifndef MODMENU_H
#define MODMENU_H

#include "menu.h"
#include <boost/filesystem.hpp>

class ModMenu
{
	std::string bg;

	//The button data
	ButtonImage img;
	SDL_Rect slot_pos;
	Uint16 font;
	SDL_Color col;
	struct Offset { Sint16 x,y; Offset(){ x = 0; y = 0; } } off_name;

	//The collection of menus
	std::vector<Menu> menu;
	Uint16 curmenu, slot_per_menu;

	//The buttons for cycling between menus
	Button prev, next;

	struct ModDetails
	{
		std::string name, path;
		ModDetails(boost::filesystem::path filepath)
		{
			name = boost::filesystem::basename(filepath);
			path = filepath.string();
		}
	};

	std::vector<ModDetails> mod_list;
public:

	ModMenu(void)
	{
		mod_list.clear();
		font = 0;
		col = black;

		slot_pos.x = 0;
		slot_pos.y = 0;
		slot_pos.w = 0;
		slot_pos.h = 0;

		menu.resize(1);
		curmenu = 0;
		slot_per_menu = 12;
	}
	~ModMenu(void){}

	void ScanDir();
	void Load(rapidxml::xml_node<char> *node);
	void Draw();
	bool HandleEvents(SDL_Event Event);
};

#endif //MODMENU_H