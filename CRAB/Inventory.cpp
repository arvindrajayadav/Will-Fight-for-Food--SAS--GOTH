#include "Inventory.h"

//------------------------------------------------------------------------
// Purpose: Load layout
//------------------------------------------------------------------------
void Inventory::Load(std::string filename)
{
	XMLDoc conf(filename);
	if (conf.ready())
	{
		rapidxml::xml_node<char> * node = conf.Doc()->first_node("inventory");
		if (NodeValid(node))
		{
			rapidxml::xml_node<char> *bgnode = node->first_node("bg");
			if (NodeValid(bgnode))
			{
				LoadStr(bg, "img", bgnode);
				LoadXY(x, y, bgnode);
			}

			desc_info.Load(node->first_node("description"));
			name_info.Load(node->first_node("name"));

			close.Load(node->first_node("close"));

			equip.Load(node->first_node("equip"));
			storage.Load(node->first_node("storage"));

			rapidxml::xml_node<char> *statnode = node->first_node("stats");
			if (NodeValid(statnode))
			{
				dh_stat[STAT_HEALTH].Load(statnode->first_node("health"));
				dh_stat[STAT_ATTACK].Load(statnode->first_node("attack"));
				dh_stat[STAT_DEFENSE].Load(statnode->first_node("defense"));
				dh_stat[STAT_SPEED].Load(statnode->first_node("speed"));
			}
		}
	}
}

void Inventory::AddItem(Item item)
{
	storage.EquipItem(item);
}

bool Inventory::HasItem(std::string container, std::string name)
{
	if (container == "equip")
		return equip.HasItem(name);
	else if (container == "storage")
		return storage.HasItem(name);
	else
		return (equip.HasItem(name) || storage.HasItem(name));
}

//------------------------------------------------------------------------
// Purpose: Draw
//------------------------------------------------------------------------
void Inventory::Draw(GameObject &obj)
{
	gAssetManager.DrawTexture(x, y, bg);
	close.Draw();

	dh_stat[STAT_HEALTH].Draw(obj.stat[STAT_HEALTH].val[STVAL_FULL].str);
	dh_stat[STAT_ATTACK].Draw(obj.stat[STAT_ATTACK].val[STVAL_CUR].str);
	dh_stat[STAT_DEFENSE].Draw(obj.stat[STAT_DEFENSE].val[STVAL_CUR].str);
	dh_stat[STAT_SPEED].Draw(obj.stat[STAT_SPEED].val[STVAL_CUR].str);

	if (storage.HoverIndex() != -1)
	{
		preview = obj;
		preview.ResetStat();
		equip.CalcStat(preview);
		storage.ChangeStat(equip, storage.HoverIndex(), preview);

		dh_stat[STAT_HEALTH].preview.SetVal(preview.stat[STAT_HEALTH].val[STVAL_FULL].num);
		dh_stat[STAT_HEALTH].preview.Draw(obj.stat[STAT_HEALTH].val[STVAL_FULL].num);

		dh_stat[STAT_ATTACK].preview.SetVal(preview.stat[STAT_ATTACK].val[STVAL_CUR].num);
		dh_stat[STAT_ATTACK].preview.Draw(obj.stat[STAT_ATTACK].val[STVAL_CUR].num);

		dh_stat[STAT_DEFENSE].preview.SetVal(preview.stat[STAT_DEFENSE].val[STVAL_CUR].num);
		dh_stat[STAT_DEFENSE].preview.Draw(obj.stat[STAT_DEFENSE].val[STVAL_CUR].num);

		dh_stat[STAT_SPEED].preview.SetVal(preview.stat[STAT_SPEED].val[STVAL_CUR].num);
		dh_stat[STAT_SPEED].preview.Draw(obj.stat[STAT_SPEED].val[STVAL_CUR].num);
	}

	storage.Draw(name_info, desc_info);
	equip.Draw(name_info, desc_info);
}

//------------------------------------------------------------------------
// Purpose: Handle events
//------------------------------------------------------------------------
bool Inventory::HandleEvents(GameObject &obj, SDL_Event Event)
{
	if (close.HandleEvents(Event) == BUTTON_LCLICK)
		return true;

	//Do we need to recalculate player stats?
	bool calc = false;

	int index = storage.HandleEvents(Event);
	if (index != -1)
	{
		equip.SwapItem(storage, index);
		calc = true;
	}

	index = equip.HandleEvents(Event);
	if (index != -1)
	{
		storage.SwapItem(equip, index);
		calc = true;
	}

	if (calc)
	{
		obj.ResetStat();
		equip.CalcStat(obj);
		obj.ValidateStat();
		calc = false;
	}

	return false;
}

//------------------------------------------------------------------------
// Purpose: Load and save items
//------------------------------------------------------------------------
void Inventory::LoadState(rapidxml::xml_node<char> *node)
{
	if (NodeValid(node))
	{
		equip.LoadItems(node->first_node("equip"));
		storage.LoadItems(node->first_node("storage"));
	}
}

void Inventory::SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root)
{
	rapidxml::xml_node<char> *eqchild = doc.allocate_node(rapidxml::node_element, "equip");
	equip.SaveState(doc, eqchild);
	root->append_node(eqchild);

	rapidxml::xml_node<char> *stchild = doc.allocate_node(rapidxml::node_element, "storage");
	storage.SaveState(doc, stchild);
	root->append_node(stchild);
}