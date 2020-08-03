#ifndef INVENTORY_H
#define INVENTORY_H

#include "AssetManager.h"
#include "ItemSlot.h"
#include "Stat.h"
#include "ItemCollection.h"

class Inventory
{
	//The background image and location
	std::string bg;
	Sint16 x, y;
	Button close;

	//Player equipment and pockets for keeping other items
	ItemCollection equip, storage;

	//To draw the player stats
	StatDrawHelper dh_stat[STAT_TOTAL];

	//Draw the hovered item's description
	ItemInfo desc_info, name_info;

	//The stat object use to preview new stats
	GameObject preview;

public:
	Inventory(){ x = 0; y = 0; }
	~Inventory(){}

	void AddItem(Item item);
	bool HasItem(std::string container, std::string name);

	void Load(std::string filename);
	void Draw(GameObject &obj);
	bool HandleEvents(GameObject &obj, SDL_Event Event);

	void LoadState(rapidxml::xml_node<char> *node);
	void SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root);
};

#endif //INVENTORY_H