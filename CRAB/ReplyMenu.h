#ifndef REPLYMENU_H
#define REPLYMENU_H

#include "Caption.h"
#include "menu.h"

class ReplyMenu
{
	std::string bg;
	Sint16 x,y;
	Menu menu;
	SDL_Color col_b, col_s, col_h;
	Uint16 font;

public:

	ReplyMenu(){ x = 0; y = 0; col_b = black; col_s = black; col_h = black; font = 0; }
	~ReplyMenu(){}

	void Load(rapidxml::xml_node<char> *node)
	{
		if(NodeValid(node))
		{
			menu.Load(node);
			LoadColor(col_b,node->first_node("col_b"));
			LoadColor(col_s,node->first_node("col_s"));
			LoadColor(col_h,node->first_node("col_h"));
			LoadNum(font, "font", node);
			LoadXY(x , y, node);
			LoadStr(bg, "bg", node);
		}
	}

	int HandleEvents(SDL_Event Event){ return menu.HandleEvents(Event); }

	void Draw()
	{
		gAssetManager.DrawTexture(x,y,bg);
		menu.Draw();
	}

	void Cache(std::vector<std::string> &reply)
	{
		Uint16 i = 0;
		for(; i < reply.size(); i++)
		{
			menu.button[i].Visible(true);

			std::string text = reply[i];

			SDL_Surface *ref = gTextManager.RenderTextBlended(font,text.c_str(),col_b);
			menu.button[i].W(ref->w);
			menu.button[i].H(ref->h);

			gAssetManager.AddTexture(menu.button[i].Image().normal,ref);
			gAssetManager.AddTexture(menu.button[i].Image().select, gTextManager.RenderTextBlended(font,text,col_s));
			gAssetManager.AddTexture(menu.button[i].Image().hover, gTextManager.RenderTextBlended(font,text,col_h));
		}

		for(; i < menu.button.size(); i++)
			menu.button[i].Visible(false);
	}
};

#endif //REPLYMENU_H