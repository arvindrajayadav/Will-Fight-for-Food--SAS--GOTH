#ifndef MAP_H
#define MAP_H

#include "AssetManager.h"
#include "menu.h"

class Map : public Menu
{
	//Background image of the map
	std::string bg;

	//The currently selected location
	std::string curloc;

	//Is any key pressed?
	bool keydown;

	//Have we selected a location in the map, and have we printed the loading message
	bool locpicked, loaddrawn;

	//The ids, images and position of locations
	struct LocInfo
	{
		Sint16 x, y;
		std::string id, img;

		LocInfo() { x = 0; y = 0; }

		void Load(rapidxml::xml_node<char> *node)
		{
			if (NodeValid(node))
			{
				LoadXY(x, y, node);
				LoadStr(id, "id", node);
				LoadStr(img, "img", node);
			}
		}
	};
	std::vector<LocInfo> loc;

	//The positions of loading notification
	struct
	{
		Sint16 x, y;
		Uint16 font;
		SDL_Color col;
		std::string text;

		void Load(rapidxml::xml_node<char> *node)
		{
			if (NodeValid(node))
			{
				LoadXY(x, y, node);
				LoadNum(font, "font", node);
				LoadColor(col, node);
				LoadStr(text, "text", node);
			}
		}

		void Draw()
		{
			gTextManager.Draw(x, y, text, col, font);
		}
	} loading;

public:
	Map(void)
	{
		loc.clear();
		keydown = false;
		locpicked = false;
		loaddrawn = false;
	}
	~Map(void){ loc.clear(); }

	void Load(std::string filename);
	void Draw();
	bool HandleEvents(SDL_Event Event);
	bool InternalEvents();

	std::string CurLocation() { return curloc; }
	void CurLocation(std::string val) { curloc = val; }
};

#endif //MAP_H