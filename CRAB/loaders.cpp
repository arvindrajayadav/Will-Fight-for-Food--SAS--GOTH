#include "loaders.h"

bool NodeValid(rapidxml::xml_node<char> *node, bool echo)
{
	if(node == NULL)
	{
		if(echo)
		{
			std::string error_msg = "node not found \n";
			fprintf(stderr, error_msg.c_str());
		}
		return false;
	}

	return true;
}

bool LoadStr(std::string &val, std::string name, rapidxml::xml_node<char> *node, bool echo)
{
	if(node->first_attribute(name.c_str()) != NULL)
		val = node->first_attribute(name.c_str())->value();
	else
	{
		if(echo)
		{
			std::string error_msg = "attribute " + name + " not found in " + node->name() + "\n";
			fprintf(stderr, error_msg.c_str());
		}
		return false;
	}

	return true;
}

template <typename T>
bool LoadNum(T &val, std::string name, rapidxml::xml_node<char> *node, bool echo)
{
	if(node->first_attribute(name.c_str()) != NULL)
		val = StringToNumber<T>(node->first_attribute(name.c_str())->value());
	else
	{
		if(echo)
		{
			std::string error_msg = "attribute " + name + " not found in " + node->name() + "\n";
			fprintf(stderr, error_msg.c_str());
		}
		return false;
	}

	return true;
}

bool LoadRect(SDL_Rect &rect, rapidxml::xml_node<char> *node, bool echo)
{
	if(LoadNum(rect.x, "x", node, echo) && LoadNum(rect.y, "y", node, echo)	&& LoadNum(rect.w, "w", node, echo) && LoadNum(rect.h, "h", node, echo))
		return true;
	return false;
}

bool LoadColor(SDL_Color &col, rapidxml::xml_node<char> *node, std::string r_name, std::string g_name, std::string b_name, bool echo)
{
	int r = 0, g = 0, b = 0;

	if(LoadNum(r, r_name, node, echo) && LoadNum(g, g_name, node, echo) && LoadNum(b, b_name, node, echo))
	{
		col.r = r;
		col.g = g;
		col.b = b;
		return true;
	}

	return false;
}

bool LoadXY(Sint16 &x, Sint16 &y, rapidxml::xml_node<char> *node, bool echo)
{
	if(LoadNum(x, "x", node, echo) && LoadNum(y, "y", node, echo))
		return true;
	return false;
}