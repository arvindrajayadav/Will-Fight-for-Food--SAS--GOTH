#ifndef GAMEOBJECTHANDLER_H
#define GAMEOBJECTHANDLER_H

#include "object.h"

class GameObjectHandler
{
	struct Position
	{
		Sint16 img_x, img_y, name_x, name_y, approval_x, approval_y, notify_x, notify_y;

		Position()
		{
			img_x = 0; img_y = 0;
			name_x = 0;	name_y = 0;
			notify_x = 0; notify_y = 0;
			approval_x = 0; approval_y = 0;
		}
	} pos, alt;

	Uint16 notify_delay, font_name, font_notify, font_approval;
	SDL_Color col_name, col_approval;

	//To show the change in opinion
	int rating_change;
	bool refresh_rating;
	Uint32 notify_start_time;

public:
	GameObjectHandler()
	{
		notify_delay = 1000; notify_start_time = 0;
		font_name = 0; font_notify = 0; font_approval = 0;
		col_name = white; col_approval = black;
		rating_change = 0;
		refresh_rating = false;
	}
	~GameObjectHandler(){}

	void Load(rapidxml::xml_node<char> *node);
	void Draw(GameObject &obj, bool alt_active = false);

	void ChangeOpinion(GameObject &obj, int val)
	{
		obj.ChangeOpinion(val);
		refresh_rating = true;
		rating_change = val;
	}
};

#endif //GAMEOBJECTHANDLER_H