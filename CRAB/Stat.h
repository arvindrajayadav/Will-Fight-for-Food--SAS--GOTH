#ifndef STAT_H
#define STAT_H

#include "AssetManager.h"
#include "TextManager.h"

enum StatType
{
	STAT_HEALTH,
	STAT_ATTACK,
	STAT_DEFENSE,
	STAT_SPEED,
	STAT_TOTAL
};

enum StatVal { STVAL_CUR, STVAL_FULL, STVAL_TOTAL };

struct PreviewData
{
	Sint16 x, y;
	SDL_Color col, col_inc, col_dec;
	Uint16 font;

	int val;
	std::string str;

	PreviewData() { x = 0; y = 0; col_inc = black; col_dec = black; val = 0; }

	void Load(rapidxml::xml_node<char> *node)
	{
		if (NodeValid(node))
		{
			LoadXY(x, y, node);
			LoadNum(font, "font", node);
			LoadColor(col, node);
			LoadColor(col_inc, node, "r_inc", "g_inc", "b_inc");
			LoadColor(col_dec, node, "r_dec", "g_dec", "b_dec");
		}
	}

	void Draw(int base_val)
	{
		str = NumberToString(val);

		for (int i = 0; i < STAT_TOTAL; i++)
			if (base_val == val)
				gTextManager.Draw(x, y, str, col, font);
			else if (base_val < val)
				gTextManager.Draw(x, y, str, col_inc, font);
			else
				gTextManager.Draw(x, y, str, col_dec, font);
	}

	void SetVal(int num)
	{
		if (num < 0)
			val = 1;
		else
			val = num;
	}
};

struct Stat
{
	struct StatContainer
	{
		int num;
		std::string str;

		StatContainer() { num = 5; }

		void Load(std::string name, rapidxml::xml_node<char> *node)
		{
			LoadStr(str, name, node);
			num = StringToNumber<int>(str);
		}
	};

	StatContainer val[STVAL_TOTAL];

	Stat(){}

	void Change(StatVal st, int change, bool clamp);
	void Set(StatVal st, int value, bool clamp);
	void Reset() { Set(STVAL_CUR, val[STVAL_FULL].num, false); }
	void Validate();

	void Load(rapidxml::xml_node<char> *node)
	{
		if (NodeValid(node))
		{
			val[STVAL_CUR].Load("cur", node);
			val[STVAL_FULL].Load("val", node);
		}
	}
};

class StatDrawHelper
{
	//Used to draw stat value and description
	struct DrawInfo
	{
		//Stat drawing stuff
		Sint16 x, y;
		SDL_Color col;
		Uint16 font;

		DrawInfo()
		{
			x = 0;
			y = 0;
			col = black;
			font = 0;
		}

		void Load(rapidxml::xml_node<char> *node)
		{
			if (NodeValid(node))
			{
				LoadXY(x, y, node);
				LoadNum(font, "font", node);
				LoadColor(col, node);
			}
		}
	} value, info;

	std::string description;

public:
	PreviewData preview;

	StatDrawHelper(){}
	~StatDrawHelper(){}

	void Load(rapidxml::xml_node<char> *node);
	void Draw(std::string val);
};

#endif //STAT_H