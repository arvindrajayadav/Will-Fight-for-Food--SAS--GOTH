//=============================================================================
// Author:   Arvind
// Purpose:  The tray where you have inventory, map and objectives icons
//=============================================================================
#ifndef HUD_H
#define HUD_H

#include "button.h"
#include "SlideShow.h"
#include "GameSaveMenu.h"
#include "GameOverMenu.h"
#include "PauseMenu.h"

enum HUDSignal { HS_NONE, HS_OBJ, HS_MAP, HS_PAUSE, HS_INV };

class HUD
{
	//The world map, inventory and objective buttons
	Button map, inv, obj, opt;
	bool keydown;

	//The mode indicator images and location
	struct ModeIndicator
	{
		std::string img[MODE_TOTAL];
		Sint16 x, y;

		ModeIndicator(){ x = 0; y = 0; }

		void Load(rapidxml::xml_node<char> *node)
		{
			if (NodeValid(node))
			{
				LoadStr(img[MODE_RPG], "rpg", node);
				LoadStr(img[MODE_FIGHT], "fight", node);
				LoadXY(x, y, node);
			}
		}
	} mode_img;

	struct NotifyImg
	{
		std::string img;
		Sint16 x, y;

		NotifyImg(){ x = 0; y = 0; }

		void Draw() { gAssetManager.DrawTexture(x, y, img); }

		void Load(rapidxml::xml_node<char> *node)
		{
			if (NodeValid(node))
			{
				LoadStr(img, "img", node);
				LoadXY(x, y, node);
			}
		}
	} notify_inv, notify_obj;

	struct HealthMeter
	{
		Sint16 x, y, bg_x, bg_y;
		Uint16 font;
		SDL_Color col;
		std::string bg;

		HealthMeter() { x = 0; y = 0; bg_x = 0; bg_y = 0; font = 0; col = black; }

		void Load(rapidxml::xml_node<char> *node)
		{
			if (NodeValid(node))
			{
				LoadXY(x, y, node);
				LoadNum(font, "font", node);
				LoadColor(col, node);

				rapidxml::xml_node<char> *bgnode = node->first_node("bg");
				if (NodeValid(bgnode))
				{
					LoadStr(bg, "img", bgnode);
					LoadXY(bg_x, bg_y, bgnode);
				}
			}
		}

		void Draw(int num)
		{
			gAssetManager.DrawTexture(bg_x, bg_y, bg);
			gTextManager.Draw(x, y, NumberToString<int>(num), col, font);
		}
	} health_meter;

public:

	GameOverMenu gom;
	PauseMenu pause;
	Button back;

	HUD(){ keydown = false; }
	~HUD(){}

	void Load(std::string filename);
	void Draw(bool IsAnyoneViolent, int health, bool ShowMap);
	HUDSignal HandleEvents(bool ShowMap, SDL_Event Event);
};

#endif //HUD_H