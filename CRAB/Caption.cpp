#include "Caption.h"

void Caption::Load(rapidxml::xml_node<char> *node)
{
	if (NodeValid(node, false))
	{
		LoadXY(x, y, node);
		LoadStr(text, "text", node);
		LoadNum(font, "font", node);
		LoadColor(col, node);
		LoadColor(col_s, node, "r_s", "g_s", "b_s", false);

		int num = 0;
		if (LoadNum(num, "align", node, false))
			align = (TextAlign)num;
		else
			align = ALIGN_LEFT;

		enabled = true;
	}
}

void Caption::Draw(bool selected)
{
	if (enabled)
		if (selected)
			gTextManager.Draw(x, y, text, col_s, font, align);
		else
			gTextManager.Draw(x, y, text, col, font, align);
}

void Caption::Draw(Sint16 XOffset, Sint16 YOffset, std::string str, bool selected)
{
	if (selected)
		gTextManager.Draw(XOffset + x, YOffset + y, text + str, col_s, font, align);
	else
		gTextManager.Draw(XOffset + x, YOffset + y, text + str, col, font, align);
}