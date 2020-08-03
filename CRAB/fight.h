#ifndef FIGHT_H
#define FIGHT_H

#include "FightInput.h"

struct ImageEffect
{
	std::string img;
	Sint16 x,y;
	bool visible;

	ImageEffect() { x = 0; y = 0; visible = false; }

	void Load(rapidxml::xml_node<char> * node);
	void Draw(Sint16 X, Sint16 Y);
};

struct Frame
{
	//The clip rectangle
	SDL_Rect clip;

	//The hit box of the player WITH RESPECT TO the sprite bounding box
	//_v is the vulnerable hit box, _d is the damage hit box
	SDL_Rect vbox, dbox;

	//The displacement in the position caused by the frame
	int dx, dy;

	Frame() { dx = 0; dy = 0; }

	void Load(rapidxml::xml_node<char> * node);
};

struct MoveFrame
{
	Frame left, right;

	void Load(rapidxml::xml_node<char> * node);
};

struct Range
{
	bool valid;
	SDL_Rect left, right;

	Range() { valid = false; }

	void Load(rapidxml::xml_node<char> * node)
	{
		if(NodeValid(node,false))
		{
			LoadRect(left,node->first_node("left"));
			LoadRect(right,node->first_node("right"));
			valid = true;
		}
		else
			valid = false;
	}
};

struct FightMove
{
	//The type of move, next move in combo and effect of the move on opponent
	std::string type, effect, sound;

	//The base damage of the move and input needed
	int base_dmg;

	//The stun time for the move
	Uint16 stun;

	//The sprite state required for the move, and the state resulting from the move
	Uint16 req_state, res_state;

	//The hit box and sprite coordinates for each frame
	std::vector<MoveFrame> frame;

	//The range of the move
	Range range;

	//The image displayed on being hit
	ImageEffect img_eff;

	//Can this move be used by AI to attack
	bool attack;

	//The AI delays executing the move by this long
	Uint16 delay;

	FightMove() { req_state = 0; res_state = 0; base_dmg = 0; stun = 0; attack = false; delay = 1000; }

	void Load(rapidxml::xml_node<char> * node);
};

struct FightMoveSet
{
	std::vector <FightMove> move;

	//The currently selected move
	int cur_move;

	//The current frame and total frames
	Uint16 frame_cur, frame_total;

	//For AI - the move about to execute
	int next_move;

	FightMoveSet()
	{
		cur_move = -1;
		next_move = -1;

		frame_cur = 0;
		frame_total = 0;

		move.clear();
	}

	void Load(std::string filename);

	bool CurrentMove(FightMove &f);
	bool CurrentFrame(MoveFrame &m);
	bool NextMove(FightMove &f);

	int FindMove(std::string type, int state);
	void ListAttackMoves(std::vector<Uint16> &list);
};

#endif //FIGHT_H