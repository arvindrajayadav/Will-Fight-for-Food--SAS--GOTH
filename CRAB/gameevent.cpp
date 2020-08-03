#include "gameevent.h"

void GameEvent::Load(rapidxml::xml_node<char> *node)
{
	if (NodeValid(node))
	{
		happened = false;

		LoadStr(str_id, "id", node);
		id = StringToNumber<Uint32>(str_id);

		LoadStr(title, "title", node);
		LoadStr(dialog, "dialog", node);

		if (node->first_attribute("alt") == NULL)
			alt = false;
		else
			alt = true;

		std::string Type;
		LoadStr(Type, "type", node);

		if (Type == "reply")
		{
			type = EVENT_REPLY;
			reply.clear();
			for (rapidxml::xml_node<char> *rep = node->first_node("reply"); rep != NULL; rep = rep->next_sibling("reply"))
			{
				std::string str;
				LoadStr(str, "text", rep);
				reply.push_back(str);
			}
		}
		else if (Type == "talk")
		{
			type = EVENT_TALK;
			con.Reset();
			con.Load(node->first_node("talk"));
		}
		else if (Type == "animation")
		{
			type = EVENT_ANIM;
			anim.Load(node->first_node("animation"));
		}
		else if (Type == "pop")
		{
			type = EVENT_POPUP;
			pop.Load(node->first_node("popup"));
		}
		else if (Type == "silent") type = EVENT_SILENT;
		else if (Type == "text") type = EVENT_TEXT;
		else type = EVENT_DIALOG;

		trig.clear();
		for (rapidxml::xml_node<char> *it = node->first_node("trigger"); it != NULL; it = it->next_sibling("trigger"))
		{
			Trigger t;

			std::string ty;
			LoadStr(ty, "type", it);

			t.type = TRIG_VAR;
			if (ty == "loc") t.type = TRIG_LOC;
			else if (ty == "obj") t.type = TRIG_OBJ;
			else if (ty == "health") t.type = TRIG_HEALTH;
			else if (ty == "status") t.type = TRIG_OBJ_STATUS;
			else if (ty == "state") t.type = TRIG_OBJ_STATE;
			else if (ty == "item") t.type = TRIG_ITEM;
			else if (ty == "rect") t.type = TRIG_RECT;

			LoadStr(t.subject, "subject", it);
			LoadStr(t.val, "val", it);
			trig.push_back(t);
		}

		nextid.clear();
		for (rapidxml::xml_node<char> *i = node->first_node("next"); i != NULL; i = i->next_sibling("next"))
			if (i->first_attribute("id") != NULL)
				nextid.push_back(StringToNumber<Uint32>(i->first_attribute("id")->value()));

		effect.clear();
		for (rapidxml::xml_node<char> *it = node->first_node("effect"); it != NULL; it = it->next_sibling("effect"))
		{
			Effect e;

			std::string ty;
			LoadStr(ty, "type", it);

			if (ty == "aim") e.type = EFF_AIM;
			else if (ty == "opinion") e.type = EFF_OPINION;
			else if (ty == "status") e.type = EFF_OBJ_STATUS;
			else if (ty == "state") e.type = EFF_OBJ_STATE;
			else if (ty == "achievement") e.type = EFF_ACHIEVEMENT;
			else if (ty == "end") e.type = EFF_END;
			else if (ty == "move") e.type = EFF_MOVE;
			else if (ty == "level") e.type = EFF_LEVEL;
			else if (ty == "health") e.type = EFF_HEALTH;
			else if (ty == "item") e.type = EFF_ITEM;
			else if (ty == "save") e.type = EFF_SAVE;
			else if (ty == "mode") e.type = EFF_MODE;
			else if (ty == "quit") e.type = EFF_QUIT;
			else e.type = EFF_VAR;

			LoadStr(e.subject, "subject", it);
			LoadStr(e.val, "val", it);

			effect.push_back(e);
		}
	}
}