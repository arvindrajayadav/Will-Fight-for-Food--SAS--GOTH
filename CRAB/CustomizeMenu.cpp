#include "CustomizeMenu.h"

void CustomizeMenu::Load(rapidxml::xml_node<char> *node)
{
	if (NodeValid(node))
	{
		title.Load(node->first_node("title"));
		prev.Load(node->first_node("prev"));
		next.Load(node->first_node("next"));

		if (NodeValid(node->first_node("jared")))
		{
			rapidxml::xml_node<char> *jarnode = node->first_node("jared");

			int size = 1;
			LoadNum(size, "size", jarnode);
			menu.resize(size);

			int i = 0;
			for (rapidxml::xml_node<char> *mennode = jarnode->first_node("menu"); mennode != NULL; mennode = mennode->next_sibling("menu"), ++i)
				for (rapidxml::xml_node<char> *bnode = mennode->first_node("button"); bnode != NULL; bnode = bnode->next_sibling("button"))
				{
					Button bu;
					bu.Load(bnode);
					menu.at(i).button.push_back(bu);

					SkinPaths s;
					for (rapidxml::xml_node<char> *n = bnode->first_node("path"); n != NULL; n = n->next_sibling("path"))
						s.push_back(n);

					img_list.push_back(s);
				}

			slot_per_menu = menu.at(0).button.size();
		}
	}
}

void CustomizeMenu::Draw()
{
	title.Draw();

	menu.at(curmenu).Draw();

	if (curmenu > 0)
		prev.Draw();

	if (curmenu < menu.size() - 1)
		next.Draw();
}

bool CustomizeMenu::HandleEvents(SDL_Event Event)
{
	if (curmenu > 0)
		if (prev.HandleEvents(Event) == BUTTON_LCLICK || (Event.type == SDL_KEYUP && gInputVal[I_PUNCH].Equals(Event.key.keysym.sym)))
		{
			curmenu--;
			if (curmenu < 0)
				curmenu = 0;
		}

	if (curmenu < menu.size() - 1)
		if (next.HandleEvents(Event) == BUTTON_LCLICK || (Event.type == SDL_KEYUP && gInputVal[I_KICK].Equals(Event.key.keysym.sym)))
		{
			curmenu++;
			if (curmenu >= menu.size())
				curmenu = menu.size() - 1;
		}

	int choice = menu.at(curmenu).HandleEvents(Event);
	if (choice != -1)
	{
		int index = curmenu * slot_per_menu + choice;
		int size = img_list.at(index).size();

		if (size != 1)
			cur_img_path = img_list.at(index).at(rand() % size);
		else
			cur_img_path = img_list.at(index).at(0);

		return true;
	}

	return false;
}