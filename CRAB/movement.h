#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "AssetManager.h"

struct Movement
{
	Direction direction;
	Uint16 steps;

	Movement()
	{
		direction = DIR_NONE;
		steps = 0;
	}
};

struct MovementSet
{
	//The path followed by the sprite
	std::vector<Movement> path;

	//If true, sprite repeats the path pattern after reaching the last co-ordinate
	bool repeat;

	//The current path node we are traveling to
	Uint16 current_movement, completed_steps;

	MovementSet()
	{
		completed_steps = 0;
		current_movement = 0;
		repeat = false;
	}

	void AddPath(Direction dir, Uint16 steps)
	{
		Movement m;
		m.direction = dir;
		m.steps = steps;
		path.push_back(m);
	}

	void Load(rapidxml::xml_node<char> *node)
	{
		if (NodeValid(node))
		{
			std::string str;
			if (LoadStr(str, "repeat", node) && str == "true")
				repeat = true;
			else
				repeat = false;

			for (rapidxml::xml_node<char> *n = node->first_node("walk"); n != NULL; n = n->next_sibling("walk"))
			{
				LoadStr(str, "dir", n);

				Direction direction = DIR_LEFT;
				if (str == "right") direction = DIR_RIGHT;
				else if (str == "up") direction = DIR_UP;
				else if (str == "down") direction = DIR_DOWN;

				Uint16 num = 0;
				LoadNum(num, "steps", n);
				AddPath(direction, num);
			}
		}
	}
};

#endif //MOVEMENT_H