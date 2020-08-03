//=============================================================================
// Author:   Arvind
// Purpose:  Menu class
//=============================================================================
#ifndef MENU_H
#define MENU_H

#include <vector>
#include "AssetManager.h"
#include "button.h"

//------------------------------------------------------------------------
// Purpose: The Menu Object
//------------------------------------------------------------------------
class Menu
{
protected:
	//The index of current selected option and highlighted option
	int hoverindex;

	//The horizontal and vertical paths of selection boxes
	std::vector<Uint16> hpath, vpath;

	//Are keyboard buttons enabled?
	bool usekeyboard;

	void NextButton(std::vector<Uint16> &path);
	void PrevButton(std::vector<Uint16> &path);
	void HandleKeyboard(SDLKey Direction);

public:
	//The collection of buttons in the menu
	std::vector<Button> button;

	Menu() { Reset(); usekeyboard = false; }
	~Menu(){}

	void Reset();
	void Draw();
	int HandleEvents(SDL_Event Event);

	void Load(rapidxml::xml_node<char> * node);

	void Visible(Uint16 index, bool val)
	{
		if (index < button.size())
			button[index].Visible(val);
	}

	bool Visible(Uint16 index)
	{
		if (index < button.size())
			return button[index].Visible();
		return false;
	}

	void UseKeyboard(bool val) { usekeyboard = val; }
	int NumberOfButtons() { return button.size(); }
	void Clear() { button.clear(); }
	int HoverIndex() { return hoverindex; }

	void AssignPaths(std::vector<Uint16> &VerticalPath, std::vector<Uint16> &HorizontalPath);
	void AssignPaths();
};

#endif //MENU_H