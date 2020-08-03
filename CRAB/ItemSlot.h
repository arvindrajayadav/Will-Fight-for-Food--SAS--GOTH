#ifndef ITEMSLOT_H
#define ITEMSLOT_H

#include "AssetManager.h"
#include "Item.h"
#include "button.h"

class ItemSlot
{
	//The type of items that can be contained in this slot
	std::string type;

	//Is the slot empty?
	bool empty;

	//The item contained in the slot
	Item item;

public:
	ItemSlot(){ type = ""; empty = true; }

	void Load(rapidxml::xml_node<char> *node);
	void Draw(Sint16 x, Sint16 y);
	void SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root);

	void Swap(ItemSlot &slot);

	void Equip(Item i);

	std::string Type() { return type; }
	std::string Description() { return item.desc; }
	std::string Name() { return item.name; }

	bool Empty() { return empty; }
	void Empty(bool val) { empty = val; }

	void ChangeStat(GameObject &obj, bool increase);
};

#endif //ITEMSLOT_H