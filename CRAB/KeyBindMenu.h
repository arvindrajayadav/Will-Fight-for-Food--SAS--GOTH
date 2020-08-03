#ifndef KEYBINDMENU_H
#define KEYBINDMENU_H

#include "AssetManager.h"
#include "menu.h"

class KeyBindMenu
{
	enum States
	{
		STATE_NORMAL,
		STATE_KEY,
		STATE_ALT
	} curstate;

	int selected;

	Menu me_key, me_alt;

	ButtonImage img;
	int img_w, img_h;

	struct Coordinates
	{
		Sint16 x,y,inc;
		Uint16 font;
		SDL_Color col;

		Coordinates()
		{
			x = 0;
			y = 0;
			inc = 0;
			font = 0;
			col = black;
		}

		void Load(rapidxml::xml_node<char> *node)
		{
			if(NodeValid(node))
			{
				LoadXY(x, y, node);
				LoadNum(inc, "inc", node);
				LoadNum(font, "font", node);
				LoadColor(col,node);
			}
		}
	} prim, alt, desc;

	struct PromptInfo
	{
		Uint16 font;
		SDL_Color col;
		std::string text;

		PromptInfo()
		{
			font = 0;
			col = black;
		}

		void Load(rapidxml::xml_node<char> *node)
		{
			if(NodeValid(node))
			{
				LoadStr(text, "text", node);
				LoadNum(font, "font", node);
				LoadColor(col,node);
			}
		}
	} prompt;

public:
	KeyBindMenu(void){ img_w = 100; img_h = 100; Reset(); }
	~KeyBindMenu(void){}

	void Reset()
	{
		curstate = STATE_NORMAL;
		selected = -1;
	}

	void Load(rapidxml::xml_node<char> *node);
	void Draw();
	void HandleEvents(SDL_Event Event);
};

#endif //KEYBINDMENU_H