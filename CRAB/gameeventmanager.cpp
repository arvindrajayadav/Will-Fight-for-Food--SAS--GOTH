#include "gameeventmanager.h"

void GameEventManager::Init()
{
	event_map.clear();
	event_end.clear();
	active_sequence = "";

	conv_ans.visible = false;
	conv_ans.val = 0;
}

//------------------------------------------------------------------------
// Purpose: Load this
//------------------------------------------------------------------------
void GameEventManager::Load(std::string eventfile, std::string layoutfile, std::string charfile, std::string convfile, std::string sentencefile)
{
	XMLDoc conf(eventfile);
	if (conf.ready())
	{
		rapidxml::xml_node<char> *node = conf.Doc()->first_node("event_list");
		for (rapidxml::xml_node<char> *n = node->first_node("file"); n != NULL; n = n->next_sibling("file"))
		{
			std::string seq_name, seq_path;
			LoadStr(seq_name, "name", n);
			LoadStr(seq_path, "path", n);
			event_map[seq_name].Load(seq_path);
		}
	}

	active_sequence = "";

	conf.Load(layoutfile);
	if (conf.ready())
	{
		rapidxml::xml_node<char> *layout = conf.Doc()->first_node("layout");
		if (NodeValid(layout))
		{
			dlbox.Load(layout->first_node("dialogbox"));
			oh.Load(layout->first_node("character"));
			chan.Load(convfile, sentencefile);
			reply.Load(layout->first_node("reply"));
		}
	}

	conf.Load(charfile);
	if (conf.ready())
	{
		rapidxml::xml_node<char> *node = conf.Doc()->first_node("characters");
		if (NodeValid(node))
		{
			LoadNum(MIN_OPINION, "op_min", node);
			LoadNum(MAX_OPINION, "op_max", node);

			for (rapidxml::xml_node<char> *n = node->first_node("group"); n != NULL; n = n->next_sibling("group"))
				LoadPeople(n->value());
		}
	}

	m_pSteamUserStats = SteamUserStats();
	if (m_pSteamUserStats != nullptr)
		m_pSteamUserStats->RequestCurrentStats();
}

void GameEventManager::LoadPeople(std::string filename)
{
	XMLDoc conf(filename);
	if (conf.ready())
	{
		rapidxml::xml_node<char> *node = conf.Doc()->first_node("people");
		if (NodeValid(node))
		{
			for (rapidxml::xml_node<char> *n = node->first_node(); n != NULL; n = n->next_sibling())
			{
				std::string str;
				LoadStr(str, "name", n);
				people[str].Load(n);
			}
		}
	}
}

//------------------------------------------------------------------------
// Purpose: Handle events
//------------------------------------------------------------------------
void GameEventManager::HandleEvents(GameObjective &objective, SDL_Event Event, std::vector<EventResult> &result)
{
	//If an event is already being performed
	if (event_map.count(active_sequence) > 0 && event_map[active_sequence].EventInProgress())
		switch (event_map[active_sequence].CurrentEvent()->type)
	{
		case EVENT_DIALOG:
		case EVENT_ANIM:
			if (dlbox.HandleEvents(Event))
				ApplyResult(event_map[active_sequence].NextEvent(people, var, objective, result, m_pSteamUserStats));
			break;
		case EVENT_REPLY:
		{
			int choice = reply.HandleEvents(Event);
			if (choice != -1)
				ApplyResult(event_map[active_sequence].NextEvent(people, var, objective, result, m_pSteamUserStats, choice));
		}
		break;
		case EVENT_TALK:
		{
			if (conv_ans.visible && dlbox.HandleEvents(Event))
			{
				conv_ans.visible = false;

				int branch = event_map[active_sequence].CurrentEvent()->con.branch;
				if (branch != -1)
				{
					if (people[event_map[active_sequence].CurrentEvent()->title].Opinion() >= branch)
						ApplyResult(event_map[active_sequence].NextEvent(people, var, objective, result, m_pSteamUserStats, 1));
					else
						ApplyResult(event_map[active_sequence].NextEvent(people, var, objective, result, m_pSteamUserStats, 0));
				}
				else
					ApplyResult(event_map[active_sequence].NextEvent(people, var, objective, result, m_pSteamUserStats, 0));
			}
			else
			{
				int res = 0;
				if (chan.HandleEvents(Event, res))
				{
					conv_ans.visible = true;
					conv_ans.val = chan.GetSelectedReply(res);
					oh.ChangeOpinion(people[event_map[active_sequence].CurrentEvent()->title], res);
				}
			}
		}
		break;
		case EVENT_TEXT:
			if (textin.HandleEvents(Event))
				ApplyResult(event_map[active_sequence].NextEvent(people, var, objective, result, m_pSteamUserStats));
			break;

		case EVENT_SILENT:
		case EVENT_POPUP:
		default: break;
	}
}
//------------------------------------------------------------------------
// Purpose: Internalevent_map
//------------------------------------------------------------------------
void GameEventManager::InternalEvents(std::string object, std::string curloc, GameObjective &objective, Inventory &inv,
	std::vector<EventResult> &result, int &CurTrigRect, Level &level)
{
	if (event_map.count(active_sequence) > 0 && event_map[active_sequence].EventInProgress())
	{
		switch (event_map[active_sequence].CurrentEvent()->type)
		{
		case EVENT_ANIM:
			ah.InternalEvents();
		case EVENT_DIALOG:
			dlbox.InternalEvents();
			break;
		case EVENT_TALK:
			if (conv_ans.visible)
				dlbox.InternalEvents();
			break;
		case EVENT_POPUP:
			if (pop.InternalEvents())
				return ApplyResult(event_map[active_sequence].NextEvent(people, var, objective, result, m_pSteamUserStats));
			break;
		case EVENT_SILENT:
			return ApplyResult(event_map[active_sequence].NextEvent(people, var, objective, result, m_pSteamUserStats));
			break;
		default: break;
		}
	}
	else
	{
		for (EventMap::iterator it = event_map.begin(); it != event_map.end(); ++it)
			it->second.InternalEvents(people, var, inv, curloc, object, CurTrigRect);

		CalcActiveSeq(level);
	}
}
//------------------------------------------------------------------------
// Purpose: Draw
//------------------------------------------------------------------------
void GameEventManager::Draw()
{
	if (event_map.count(active_sequence) > 0 && event_map[active_sequence].EventInProgress())
		switch (event_map[active_sequence].CurrentEvent()->type)
	{
		case EVENT_ANIM:
			ah.Draw();
		case EVENT_DIALOG:
			dlbox.Draw(event_map[active_sequence].CurrentEvent()->dialog, event_map[active_sequence].CurrentEvent()->alt);
			oh.Draw(people[event_map[active_sequence].CurrentEvent()->title], event_map[active_sequence].CurrentEvent()->alt);
			break;
		case EVENT_POPUP:
			pop.Draw(event_map[active_sequence].CurrentEvent()->dialog);
			break;
		case EVENT_REPLY:
			dlbox.Draw(event_map[active_sequence].CurrentEvent()->dialog, false);
			oh.Draw(people[event_map[active_sequence].CurrentEvent()->title]);
			reply.Draw();
			break;
		case EVENT_TALK:
			if (conv_ans.visible)
			{
				dlbox.Draw(event_map[active_sequence].CurrentEvent()->con.reply[conv_ans.val], false);
				oh.Draw(people[event_map[active_sequence].CurrentEvent()->title]);
			}
			else
			{
				chan.DrawBackground();
				dlbox.Draw(event_map[active_sequence].CurrentEvent()->dialog, false);
				oh.Draw(people[event_map[active_sequence].CurrentEvent()->title]);
				chan.Draw(people[event_map[active_sequence].CurrentEvent()->title].hint, event_map[active_sequence].CurrentEvent()->con.context);
			}
			break;
		case EVENT_TEXT:
			dlbox.Draw(event_map[active_sequence].CurrentEvent()->dialog, false);
			oh.Draw(people[event_map[active_sequence].CurrentEvent()->title]);
			textin.Draw();
			break;
		default:
			break;
	}
}

//------------------------------------------------------------------------
// Purpose: Calculate the current sequence in progress
//------------------------------------------------------------------------
void GameEventManager::CalcActiveSeq(Level &level)
{
	active_sequence = "";
	for (EventMap::iterator i = event_map.begin(); i != event_map.end(); ++i)
		if (i->second.EventInProgress())
		{
			active_sequence = i->first;
			switch (event_map[active_sequence].CurrentEvent()->type)
			{
			case EVENT_POPUP:
				if (event_map[active_sequence].CurrentEvent()->alt == false)
					pop.Param(event_map[active_sequence].CurrentEvent()->pop);
				else
				{
					for (std::vector<Sprite>::iterator it = level.objects.begin(); it != level.objects.end(); ++it)
						if (event_map[active_sequence].CurrentEvent()->title == it->Name())
						{
							pop.Param(it->X() - level.Camera().x, it->Y() - level.Camera().y,
								event_map[active_sequence].CurrentEvent()->pop);
							break;
						}
				}
				break;
			case EVENT_ANIM: ah.Data(event_map[active_sequence].CurrentEvent()->anim); break;
			case EVENT_TALK: chan.SetReference(event_map[active_sequence].CurrentEvent()->con,
				people[event_map[active_sequence].CurrentEvent()->title].cp_like,
				people[event_map[active_sequence].CurrentEvent()->title].cp_hate); break;
			case EVENT_REPLY: reply.Cache(event_map[active_sequence].CurrentEvent()->reply); break;
			default:break;
			}
			return;
		}
}

//------------------------------------------------------------------------
// Purpose: Increase health of an object
//------------------------------------------------------------------------
void GameEventManager::Heal(std::string name, Sint16 mod)
{
	if (people.count(name) > 0)
	{
		int heal_val = 0;
		if (mod == 0)
		{
			switch (difficulty)
			{
			case DIFF_EASY: heal_val = people[name].stat[STAT_HEALTH].val[STVAL_FULL].num; break;
			case DIFF_NORMAL: heal_val = people[name].stat[STAT_HEALTH].val[STVAL_FULL].num / 2; break;
			case DIFF_HARD: heal_val = people[name].stat[STAT_HEALTH].val[STVAL_FULL].num / 4; break;
			default:break;
			}
		}
		else if (mod == 1)
			heal_val = people[name].stat[STAT_HEALTH].val[STVAL_FULL].num;
		else
			heal_val = -1 * people[name].stat[STAT_HEALTH].val[STVAL_FULL].num;

		people[name].ChangeStat(STAT_HEALTH, STVAL_CUR, heal_val);
	}
}

//------------------------------------------------------------------------
// Purpose: Save the state of the object
//------------------------------------------------------------------------
void GameEventManager::SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root)
{
	for (std::vector<std::string>::iterator i = event_end.begin(); i != event_end.end(); ++i)
	{
		rapidxml::xml_node<char> *child = doc.allocate_node(rapidxml::node_element, "end");
		child->value(i->c_str());
		root->append_node(child);
	}

	for (EventMap::iterator seq = event_map.begin(); seq != event_map.end(); ++seq)
		seq->second.SaveState(doc, root, seq->first.c_str());

	for (VarMap::iterator v = var.begin(); v != var.end(); ++v)
	{
		rapidxml::xml_node<char> *child = doc.allocate_node(rapidxml::node_element, "var");
		child->value(v->first.c_str());
		root->append_node(child);
	}

	for (ObjMap::iterator obj = people.begin(); obj != people.end(); ++obj)
		obj->second.SaveState(doc, root);
}

//------------------------------------------------------------------------
// Purpose: Load the state of the object
//------------------------------------------------------------------------
void GameEventManager::LoadState(rapidxml::xml_node<char> *node)
{
	for (rapidxml::xml_node<char> *i = node->first_node("end"); i != NULL; i = i->next_sibling("end"))
	{
		event_end.push_back(i->value());
		event_map.erase(i->value());
	}

	for (rapidxml::xml_node<char> *n = node->first_node("set"); n != NULL; n = n->next_sibling("set"))
		if (n->first_attribute("name") != NULL)
		{
			std::string name = n->first_attribute("name")->value();
			if (event_map.count(name) > 0)
				event_map[name].LoadState(n);
		}

	for (rapidxml::xml_node<char> *v = node->first_node("var"); v != NULL; v = v->next_sibling("var"))
		var[v->value()] = true;

	for (rapidxml::xml_node<char> *obj = node->first_node("object"); obj != NULL; obj = obj->next_sibling("object"))
	{
		std::string name = obj->first_attribute("name")->value();
		people[name].LoadState(obj);
	}
}