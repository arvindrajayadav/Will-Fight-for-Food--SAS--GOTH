#include "Stat.h"

void Stat :: Change(StatVal st, int change, bool clamp)
{
	val[st].num += change;

	if(clamp)
		Clamp(val[STVAL_CUR].num,0,val[STVAL_FULL].num);

	val[st].str = NumberToString<int>(val[st].num);
}

void Stat :: Set(StatVal st, int value, bool clamp)
{
	val[st].num = value;

	if(clamp)
		Clamp(val[STVAL_CUR].num,0,val[STVAL_FULL].num);

	val[st].str = NumberToString<int>(val[st].num);
}

void Stat :: Validate()
{
	if(val[STVAL_CUR].num < 1)
		Set(STVAL_CUR, 1, false);
}

void StatDrawHelper :: Load(rapidxml::xml_node<char> *node)
{
	if(NodeValid(node))
	{
		info.Load(node->first_node("info"));
		value.Load(node->first_node("value"));
		preview.Load(node->first_node("preview"));
		LoadStr(description, "text", node->first_node("info"));
	}
}

void StatDrawHelper :: Draw(std::string val)
{
	gTextManager.Draw(info.x,info.y,description,info.col,info.font);
	gTextManager.Draw(value.x,value.y,val,value.col,value.font);
}