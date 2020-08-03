#ifndef ITEM_H
#define ITEM_H

#include "AssetManager.h"
#include "object.h"

//Stat bonuses for an item
struct Bonus
{
	StatType type;
	Sint16 val;
	std::string str_val;
};

struct Item
{
	std::string name, img, type, desc;
	std::vector<Bonus> bonus;

	Item(){ Clear(); }
	~Item(){}

	std::string Type() { return type; }
	void Clear();

	void ChangeStat(GameObject &obj, bool increase);

	void Load(rapidxml::xml_node<char> *node);
	void Draw(Sint16 x, Sint16 y) { gAssetManager.DrawTextureCentered(x, y, img); }
	void SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root, const char* rootname);
};

#endif //ITEM_H