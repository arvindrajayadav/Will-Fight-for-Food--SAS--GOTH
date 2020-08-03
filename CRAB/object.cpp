#include "object.h"

int MAX_OPINION = 100, MIN_OPINION = 0;

GameObject::GameObject(std::string Name, int Op)
{
	name = Name;
	Opinion(Op);
	hint.clear();
	Type(OT_NEUTRAL);
	State(OST_STAND);
}

void GameObject::Load(rapidxml::xml_node<char> *node)
{
	if (NodeValid(node))
	{
		LoadStr(name, "name", node);
		LoadStr(pic_normal, "img", node);
		LoadStr(pic_friendly, "img_f", node);
		LoadStr(pic_hostile, "img_e", node);

		LoadNum(opinion, "opinion", node);
		str_opinion = NumberToString<int>(opinion);

		if (node->first_attribute("type") != NULL)
		{
			std::string t;
			LoadStr(t, "type", node);

			if (t == "hostile") type = OT_HOSTILE;
			else if (t == "coward") type = OT_COWARD;
			else if (t == "immobile") type = OT_IMMOBILE;
			else if (t == "obstacle") type = OT_OBSTACLE;
			else if (t == "prop") type = OT_PROP;
			else type = OT_NEUTRAL;
		}
		else
			type = OT_NEUTRAL;

		rapidxml::xml_node<char> *statnode = node->first_node("stats");
		if (NodeValid(statnode))
		{
			stat[STAT_HEALTH].Load(statnode->first_node("health"));
			stat[STAT_ATTACK].Load(statnode->first_node("attack"));
			stat[STAT_DEFENSE].Load(statnode->first_node("defense"));
			stat[STAT_SPEED].Load(statnode->first_node("speed"));
		}

		hint.clear();
		rapidxml::xml_node<char> *hintnode = node->first_node("hints");
		if (NodeValid(hintnode))
		{
			for (rapidxml::xml_node<char> *n = hintnode->first_node("hint"); n != NULL; n = n->next_sibling("hint"))
				hint.push_back(n->first_attribute("text")->value());
		}

		cp_like.Load(node->first_node("likes"));
		cp_hate.Load(node->first_node("hates"));
	}
}

void GameObject::Opinion(int val)
{
	opinion = val;
	Clamp(opinion, MIN_OPINION, MAX_OPINION);
	str_opinion = NumberToString<int>(opinion);
}

void GameObject::ChangeOpinion(int val)
{
	opinion += val;
	Clamp(opinion, MIN_OPINION, MAX_OPINION);
	str_opinion = NumberToString<int>(opinion);
}

void GameObject::State(GameObjectState newstate)
{
	state = newstate;
}

void GameObject::Type(GameObjectType newtype)
{
	type = newtype;
}

void GameObject::SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root)
{
	rapidxml::xml_node<char> *child_lv1 = doc.allocate_node(rapidxml::node_element, "object");
	child_lv1->append_attribute(doc.allocate_attribute("name", name.c_str()));
	child_lv1->append_attribute(doc.allocate_attribute("opinion", str_opinion.c_str()));

	if (type == OT_HOSTILE)
		child_lv1->append_attribute(doc.allocate_attribute("type", "hostile"));
	else if (type == OT_COWARD)
		child_lv1->append_attribute(doc.allocate_attribute("type", "coward"));
	else if (type == OT_IMMOBILE)
		child_lv1->append_attribute(doc.allocate_attribute("type", "immobile"));
	else if (type == OT_OBSTACLE)
		child_lv1->append_attribute(doc.allocate_attribute("type", "obstacle"));
	else if (type == OT_PROP)
		child_lv1->append_attribute(doc.allocate_attribute("type", "prop"));
	else
		child_lv1->append_attribute(doc.allocate_attribute("type", "neutral"));

	if (state == OST_FIGHT)
		child_lv1->append_attribute(doc.allocate_attribute("state", "fight"));
	else if (state == OST_FLEE)
		child_lv1->append_attribute(doc.allocate_attribute("state", "flee"));
	else if (state == OST_KO)
		child_lv1->append_attribute(doc.allocate_attribute("state", "ko"));
	else
		child_lv1->append_attribute(doc.allocate_attribute("state", "stand"));

	rapidxml::xml_node<char> *child_lv2 = doc.allocate_node(rapidxml::node_element, "stats");

	rapidxml::xml_node<char> *child_lv3h = doc.allocate_node(rapidxml::node_element, "health");
	child_lv3h->append_attribute(doc.allocate_attribute("cur", stat[STAT_HEALTH].val[STVAL_CUR].str.c_str()));
	child_lv3h->append_attribute(doc.allocate_attribute("val", stat[STAT_HEALTH].val[STVAL_FULL].str.c_str()));
	child_lv2->append_node(child_lv3h);

	rapidxml::xml_node<char> *child_lv3a = doc.allocate_node(rapidxml::node_element, "attack");
	child_lv3a->append_attribute(doc.allocate_attribute("cur", stat[STAT_ATTACK].val[STVAL_CUR].str.c_str()));
	child_lv3a->append_attribute(doc.allocate_attribute("val", stat[STAT_ATTACK].val[STVAL_FULL].str.c_str()));
	child_lv2->append_node(child_lv3a);

	rapidxml::xml_node<char> *child_lv3d = doc.allocate_node(rapidxml::node_element, "defense");
	child_lv3d->append_attribute(doc.allocate_attribute("cur", stat[STAT_DEFENSE].val[STVAL_CUR].str.c_str()));
	child_lv3d->append_attribute(doc.allocate_attribute("val", stat[STAT_DEFENSE].val[STVAL_FULL].str.c_str()));
	child_lv2->append_node(child_lv3d);

	rapidxml::xml_node<char> *child_lv3s = doc.allocate_node(rapidxml::node_element, "speed");
	child_lv3s->append_attribute(doc.allocate_attribute("cur", stat[STAT_SPEED].val[STVAL_CUR].str.c_str()));
	child_lv3s->append_attribute(doc.allocate_attribute("val", stat[STAT_SPEED].val[STVAL_FULL].str.c_str()));
	child_lv2->append_node(child_lv3s);

	child_lv1->append_node(child_lv2);
	root->append_node(child_lv1);
}

void GameObject::LoadState(rapidxml::xml_node<char> *node)
{
	name = node->first_attribute("name")->value();

	std::string ops = node->first_attribute("opinion")->value();
	int op = StringToNumber<int>(ops);
	Opinion(op);

	if (node->first_attribute("type") != NULL)
	{
		std::string t = node->first_attribute("type")->value();

		if (t == "hostile") type = OT_HOSTILE;
		else if (t == "coward") type = OT_COWARD;
		else if (t == "immobile") type = OT_IMMOBILE;
		else if (t == "obstacle") type = OT_OBSTACLE;
		else if (t == "prop") type = OT_PROP;
		else type = OT_NEUTRAL;
	}

	if (node->first_attribute("state") != NULL)
	{
		std::string s = node->first_attribute("state")->value();

		if (s == "ko") state = OST_KO;
		else if (s == "fight") state = OST_FIGHT;
		else if (s == "flee") state = OST_FLEE;
		else state = OST_STAND;
	}

	rapidxml::xml_node<char> *statnode = node->first_node("stats");
	if (NodeValid(statnode))
	{
		rapidxml::xml_node<char> *nh = statnode->first_node("health");
		SetStat(STAT_HEALTH, STVAL_FULL, StringToNumber<int>(nh->first_attribute("val")->value()));
		SetStat(STAT_HEALTH, STVAL_CUR, StringToNumber<int>(nh->first_attribute("cur")->value()));

		rapidxml::xml_node<char> *na = statnode->first_node("attack");
		SetStat(STAT_ATTACK, STVAL_FULL, StringToNumber<int>(na->first_attribute("val")->value()));
		SetStat(STAT_ATTACK, STVAL_CUR, StringToNumber<int>(na->first_attribute("cur")->value()));

		rapidxml::xml_node<char> *nd = statnode->first_node("defense");
		SetStat(STAT_DEFENSE, STVAL_FULL, StringToNumber<int>(nd->first_attribute("val")->value()));
		SetStat(STAT_DEFENSE, STVAL_CUR, StringToNumber<int>(nd->first_attribute("cur")->value()));

		rapidxml::xml_node<char> *ns = statnode->first_node("speed");
		SetStat(STAT_SPEED, STVAL_FULL, StringToNumber<int>(ns->first_attribute("val")->value()));
		SetStat(STAT_SPEED, STVAL_CUR, StringToNumber<int>(ns->first_attribute("cur")->value()));
	}
}