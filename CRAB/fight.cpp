#include "fight.h"

void ImageEffect :: Load(rapidxml::xml_node<char> * node)
{
	if(NodeValid(node,false))
	{
		if(LoadStr(img,"img",node) && LoadXY(x,y,node))
			visible = true;
	}
}

void ImageEffect :: Draw(Sint16 X, Sint16 Y)
{
	if(visible)
		gAssetManager.DrawTexture(X+x,Y+y,img);
}

void Frame :: Load(rapidxml::xml_node<char> * node)
{
	if(NodeValid(node))
	{
		LoadRect(clip,node->first_node("clip"));
		LoadRect(vbox,node->first_node("box_v"));
		LoadRect(dbox,node->first_node("box_d"));

		if(node->first_attribute("dx") != NULL)
			LoadNum(dx, "dx", node);
		else
			dx = 0;

		if(node->first_attribute("dy") != NULL)
			LoadNum(dy, "dy", node);
		else
			dy = 0;
	}
}

void MoveFrame :: Load(rapidxml::xml_node<char> * node)
{
	if(NodeValid(node))
	{
		left.Load(node->first_node("left"));
		right.Load(node->first_node("right"));
	}
}

void FightMove :: Load(rapidxml::xml_node<char> * node)
{
	frame.clear();

	if(NodeValid(node))
	{
		LoadStr(type, "type", node);
		LoadNum(base_dmg, "damage", node);

		if(!LoadStr(effect, "effect", node))
			effect = "";

		if(!LoadStr(sound, "sound", node))
			sound = "";

		if(!LoadNum(stun, "stun", node))
			stun = 0;

		if(!LoadNum(delay, "delay", node))
			delay = 0;

		img_eff.Load(node->first_node("img"));
		range.Load(node->first_node("range"));

		std::string att;
		LoadStr(att,"attack",node);
		if(att == "true")
			attack = true;
		else
			attack = false;

		rapidxml::xml_node<char> *statenode = node->first_node("state");
		if(NodeValid(statenode,false))
		{
			if(!LoadNum(req_state, "req", statenode))
				req_state = 0;

			if(!LoadNum(res_state, "res", statenode))
				res_state = 0;
		}
		else
		{
			req_state = 0;
			res_state = 0;
		}

		for(rapidxml::xml_node<char> *f = node->first_node("framelist")->first_node("frame"); f != NULL; f = f->next_sibling("frame"))
		{
			MoveFrame mf;
			mf.Load(f);
			frame.push_back(mf);
		}
	}
}

void FightMoveSet :: Load(std::string filename)
{
	XMLDoc conf(filename);
	if(conf.ready())
	{
		rapidxml::xml_node<char> *node = conf.Doc()->first_node();

		for(rapidxml::xml_node<char> *n = node->first_node("move"); n != NULL; n = n->next_sibling("move"))
		{
			FightMove fm;
			fm.Load(n);
			move.push_back(fm);
		}
	}
}

bool FightMoveSet :: CurrentMove(FightMove &f)
{
	if(cur_move >= 0 && cur_move < move.size())
	{
		f = move.at(cur_move);
		return true;
	}

	return false;
}

bool FightMoveSet :: NextMove(FightMove &f)
{
	if(next_move >= 0 && next_move < move.size())
	{
		f = move.at(next_move);
		return true;
	}

	return false;
}

bool FightMoveSet :: CurrentFrame(MoveFrame &m)
{
	if(cur_move >= 0 && cur_move < move.size())
		if(frame_cur < frame_total && frame_cur < move.at(cur_move).frame.size())
		{
			m = move.at(cur_move).frame.at(frame_cur);
			return true;
		}

	return false;
}

int FightMoveSet :: FindMove(std::string type, int state)
{
	Uint16 pos = 0;
	for(std::vector<FightMove>::iterator i = move.begin(); i != move.end(); ++i, ++pos)
		if(i->type == type && i->req_state == state)
			return pos;

	pos = 0;
	for(std::vector<FightMove>::iterator i = move.begin(); i != move.end(); ++i, ++pos)
		if(i->type == type && i->req_state == 65535)
			return pos;

	return -1;
}

void FightMoveSet :: ListAttackMoves(std::vector<Uint16> &list)
{
	list.clear();
	Uint16 pos = 0;
	for(std::vector<FightMove>::iterator i = move.begin(); i != move.end(); ++i, ++pos)
		if(i->attack)
			list.push_back(pos);
}