#ifndef GAMEOVERMENU_H
#define GAMEOVERMENU_H

#include "menu.h"

class GameOverMenu
{
	Sint16 x, y;
	std::string bg;

	struct TitleInfo
	{
		Sint16 x, y;
		std::vector<std::string> quote;
		Uint16 font, cur;
		SDL_Color col;
		TextAlign align;

		TitleInfo()
		{
			x = 0;
			y = 0;
			font = 0;
			cur = 0;
			col = black;
			align = ALIGN_CENTER;
		}

		void Load(rapidxml::xml_node<char> *node)
		{
			if(NodeValid(node))
			{
				LoadXY(x,y,node);
				LoadNum(font, "font", node);
				LoadColor(col,node);

				int num = 0;
				if(LoadNum(num,"align",node))
					align = (TextAlign) num;
				else
					align = ALIGN_CENTER;

				for(rapidxml::xml_node<char> *n = node->first_node("quote"); n != NULL; n = n->next_sibling("quote"))
				{
					std::string str;
					LoadStr(str, "text", n);
					quote.push_back(str);
				}
			}
		}

		void Draw() { gTextManager.Draw(x,y,quote[cur],col,font,align); }
	} title;

	Menu menu;

public:
	GameOverMenu(void){}
	~GameOverMenu(void){}

	void Reset() { title.cur = rand()%(title.quote.size()); }

	void Load(rapidxml::xml_node<char> *node);
	int HandleEvents(SDL_Event Event);
	void Draw();
};

#endif //GAMEOVERMENU_H