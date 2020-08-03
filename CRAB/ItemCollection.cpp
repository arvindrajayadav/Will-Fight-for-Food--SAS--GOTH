#include "ItemCollection.h"

void ItemCollection::Load(rapidxml::xml_node<char> *node)
{
	slot.clear();
	button.clear();
	for (rapidxml::xml_node<char> *n = node->first_node("button"); n != NULL; n = n->next_sibling("button"))
	{
		Button b;
		b.Load(n);
		button.push_back(b);

		ItemSlot s;
		s.Load(n);
		slot.push_back(s);
	}

	if (!button.empty())
	{
		w = button.at(0).W() / 2;
		h = button.at(0).H() / 2;
	}
}

void ItemCollection::LoadItems(rapidxml::xml_node<char> *node)
{
	Uint16 count = 0;
	for (rapidxml::xml_node<char> *n = node->first_node("item"); n != NULL && count < slot.size(); n = n->next_sibling("item"), ++count)
	{
		Item i;
		i.Load(n);
		slot[count].Equip(i);
	}
}

void ItemCollection::SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root)
{
	for (std::vector<ItemSlot>::iterator i = slot.begin(); i != slot.end(); ++i)
		i->SaveState(doc, root);
}

void ItemCollection::Draw(ItemInfo &name_info, ItemInfo &desc_info)
{
	Menu::Draw();
	hover_index = -1;

	int index = 0;
	for (std::vector<ItemSlot>::iterator i = slot.begin(); i != slot.end(); ++i, ++index)
	{
		i->Draw(button[index].X() + w, button[index].Y() + h);
		if (button[index].Hovered() && !i->Empty())
		{
			name_info.Draw(i->Name());
			desc_info.Draw(i->Description());
			hover_index = index;
		}
	}
}

bool ItemCollection::EquipItem(Item item)
{
	for (std::vector<ItemSlot>::iterator i = slot.begin(); i != slot.end(); ++i)
		if (i->Type() == item.Type() && i->Empty())
		{
			i->Equip(item);
			return true;
		}

	return false;
}

bool ItemCollection::SwapItem(ItemCollection &collection, int index)
{
	//We need to scan the slots first for an empty slot to store the item.
	//If no empty slot is found, then swap with a filled slot of same type
	//If no slot of type is found, don't swap items at all
	int found_index = -1, cur_index = 0;

	for (std::vector<ItemSlot>::iterator i = slot.begin(); i != slot.end(); ++i, ++cur_index)
		if (i->Type() == collection.slot[index].Type())
		{
			if (i->Empty())
			{
				i->Swap(collection.slot[index]);
				return true;
			}
			else
				found_index = cur_index;
		}

	if (found_index != -1)
	{
		slot[found_index].Swap(collection.slot[index]);
		return true;
	}

	return false;
}

void ItemCollection::ChangeStat(ItemCollection &ref, int index, GameObject &obj)
{
	for (std::vector<ItemSlot>::iterator i = ref.slot.begin(); i != ref.slot.end(); ++i)
		if (i->Type() == slot[index].Type())
		{
			i->ChangeStat(obj, false);
			break;
		}

	slot[index].ChangeStat(obj, true);
}

void ItemCollection::CalcStat(GameObject &obj)
{
	for (std::vector<ItemSlot>::iterator i = slot.begin(); i != slot.end(); ++i)
		i->ChangeStat(obj, true);
}

bool ItemCollection::HasItem(std::string name)
{
	for (std::vector<ItemSlot>::iterator i = slot.begin(); i != slot.end(); ++i)
		if (i->Name() == name)
			return true;

	return false;
}