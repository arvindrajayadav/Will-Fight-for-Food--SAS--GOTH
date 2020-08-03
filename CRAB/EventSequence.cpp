#include "EventSequence.h"

//------------------------------------------------------------------------
// Purpose: Load
//------------------------------------------------------------------------
void EventSequence::Load(std::string filename)
{
	XMLDoc conf(filename);
	if (conf.ready())
	{
		rapidxml::xml_node<char> *node = conf.Doc()->first_node("events");
		for (rapidxml::xml_node<char> *n = node->first_node("event"); n != NULL; n = n->next_sibling("event"))
		{
			GameEvent e(n);
			events.push_back(e);
		}
	}
}

//------------------------------------------------------------------------
// Purpose: Check for events happening
//------------------------------------------------------------------------
void EventSequence::InternalEvents(ObjMap &people, VarMap &varmap, Inventory &inv, std::string &loc, std::string &obj, int CurTrigRect)
{
	for (std::vector<Uint32>::iterator nxe = next_event_id.begin(); nxe != next_event_id.end(); ++nxe)
		if (*nxe < events.size() && events[*nxe].trig.size() > 0)
		{
			int i = 0, size = events[*nxe].trig.size();
			for (; i < size; i++)
				switch (events[*nxe].trig[i].type)
			{
				case TRIG_LOC:
					if (loc != events[*nxe].trig[i].val)
						i = size + 1;
					break;
				case TRIG_OBJ:
					if (obj != events[*nxe].trig[i].val)
						i = size + 1;
					break;
				case TRIG_HEALTH:
				{
					int health = people[events[*nxe].trig[i].val].stat[STAT_HEALTH].val[STVAL_CUR].num;
					int val = StringToNumber<int>(events[*nxe].trig[i].subject);
					if (health > val)
						i = size + 1;
				}
				break;
				case TRIG_OBJ_STATUS:
				{
					GameObjectType ty = OT_NEUTRAL;
					if (events[*nxe].trig[i].val == "neutral")
						ty = OT_NEUTRAL;
					else if (events[*nxe].trig[i].val == "hostile")
						ty = OT_HOSTILE;
					else if (events[*nxe].trig[i].val == "coward")
						ty = OT_COWARD;
					else if (events[*nxe].trig[i].val == "immobile")
						ty = OT_IMMOBILE;
					else if (events[*nxe].trig[i].val == "obstacle")
						ty = OT_OBSTACLE;
					else if (events[*nxe].trig[i].val == "prop")
						ty = OT_PROP;

					if (people[events[*nxe].trig[i].subject].Type() != ty)
						i = size + 1;
				}
				break;
				case TRIG_OBJ_STATE:
				{
					GameObjectState st = OST_STAND;
					if (events[*nxe].trig[i].val == "stand")
						st = OST_STAND;
					else if (events[*nxe].trig[i].val == "fight")
						st = OST_FIGHT;
					else if (events[*nxe].trig[i].val == "flee")
						st = OST_FLEE;
					else
						st = OST_KO;

					if (people[events[*nxe].trig[i].subject].State() != st)
						i = size + 1;
				}
				break;
				case TRIG_ITEM:
					if (inv.HasItem(events[*nxe].trig[i].subject, events[*nxe].trig[i].val) == false)
						i = size + 1;
					break;
				case TRIG_VAR:
					if (events[*nxe].trig[i].subject == "!")
					{
						if (varmap.count(events[*nxe].trig[i].val) != 0)
							i = size + 1;
					}
					else
					{
						if (varmap.count(events[*nxe].trig[i].val) == 0)
							i = size + 1;
					}
					break;
				case TRIG_RECT:
					if (CurTrigRect < 0 || StringToNumber<int>(events[*nxe].trig[i].val) != CurTrigRect)
						i = size + 1;
				default:break;
			}

			if (i == size)
			{
				event_in_progress = true;
				cur_event_id = *nxe;
				break;
			}
		}
		else if (*nxe < events.size() && events[*nxe].trig.size() == 0)
		{
			event_in_progress = true;
			cur_event_id = *nxe;
			break;
		}
}

//------------------------------------------------------------------------
// Purpose: Go to next event
//------------------------------------------------------------------------
bool EventSequence::NextEvent(ObjMap &people, VarMap &varmap, GameObjective &objective, std::vector<EventResult> &result, ISteamUserStats *m_pSteamUserStats, int NextEventChoice)
{
	bool end_seq = false, sync = false;

	event_in_progress = false;
	events[cur_event_id].happened = true;

	//Execute all effects associated with the event
	for (std::vector<Effect>::iterator i = events[cur_event_id].effect.begin(); i != events[cur_event_id].effect.end(); ++i)
	{
		if (i->type < EFF_MOVE)
		{
			switch (i->type)
			{
			case EFF_VAR:
				if (i->subject == "del")
					varmap.erase(i->val);
				else
					varmap[i->val] = true;

				sync = true;
				break;
			case EFF_AIM:
				objective.AddText(i->subject, i->val);
				unread_obj = true;
				break;
			case EFF_OPINION:
				if (people.count(i->subject) > 0)
					people[i->subject].Opinion(StringToNumber<Sint16>(i->val));
				break;
			case EFF_OBJ_STATUS:
				if (people.count(i->subject) > 0)
				{
					if (i->val == "neutral")
						people[i->subject].Type(OT_NEUTRAL);
					else if (i->val == "hostile")
						people[i->subject].Type(OT_HOSTILE);
					else if (i->val == "coward")
						people[i->subject].Type(OT_COWARD);
					else if (i->val == "immobile")
						people[i->subject].Type(OT_IMMOBILE);
					else if (i->val == "obstacle")
						people[i->subject].Type(OT_OBSTACLE);
					else if (i->val == "prop")
						people[i->subject].Type(OT_PROP);
				}
				break;
			case EFF_OBJ_STATE:
				if (people.count(i->subject) > 0)
				{
					if (i->val == "stand")
						people[i->subject].State(OST_STAND);
					else if (i->val == "fight")
						people[i->subject].State(OST_FIGHT);
					else if (i->val == "flee")
						people[i->subject].State(OST_FLEE);
					else
						people[i->subject].State(OST_KO);
				}
				break;
			case EFF_ACHIEVEMENT:
				if (m_pSteamUserStats != nullptr)
				{
					m_pSteamUserStats->SetAchievement(i->val.c_str());
					m_pSteamUserStats->StoreStats();
				}
				break;
			case EFF_END: end_seq = true; break;
			default:break;
			}
		}
		else
		{
			EventResult r;
			switch (i->type)
			{
			case EFF_LEVEL:
			{
				r.type = ER_LEVEL;
				r.val = i->subject;

				int pos = i->val.find(',');
				if (pos > 0)
				{
					std::string xs = i->val.substr(0, pos), ys = i->val.substr(pos + 1, i->val.size() - 1);
					r.x = StringToNumber<Sint16>(xs);
					r.y = StringToNumber<Sint16>(ys);
				}
			}
			break;
			case EFF_HEALTH:
				r.type = ER_HEAL;
				r.val = i->subject;
				if (i->val == "kill")
					r.x = -1;
				else if (i->val == "full")
					r.x = 1;
				else
					r.x = 0;
				break;
			case EFF_MOVE: r.type = ER_MOVE; r.val = i->subject; r.x = StringToNumber<Sint16>(i->val); break;
			case EFF_ITEM: r.type = ER_ITEM; r.val = i->val; unread_inv = true; break;
			case EFF_SAVE: r.type = ER_SAVE; break;
			case EFF_MODE: r.type = ER_MODE; r.val = i->val; break;
			case EFF_QUIT: r.type = ER_QUIT; break;
			default:break;
			}

			result.push_back(r);
		}
	}

	if (sync)
	{
		EventResult r;
		r.type = ER_SYNC;
		result.push_back(r);
	}

	//Go to next event
	if (NextEventChoice != -1)
	{
		cur_event_id = events[cur_event_id].nextid[NextEventChoice];

		next_event_id.clear();
		next_event_id.push_back(cur_event_id);
	}
	else
	{
		next_event_id.clear();

		for (std::vector<Uint32>::iterator i = events[cur_event_id].nextid.begin(); i != events[cur_event_id].nextid.end(); ++i)
			next_event_id.push_back(*i);

		cur_event_id = next_event_id[0];
	}

	return end_seq;
}

//------------------------------------------------------------------------
// Purpose: Save the state of the object
//------------------------------------------------------------------------
void EventSequence::SaveState(rapidxml::xml_document<char> &doc, rapidxml::xml_node<char> *root, const char* name)
{
	rapidxml::xml_node<char> *seqnode = doc.allocate_node(rapidxml::node_element, "set");
	root->append_node(seqnode);

	//Write current event id to node
	str_cur_event_id = NumberToString<Uint32>(cur_event_id);
	seqnode->append_attribute(doc.allocate_attribute("name", name));
	seqnode->append_attribute(doc.allocate_attribute("current", str_cur_event_id.c_str()));

	//Prepare strings of next events and write them
	str_next_event_id.clear();
	for (Uint32 i = 0; i < next_event_id.size(); i++)
	{
		str_next_event_id.push_back(NumberToString<Uint32>(next_event_id[i]));

		rapidxml::xml_node<char> *child = doc.allocate_node(rapidxml::node_element, "next");
		child->value(str_next_event_id[i].c_str());
		seqnode->append_node(child);
	}

	for (std::vector<GameEvent>::iterator e = events.begin(); e != events.end(); ++e)
		if (e->happened)
		{
			rapidxml::xml_node<char> *child = doc.allocate_node(rapidxml::node_element, "event");
			child->value(e->str_id.c_str());
			seqnode->append_node(child);
		}
}

//------------------------------------------------------------------------
// Purpose: Load the state of the object
//------------------------------------------------------------------------
void EventSequence::LoadState(rapidxml::xml_node<char> *node)
{
	rapidxml::xml_attribute<char> *curid = node->first_attribute("current");
	if (curid != NULL)
		cur_event_id = StringToNumber<Uint32>(curid->value());

	next_event_id.clear();
	for (rapidxml::xml_node<char> *n = node->first_node("next"); n != NULL; n = n->next_sibling("next"))
		next_event_id.push_back(StringToNumber<Uint32>(n->value()));

	if (next_event_id.empty())
		next_event_id.push_back(0);

	for (rapidxml::xml_node<char> *n = node->first_node("event"); n != NULL; n = n->next_sibling("event"))
	{
		Uint32 id = StringToNumber<Uint32>(n->value());
		if (id < events.size())
			events[id].happened = true;
	}
}