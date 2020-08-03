#ifndef CUSTOMIZEMENU_H
#define CUSTOMIZEMENU_H

#include "menu.h"

class CustomizeMenu
{
	//The collection of menus
	std::vector<Menu> menu;
	Uint16 curmenu, slot_per_menu;

	//The buttons for cycling between menus
	Button prev, next;

	//The title of the menu
	HoverInfo title;

	//The list of all possible player sprites
	typedef std::vector<ImagePaths> SkinPaths;
	std::vector<SkinPaths> img_list;

public:
	CustomizeMenu()
	{
		menu.resize(1);
		curmenu = 0;
		slot_per_menu = 4;
	}

	~CustomizeMenu(){}

	void Load(rapidxml::xml_node<char> *node);
	void Draw();
	bool HandleEvents(SDL_Event Event);
};

#endif //CUSTOMIZEMENU_H