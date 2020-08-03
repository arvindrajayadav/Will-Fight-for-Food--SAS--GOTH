#include "ItemSlot.h"

//------------------------------------------------------------------------
// Purpose: Load
//------------------------------------------------------------------------
void ItemSlot :: Load(rapidxml::xml_node<char> *node)
{
	if(NodeValid(node))
		LoadStr(type, "slot", node);
}

void ItemSlot :: SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root)
{
	item.SaveState(doc,root,"item");
}

//------------------------------------------------------------------------
// Purpose: Draw
//------------------------------------------------------------------------
void ItemSlot :: Draw(Sint16 x, Sint16 y)
{
	item.Draw(x,y);
}

void ItemSlot :: Swap(ItemSlot &slot)
{
	if(type == slot.type)
	{
		Item temp = item;
		item = slot.item;
		slot.item = temp;

		bool val = empty;
		empty = slot.empty;
		slot.empty = val;
	}
}

void ItemSlot :: Equip(Item i)
{
	if(type == i.Type())
	{
		item = i;
		Empty(false);
	}
}

void ItemSlot :: ChangeStat(GameObject &obj, bool increase)
{
	item.ChangeStat(obj,increase);
}