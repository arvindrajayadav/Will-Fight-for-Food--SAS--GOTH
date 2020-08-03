#include "LevelExit.h"

void LevelExit::Load(rapidxml::xml_node<char> *node)
{
	if (NodeValid(node))
	{
		LoadRect(dim, node);
		LoadStr(name, "level", node);

		std::string d;
		LoadStr(d, "dir", node);
		if (d == "left") dir = DIR_LEFT;
		else if (d == "right") dir = DIR_RIGHT;
		else if (d == "up") dir = DIR_UP;
		else if (d == "down") dir = DIR_DOWN;
		else dir = DIR_NONE;

		rapidxml::xml_node<char> *entrynode = node->first_node("entry");
		if (NodeValid(entrynode))
		{
			if (!LoadNum(entry_x, "x", entrynode))
				entry_x = -1;

			if (!LoadNum(entry_y, "y", entrynode))
				entry_y = -1;
		}
	}
}

void LevelExit::Draw(SDL_Rect camera)
{
	if (dir == DIR_LEFT)
		gAssetManager.DrawExit(dim.x + dim.w - camera.x, dim.y + (dim.h / 2) - camera.y, dir);
	else if (dir == DIR_RIGHT)
		gAssetManager.DrawExit(dim.x - (dim.w / 2) - camera.x, dim.y + (dim.h / 2) - camera.y, dir);
	else
		gAssetManager.DrawExit(dim.x + (dim.w / 2) - camera.x, dim.y + (dim.h / 2) - camera.y, dir);
}