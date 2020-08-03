#include "Item.h"

//------------------------------------------------------------------------
// Purpose: Load
//------------------------------------------------------------------------
void Item :: Load(rapidxml::xml_node<char> *node)
{
	if(NodeValid(node))
	{
		name = node->first_attribute("name")->value();
		type = node->first_attribute("type")->value();
		img = node->first_attribute("img")->value();
		desc = node->first_attribute("desc")->value();

		bonus.clear();
		for(rapidxml::xml_node<char> *n = node->first_node("bonus"); n != NULL; n = n->next_sibling("bonus"))
		{
			Bonus b;

			std::string type;
			LoadStr(type, "type", n);

			if(type == "health") b.type = STAT_HEALTH;
			else if(type == "attack") b.type = STAT_ATTACK;
			else if(type == "defense") b.type = STAT_DEFENSE;
			else if(type == "speed") b.type = STAT_SPEED;

			LoadStr(b.str_val, "val", n);
			b.val = StringToNumber<Sint16>(b.str_val);

			bonus.push_back(b);
		}
	}
}

void Item :: Clear()
{
	name = "";
	type = "";
	img = "";
	bonus.clear();
}

//------------------------------------------------------------------------
// Purpose: Save and load state
//------------------------------------------------------------------------
void Item :: SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root, const char* rootname)
{
	rapidxml::xml_node<char> *child = doc.allocate_node(rapidxml::node_element, rootname);
	child->append_attribute(doc.allocate_attribute("name",name.c_str()));
	child->append_attribute(doc.allocate_attribute("type", type.c_str()));
	child->append_attribute(doc.allocate_attribute("img", img.c_str()));
	child->append_attribute(doc.allocate_attribute("desc", desc.c_str()));

	for(std::vector<Bonus>::iterator i = bonus.begin(); i != bonus.end(); ++i)
	{
		rapidxml::xml_node<char> *n = doc.allocate_node(rapidxml::node_element, "bonus");
		switch(i->type)
		{
			case STAT_HEALTH:n->append_attribute(doc.allocate_attribute("type", "health")); break;
			case STAT_ATTACK:n->append_attribute(doc.allocate_attribute("type", "attack")); break;
			case STAT_DEFENSE:n->append_attribute(doc.allocate_attribute("type", "defense")); break;
			case STAT_SPEED:n->append_attribute(doc.allocate_attribute("type", "speed")); break;
		}

		n->append_attribute(doc.allocate_attribute("val", i->str_val.c_str()));
		child->append_node(n);
	}

	root->append_node(child);
}

//------------------------------------------------------------------------
// Purpose: Calculate effect of item on stats
//------------------------------------------------------------------------
void Item :: ChangeStat(GameObject &obj, bool increase)
{
	for(std::vector<Bonus>::iterator i = bonus.begin(); i != bonus.end(); ++i)
		if(increase)
		{
			if(i->type == STAT_HEALTH)
				obj.ChangeStat(i->type, STVAL_FULL, i->val);
			else
				obj.ChangeStat(i->type, STVAL_CUR, i->val);
		}
		else
		{
			if(i->type == STAT_HEALTH)
				obj.ChangeStat(i->type, STVAL_FULL, -1*i->val);
			else
				obj.ChangeStat(i->type, STVAL_CUR, -1*i->val);
		}
}