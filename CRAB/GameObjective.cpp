#include "GameObjective.h"

void GameObjective :: Load(std::string filename)
{
	XMLDoc conf(filename);
	if(conf.ready())
	{
		rapidxml::xml_node<char> *node = conf.Doc()->first_node("objectives");
		if(NodeValid(node))
		{
			rapidxml::xml_node<char> *bgnode = node->first_node("bg");
			if(NodeValid(bgnode))
			{
				LoadStr(bg.img, "img", bgnode);
				LoadXY(bg.x, bg.y, bgnode);
			}

			rapidxml::xml_node<char> *textnode = node->first_node("text");
			if(NodeValid(textnode))
			{
				LoadXY(text.x, text.y, textnode);
				LoadNum(text.inc, "inc", textnode);
				LoadNum(text.font, "font", textnode);
				LoadColor(text.cur,textnode->first_node("active"));
				LoadColor(text.past,textnode->first_node("done"));
			}

			rapidxml::xml_node<char> *tabnode = node->first_node("tab");
			if(NodeValid(tabnode))
			{
				tab.img.Load(tabnode);
				LoadXY(tab.x, tab.y, tabnode);
				LoadNum(tab.font, "font", tabnode);
				LoadNum(tab.inc, "inc", tabnode);
				LoadColor(tab.col,tabnode);
				LoadColor(tab.col_s,tabnode,"r_s","g_s","b_s");

				int num = 0;
				if(LoadNum(num, "align", tabnode))
					tab.align = (TextAlign) num;
				else
					tab.align = ALIGN_LEFT;
			}

			close.Load(node->first_node("close"));
		}
	}
}

void GameObjective :: Draw()
{
	gAssetManager.DrawTexture(bg.x, bg.y, bg.img);
	title_menu.Draw();
	if(selected_quest < quest.size())
		quest[selected_quest].Draw(text);
	close.Draw();
}

bool GameObjective :: HandleEvents(SDL_Event Event)
{
	int choice = title_menu.HandleEvents(Event);
	if(choice != -1)
		selected_quest = choice;

	if(close.HandleEvents(Event) == BUTTON_LCLICK)
		return true;
	return false;
}

void GameObjective :: AddText(std::string qchain, std::string qtext)
{
	int index = 0;
	for(std::vector<GameQuest>::iterator i=quest.begin(); i != quest.end(); ++i, ++index)
		if(i->name == qchain)
		{
			i->aim_text.push_back(qtext);
			break;
		}

	if(index == quest.size())
	{
		GameQuest q;
		q.name = qchain;
		q.aim_text.clear();
		q.aim_text.push_back(qtext);

		quest.push_back(q);

		Button b;
		b.Image(tab.img);

		Sint16 bx = tab.x + tab.inc * title_menu.button.size(), by = tab.y;
		Uint16 w = gAssetManager.GetTexture(tab.img.normal).W()/2, h = gAssetManager.GetTexture(tab.img.normal).H()/2;

		b.Init(bx, by);
		b.AddCaption(qchain, bx + w, by + h, tab.col, tab.col_s, tab.font, tab.align);
		title_menu.button.push_back(b);
	}
}

void GameObjective :: SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root)
{
	for(std::vector<GameQuest>::iterator q = quest.begin(); q != quest.end(); ++q)
	{
		rapidxml::xml_node<char> *child = doc.allocate_node(rapidxml::node_element, "quest");
		child->append_attribute(doc.allocate_attribute("title", q->name.c_str()));

		for(std::vector<std::string>::iterator str = q->aim_text.begin(); str != q->aim_text.end(); ++str)
		{
			rapidxml::xml_node<char> *grandchild = doc.allocate_node(rapidxml::node_element, "info");
			grandchild->value(str->c_str());
			child->append_node(grandchild);
		}

		root->append_node(child);
	}
}

void GameObjective :: LoadState(rapidxml::xml_node<char> *node)
{
	for(rapidxml::xml_node<char> *n = node->first_node("quest"); n != NULL; n = n->next_sibling("quest"))
	{
		std::string name = n->first_attribute("title")->value();

		for(rapidxml::xml_node<char> *i = n->first_node("info"); i != NULL; i = i->next_sibling("info"))
			AddText(name, i->value());
	}
}