#include "HintBox.h"

void HintBox :: Load(rapidxml::xml_node<char> *node)
{
	if(NodeValid(node))
	{
		LoadXY(x, y, node);
		LoadStr(bg, "bg", node);

		rapidxml::xml_node<char> *textnode = node->first_node("text");
		if(NodeValid(textnode))
		{
			LoadXY(text.x, text.y, textnode);
			LoadNum(text.inc, "inc", textnode);
			LoadNum(text.font, "font", textnode);
			LoadColor(text.col,textnode);
		}
	}
}

void HintBox :: Draw(std::vector<std::string> &hint)
{
	gAssetManager.DrawTexture(x, y, bg);

	int inc = 0;
	for(std::vector<std::string>::iterator i = hint.begin(); i != hint.end(); ++i, inc += text.inc)
		gTextManager.Draw(text.x, text.y + inc, i->c_str(), text.col, text.font);
}