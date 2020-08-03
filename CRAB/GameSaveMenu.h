#ifndef GAMESAVEMENU_H
#define GAMESAVEMENU_H

#include "menu.h"
#include "textarea.h"
#include <boost/filesystem.hpp>

struct SaveData
{
	std::string name, loc, diff, last_modified, path;
	SaveData(boost::filesystem::path filepath);
};

class GameSaveMenu
{
	//The buttons for cycling between menus
	Button prev, next;

	//The collection of menus
	std::vector<Menu> menu;
	Uint16 curmenu, slot_per_menu;

	//The images used for menu buttons
	ButtonImage img_slot;
	SDL_Rect slot_pos;

	//This stores the name of the save slot
	TextArea ta_name;

	//The save information for each slot
	std::vector<SaveData> slot_info;
	std::string sel_save;
	Caption slot_name, slot_loc, slot_diff, slot_time;

	//Get the index of the hovered element in the menu
	const int HoverIndex()
	{
		if (menu.at(curmenu).HoverIndex() >= 0)
			return (curmenu * slot_per_menu) + menu.at(curmenu).HoverIndex();

		return -1;
	}

public:
	GameSaveMenu()
	{
		slot_pos.x = 0;
		slot_pos.y = 0;
		slot_pos.w = 0;
		slot_pos.h = 0;

		menu.resize(1);
		curmenu = 0;
		slot_per_menu = 12;
	}
	~GameSaveMenu(){}

	bool SelectNewestFile()
	{
		if (slot_info.size() > 0)
		{
			sel_save = slot_info[0].path;
			return true;
		}

		return false;
	}

	bool Empty() { ScanDir(); return slot_info.empty(); }

	void Reset()
	{
		for (std::vector<Menu>::iterator m = menu.begin(); m != menu.end(); ++m)
			m->Reset();
	}
	void ScanDir();
	std::string SelectedSave() { return sel_save; }

	void Load(rapidxml::xml_node<char> *node);
	bool HandleEvents(SDL_Event Event);
	void Draw();
};

extern GameSaveMenu gSaveMenu;

#endif //GAMESAVEMENU_H