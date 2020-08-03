#include "map.h"

void Map::Load(std::string filename)
{
	loc.clear();
	XMLDoc conf(filename);
	if (conf.ready())
	{
		rapidxml::xml_node<char> *node = conf.Doc()->first_node("map");
		if (NodeValid(node))
		{
			rapidxml::xml_node<char> *bgnode = node->first_node("bg");
			if (NodeValid(bgnode))
				LoadStr(bg, "img", bgnode);

			loading.Load(node->first_node("load"));

			for (rapidxml::xml_node<char> *n = node->first_node("loc"); n != NULL; n = n->next_sibling("loc"))
			{
				Button b;
				b.Load(n);
				button.push_back(b);

				LocInfo l;
				l.Load(n->first_node("info"));
				loc.push_back(l);
			}

			std::string usekey;
			LoadStr(usekey, "keyboard", node, false);
			if (usekey == "true")
			{
				usekeyboard = true;
				AssignPaths();
			}
			else
				usekeyboard = false;
		}
	}
}

void Map::Draw()
{
	gAssetManager.DrawTexture(0, 0, bg);

	if (hoverindex != -1)
		gAssetManager.DrawTexture(loc.at(hoverindex).x, loc.at(hoverindex).y, loc.at(hoverindex).img);

	Menu::Draw();

	if (locpicked)
	{
		loading.Draw();
		loaddrawn = true;
	}
}

bool Map::HandleEvents(SDL_Event Event)
{
	int choice = Menu::HandleEvents(Event);
	if (choice != -1)
	{
		curloc = loc.at(choice).id;
		locpicked = true;
	}

	if (Event.type == SDL_KEYDOWN && (Event.key.keysym.sym == SDLK_m || Event.key.keysym.sym == SDLK_ESCAPE))
		keydown = true;
	else if (keydown && Event.type == SDL_KEYUP && (Event.key.keysym.sym == SDLK_m || Event.key.keysym.sym == SDLK_ESCAPE))
	{
		keydown = false;
		return true;
	}
	return false;
}

bool Map::InternalEvents()
{
	if (locpicked && loaddrawn)
	{
		locpicked = false;
		loaddrawn = false;
		return true;
	}
	return false;
}