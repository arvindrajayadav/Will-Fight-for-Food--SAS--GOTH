#include "GameOverMenu.h"

void GameOverMenu :: Load(rapidxml::xml_node<char> *node)
{
	if(NodeValid(node))
	{
		rapidxml::xml_node<char> *bgnode = node->first_node("bg");
		if(NodeValid(bgnode))
		{
			LoadStr(bg, "img", bgnode);
			LoadXY(x, y, bgnode);
		}

		title.Load(node->first_node("title"));
		menu.Load(node->first_node("menu"));
	}
}

 int GameOverMenu :: HandleEvents(SDL_Event Event)
{
	return menu.HandleEvents(Event);
}

 void GameOverMenu :: Draw()
 {
	 gAssetManager.DrawTexture(x, y, bg);
	 title.Draw();
	 menu.Draw();
 }