#ifndef ITEMCOLLECTION_H
#define ITEMCOLLECTION_H

#include "ItemSlot.h"
#include "menu.h"

//Draw the hovered item's description
struct ItemInfo
{
	Sint16 x, y;
	Uint16 font;
	SDL_Color col;

	ItemInfo()
	{
		x = 0;
		y = 0;
		font = 0;
		col = black;
	}

	void Load(rapidxml::xml_node<char> *node)
	{
		if (NodeValid(node))
		{
			LoadXY(x, y, node);
			LoadNum(font, "font", node);
			LoadColor(col, node);
		}
	}

	void Draw(std::string text)
	{
		gTextManager.Draw(x, y, text, col, font);
	}
};

//The menu is used to handle interactions like the player clicking on an item to equip it
class ItemCollection : public Menu
{
	std::vector<ItemSlot> slot;

	int hover_index;

	//The dimensions of the button
	Sint16 w, h;

public:
	ItemCollection(){ slot.clear(); hover_index = -1; }
	~ItemCollection(){}

	int HoverIndex() { return hover_index; }

	void Draw(ItemInfo &name_info, ItemInfo &desc_info);
	void Load(rapidxml::xml_node<char> *node);
	void LoadItems(rapidxml::xml_node<char> *node);

	bool SwapItem(ItemCollection &collection, int index);
	bool EquipItem(Item item);
	bool HasItem(std::string name);

	//This should be used to preview stats
	void ChangeStat(ItemCollection &ref, int index, GameObject &obj);

	//This should be used to actually calculate stats
	void CalcStat(GameObject &obj);

	void SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root);
};

#endif //ITEMCOLLECTION_H