#include "HoverInfo.h"

void HoverInfo :: Load(rapidxml::xml_node<char> *node)
{
	if(NodeValid(node,false))
	{
		LoadXY(x, y, node);
		LoadStr(text, "text", node);
		LoadNum(font, "font", node);
		LoadColor(col,node);

		int num = 0;
		if(LoadNum(num, "align", node, false))
			align = (TextAlign) num;
		else
			align = ALIGN_LEFT;

		enabled = true;
	}
}

void HoverInfo :: Draw()
{
	if(enabled)
		gTextManager.Draw(x,y,text,col,font,align);
}