#include "ScreenSettings.h"

ScreenSettings gScreenSettings;

void ScreenSettings :: Load(rapidxml::xml_node<char> *node, Uint32 &videoflags)
{
	rapidxml::xml_node<char> *fsnode = node->first_node("fullscreen");
	if(NodeValid(fsnode))
	{
		std::string str = node->first_node("fullscreen")->value();
		if(str == "true")
		{
			videoflags = videoflags | SDL_FULLSCREEN;
			fullscreen = true;
		}
		else
			fullscreen = false;
	}

	rapidxml::xml_node<char> *screennode = node->first_node("screen");
	if(NodeValid(screennode))
	{
		LoadNum(def.w, "w", screennode);
		LoadNum(def.h, "h", screennode);
		LoadNum(bpp, "bpp", screennode);
		LoadNum(fps, "fps", screennode);
	}

	rapidxml::xml_node<char> *resnode = node->first_node("resolution");
	if(NodeValid(resnode))
	{
		LoadNum(cur.w, "w", resnode);
		LoadNum(cur.h, "h", resnode);
	}
}