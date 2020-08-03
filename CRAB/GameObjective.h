#ifndef GAMEOBJECTIVE_H
#define GAMEOBJECTIVE_H

#include "AssetManager.h"
#include "menu.h"

class GameObjective
{
	//The background image data
	struct BackgroundInfo
	{
		Sint16 x, y;
		std::string img;

		BackgroundInfo() { x = 0; y = 0; }
	} bg;

	//The quest text data
	struct QuestText
	{
		Sint16 x, y, inc;
		Uint16 font;
		SDL_Color cur, past;

		QuestText() { x = 0; y = 0; inc = 0; font = 0; cur = white; past = black; }
	} text;

	//The quest tabs data
	struct TabInfo
	{
		Sint16 x, y, inc;
		Uint16 font;
		SDL_Color col, col_s;
		ButtonImage img;
		TextAlign align;

		TabInfo() { x = 0; y = 0; inc = 0; font = 0; col = white; col_s = black; align = ALIGN_LEFT; }
	} tab;

	struct GameQuest
	{
		//Name of quest
		std::string name;

		//The objective text, the last one being the current objective
		std::vector<std::string> aim_text;

		GameQuest() { name=""; aim_text.clear(); }

		void Draw(QuestText &t)
		{
			Uint16 i=0;
			for(; i<aim_text.size()-1; i++)
				gTextManager.Draw(t.x, t.y + t.inc * i,aim_text[i], t.past, t.font);
			gTextManager.Draw(t.x, t.y + t.inc * i, aim_text[i], t.cur, t.font);
		}
	};

	//The close button
	Button close;

	//All the quests
	std::vector<GameQuest> quest;

	//The menu to select which quest to display
	Menu title_menu;

	//The selected quest chain to display
	Uint16 selected_quest;

public:
	GameObjective(void)	{ Init(); }
	~GameObjective(void){}

	void Init()
	{
		quest.clear();
		selected_quest = 0;
	}

	void Load(std::string filename);
	void Draw();
	bool HandleEvents(SDL_Event Event);

	void AddText(std::string qchain, std::string qtext);

	void SaveState(rapidxml::xml_document<char> &doc, rapidxml::xml_node<char> *root);
	void LoadState(rapidxml::xml_node<char> *node);
};

#endif //GAMEOBJECTIVE_H