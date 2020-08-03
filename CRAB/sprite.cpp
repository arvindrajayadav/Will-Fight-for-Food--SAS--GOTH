#include "sprite.h"

//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
Sprite::Sprite()
{
	dir = DIR_LEFT;
	combo = "";
	collide_type = AICO_TOPRIGHT;
	fight_state = STATE_GETNEXTMOVE;

	x = 0;
	y = 0;
	img_w = 0;
	img_h = 0;
	w_walk = 0;
	h_walk = 0;

	vx = 0;
	vy = 0;

	box_d.x = 0;
	box_d.y = 0;
	box_d.w = 0;
	box_d.h = 0;

	clip = box_d;
	box_v = box_d;
	box_v_walk = box_d;

	Visible(false);
	ai_movement = AIM_DIRECT;
	dx_prev = 0;
	dy_prev = 0;
	walk_enabled = false;
	ko_playing = false;
}

void Sprite::Load(rapidxml::xml_node<char> *node, std::vector<FightMoveSet> &fight_ms, std::vector<MovementSet> &movement_ms, VarMap &varmap)
{
	if (NodeValid(node))
	{
		LoadStr(name, "name", node);
		LoadNum(x, "x", node);
		LoadNum(y, "y", node);
		LoadNum(w_walk, "w", node);
		LoadNum(h_walk, "h", node);
		LoadStr(image, "img", node);

		clip.x = 0;
		clip.y = 0;
		clip.w = w_walk;
		clip.h = h_walk;
		box_v = clip;

		ImageData dat;
		gAssetManager.GetTexture(image, dat);
		img_w = dat.W();
		img_h = dat.H();

		Uint16 index = 0;
		LoadNum(index, "moveset", node);
		fight_move = fight_ms[index];
		fight_move.ListAttackMoves(attack_moves);
		collide_type = (AICollisionCheck)(rand() % 4);

		if (node->first_attribute("dir") != NULL)
		{
			std::string str;
			LoadStr(str, "dir", node);
			if (str == "right")
				dir = DIR_RIGHT;
			else
				dir = DIR_LEFT;
		}

		if (node->first_node("box_v") != NULL)
		{
			LoadRect(box_v_walk, node->first_node("box_v"));
			box_v = box_v_walk;
		}
		else
			box_v_walk = clip;

		visible.Load(node->first_node("visible"));

		Uint16 num = 0;
		if (LoadNum(num, "movement", node) && num < movement_ms.size())
			WalkPattern(movement_ms[num]);
	}
}
//------------------------------------------------------------------------
// Purpose: Draw the sprite
//------------------------------------------------------------------------
void Sprite::Draw(Sint16 XOffset, Sint16 YOffset)
{
	gAssetManager.DrawTexture(x + XOffset, y + YOffset, image, &clip);
	img_eff.Draw(x + XOffset, y + YOffset);

	//gTextManager.Draw(x + XOffset + clip.w/2, y + YOffset - 20, NumberToString<int>(state.val), black, 2, ALIGN_CENTER);
	//SDL_FillRect(screen,&BoxV(),SDL_MapRGB(screen->format, 0xCC, 0, 0));
	//SDL_FillRect(screen,&BoxD(),SDL_MapRGB(screen->format, 0, 0, 0xCC));

	//gTextManager.Draw(x + XOffset + clip.w/2, y + YOffset - 20,
	//	NumberToString<Sint16>(clip.x) + "," + NumberToString<Sint16>(clip.y), black, 2, ALIGN_CENTER);
}
//------------------------------------------------------------------------
// Purpose: Walking animation
//------------------------------------------------------------------------
void Sprite::SetVel(Direction dir, int val)
{
	switch (dir)
	{
	case DIR_UP: XVel(0); YVel(-1 * val); break;
	case DIR_DOWN: XVel(0); YVel(val); break;
	case DIR_LEFT: XVel(-1 * val); YVel(0); break;
	case DIR_RIGHT: XVel(val); YVel(0); break;
	default:break;
	}
}

void Sprite::Walk(GameMode mode)
{
	clip.w = w_walk;
	clip.h = h_walk;
	img_eff.visible = false;

	if (vx > 0)
		dir = DIR_RIGHT;
	else if (vx < 0)
		dir = DIR_LEFT;
	else if (vy == 0)
	{
		if (dir == DIR_RIGHT)
		{
			//right idle
			Walk(mode, DIR_UP);
			return;
		}
		else
		{
			//left idle
			Walk(mode, DIR_DOWN);
			return;
		}
	}

	Walk(mode, dir);
}

void Sprite::Walk(GameMode mode, Direction direction)
{
	if (clip.x >= img_w - clip.w)
		clip.x = 0;
	else
		clip.x += clip.w;

	if (mode == MODE_RPG)
		switch (direction)
	{
		case DIR_RIGHT: clip.y = 0; break;
		case DIR_LEFT: clip.y = clip.h; break;
		case DIR_UP: clip.y = clip.h * 2; break;
		case DIR_DOWN: clip.y = clip.h * 3; break;
		default:break;
	}
	else
		switch (direction)
	{
		case DIR_RIGHT: clip.y = clip.h * 4; break;
		case DIR_LEFT: clip.y = clip.h * 5; break;
		case DIR_UP: clip.y = clip.h * 6; break;
		case DIR_DOWN: clip.y = clip.h * 7; break;
		default:break;
	}
}
//------------------------------------------------------------------------
// Purpose: Collision check
//------------------------------------------------------------------------
bool Sprite::FightCollide(Sprite &s, Range &range)
{
	if (MoveToX(s, range) == 0 && MoveToY(s, range) == 0)
		return true;

	return false;
}

bool Sprite::FullCollide(SDL_Rect box)
{
	return CollideBoundingBox(BoundRect(), box);
}

bool Sprite::InsideRect(SDL_Rect box)
{
	if (x + box_v_walk.x < box.x || x + box_v_walk.x + box_v_walk.w > box.x + box.w || y + box_v_walk.y < box.y || y + box_v_walk.y + box_v_walk.h > box.y + box.h)
		return false;
	return true;
}
//------------------------------------------------------------------------
// Purpose: Walk in preset paths
//------------------------------------------------------------------------
void Sprite::TraversePath(int speed)
{
	if (walk_enabled)
	{
		if (walk_pattern.completed_steps < walk_pattern.path[walk_pattern.current_movement].steps)
		{
			Move();
			walk_pattern.completed_steps++;
		}
		else
		{
			walk_pattern.current_movement++;
			walk_pattern.completed_steps = 0;

			//go to next node
			if (walk_pattern.current_movement >= walk_pattern.path.size())
			{
				if (walk_pattern.repeat)
					walk_pattern.current_movement = 0;
				else
				{
					Stop();
					walk_enabled = false;
				}
			}
			else
				SetVel(walk_pattern.path[walk_pattern.current_movement].direction, speed);
		}
	}
}
//------------------------------------------------------------------------
// Purpose: Update the frame info of the sprite
//------------------------------------------------------------------------
void Sprite::UpdateFrame(GameMode mode, bool repeat)
{
	MoveFrame m;
	if (fight_move.CurrentFrame(m))
	{
		if (dir == DIR_LEFT)
		{
			clip = m.left.clip;
			BoxV(m.left.vbox);
			BoxD(m.left.dbox);
			x += m.left.dx;
			y += m.left.dy;

			dx_prev = m.left.dx;
			dy_prev = m.left.dy;
		}
		else
		{
			clip = m.right.clip;
			BoxV(m.right.vbox);
			BoxD(m.right.dbox);
			x += m.right.dx;
			y += m.right.dy;

			dx_prev = m.right.dx;
			dy_prev = m.right.dy;
		}

		fight_move.frame_cur++;
	}
	else if (repeat == false)
		ResetFrame(mode);
	else
		fight_move.frame_cur = 0;
}

void Sprite::UndoUpdateFrame()
{
	x -= dx_prev;
	y -= dy_prev;
}

//------------------------------------------------------------------------
// Purpose: Update the move info of the sprite
// string version is for player moves, integer version is for AI moves
//------------------------------------------------------------------------
void Sprite::UpdateMove(std::string new_combo)
{
	state.Revert();
	if ((combo == "" || LastFrame()) && new_combo != "")
		ForceUpdateMove(new_combo);
}

void Sprite::ForceUpdateMove(std::string new_combo)
{
	fight_move.frame_cur = 0;
	fight_move.cur_move = fight_move.FindMove(new_combo, state.val);

	if (fight_move.cur_move >= 0)
	{
		combo = new_combo;
		fight_move.frame_total = fight_move.move[fight_move.cur_move].frame.size();
		state.val = fight_move.move[fight_move.cur_move].res_state;
		state.timer.start();
	}
	else
	{
		combo = "";
		fight_move.frame_total = 0;
	}
}

void Sprite::UpdateMove(Uint16 new_combo)
{
	state.Revert();
	if (combo == "" && new_combo < fight_move.move.size())
		ForceUpdateMove(new_combo);
}

void Sprite::ForceUpdateMove(Uint16 new_combo)
{
	fight_move.frame_cur = 0;
	fight_move.cur_move = new_combo;

	if (fight_move.move[new_combo].req_state == state.val)
	{
		combo = fight_move.move[new_combo].type;
		fight_move.frame_total = fight_move.move[new_combo].frame.size();
		state.val = fight_move.move[new_combo].res_state;
		state.timer.start();
	}
	else
	{
		combo = "";
		fight_move.frame_total = 0;
	}
}
//------------------------------------------------------------------------
// Purpose: Reset the frame info of the sprite
//------------------------------------------------------------------------
void Sprite::ResetFrame(GameMode mode)
{
	combo = "";

	clip.x = 0;
	clip.w = w_walk;
	clip.h = h_walk;
	if (mode == MODE_FIGHT)
	{
		if (dir == DIR_LEFT)
			clip.y = clip.h * 5;
		else
			clip.y = clip.h * 4;
	}
	else
	{
		if (dir == DIR_LEFT)
			clip.y = clip.h * 3;
		else
			clip.y = clip.h * 2;
	}

	box_v = box_v_walk;
	box_d.w = 0;
	box_d.h = 0;
	img_eff.visible = false;
}

//------------------------------------------------------------------------
// Purpose: Move towards or away from the sprite
//------------------------------------------------------------------------
int Sprite::MoveToX(Sprite &s, Range &range)
{
	Sint16 X, W, SX, SW;

	if (range.valid)
	{
		if (dir == DIR_LEFT)
		{
			X = x + range.left.x;
			W = range.left.w;
			SX = s.x + s.box_v_walk.x;
			SW = s.box_v_walk.w;
		}
		else
		{
			X = x + range.right.x;
			W = range.right.w;
			SX = s.x + s.box_v_walk.x;
			SW = s.box_v_walk.w;
		}

		if (X > SX + SW)
			return -1;
		else if (X + W < SX)
			return 1;
	}
	else
	{
		X = x + box_v_walk.x;
		W = box_v_walk.w;
		SX = s.x + s.box_v_walk.x;
		SW = s.box_v_walk.w;

		switch (collide_type)
		{
		case AICO_TOPLEFT:
		case AICO_BOTTOMLEFT:
			if (X > SX + SW)
				return -1;
			else if (X + W / 2 < SX)
				return 1;
			break;
		case AICO_TOPRIGHT:
		case AICO_BOTTOMRIGHT:
		default:
			if (X + W / 2 > SX + SW)
				return -1;
			else if (X + W < SX)
				return 1;
			break;
		}
	}

	return 0;
}

int Sprite::MoveToY(Sprite &s, Range &range)
{
	Sint16 Y = y + box_v_walk.y + box_v_walk.h, SY = s.y + s.box_v_walk.y + s.box_v_walk.h;
	Sint16 diff = Y - SY;

	switch (collide_type)
	{
	case AICO_TOPLEFT:
	case AICO_TOPRIGHT:
		if (diff > PLANE_WIDTH / 2)
			return -1;
		else if (diff < 0)
			return 1;
		break;
	case AICO_BOTTOMLEFT:
	case AICO_BOTTOMRIGHT:
	default:
		if (diff > 0)
			return -1;
		else if (diff < -1 * PLANE_WIDTH / 2)
			return 1;
		break;
	}

	return 0;
}

void Sprite::MoveToSprite(Sprite &s, int vel, Range &range)
{
	switch (ai_movement)
	{
	case AIM_DIRECT:
		XVel(MoveToX(s, range) * vel);
		YVel(MoveToY(s, range) * vel);
		break;
	case AIM_XY:
	{
		int res = MoveToX(s, range);
		if (res == 0)
		{
			YVel(MoveToY(s, range) * vel);
			XVel(0);
		}
		else
		{
			XVel(res * vel);
			YVel(0);
		}
	}
	break;
	case AIM_YX:
	default:
	{
		int res = MoveToY(s, range);
		if (res == 0)
		{
			XVel(MoveToX(s, range) * vel);
			YVel(0);
		}
		else
		{
			YVel(res * vel);
			XVel(0);
		}
	}
	break;
	}

	Move();
}

void Sprite::RunFromSprite(Sprite sp, SDL_Rect bounds)
{
	int seg_w = bounds.w / 5, seg_h = bounds.h / 5;
	double dist = Distance(sp), safe_dist = bounds.w / 3;

	//First we check whether the player is near - if not, no need to run
	if (dist < safe_dist)
	{
		//Now we locate our position in the level out of a 3x3 grid
		//Inside, we decide a direction randomly or based on player's relative position
		if (x < bounds.x + seg_w)
		{
			if (y < bounds.y + seg_h)
			{
				//(0,0)
				if (sp.x > x)
					YVel(1);
				else
					XVel(1);
			}
			else if (y > bounds.y + bounds.h - seg_h)
			{
				//(0,2)
				if (sp.x > x)
					YVel(-1);
				else
					XVel(1);
			}
			else
			{
				//(0,1)
				if (sp.x > x)
				{
					if (sp.y < y)
						YVel(1);
					else
						YVel(-1);
				}
				else
					XVel(1);
			}
		}
		else if (x > bounds.x + bounds.w - seg_w)
		{
			if (y < bounds.y + seg_h)
			{
				//(2,0)
				if (sp.x < x)
					YVel(1);
				else
					XVel(-1);
			}
			else if (y > bounds.y + bounds.h - seg_h)
			{
				//(2,2)
				if (sp.x < x)
					YVel(-1);
				else
					XVel(-1);
			}
			else
			{
				//(2,1)
				if (sp.x < x)
				{
					if (sp.y < y)
						YVel(1);
					else
						YVel(-1);
				}
				else
					XVel(-1);
			}
		}
		else
		{
			if (y < bounds.y + seg_h)
			{
				//(1,0)
				if (sp.y > y)
				{
					if (sp.x < x)
						XVel(1);
					else
						XVel(-1);
				}
				else
					YVel(1);
			}
			else if (y > bounds.y + bounds.h - seg_h)
			{
				//(1,2)
				if (sp.y < y)
				{
					if (sp.x < x)
						XVel(1);
					else
						XVel(-1);
				}
				else
					YVel(-1);
			}
			else
			{
				//(1,1)
				if (sp.y > y)
				{
					if (sp.x < x)
						XVel(1);
					else
						XVel(-1);
				}
				else
				{
					if (sp.y < y)
						YVel(1);
					else
						YVel(-1);
				}
			}
		}
	}
	else
	{
		XVel(0);
		YVel(0);
	}
	Move();
}

//------------------------------------------------------------------------
// Purpose: Checks about dealing damage to sprite
//------------------------------------------------------------------------
bool Sprite::TakingDamage(Sprite &sp)
{
	if (DamageValid(sp))
		return CollideBoundingBox(BoxV(), sp.BoxD()) && BoxV().w > 0 && BoxV().h > 0 && sp.BoxD().w > 0 && sp.BoxD().h > 0;

	return false;
}

bool Sprite::DealingDamage(Sprite &sp)
{
	if (DamageValid(sp))
		return CollideBoundingBox(BoxD(), sp.BoxV()) && sp.BoxV().w > 0 && sp.BoxV().h > 0 && BoxD().w > 0 && BoxD().h > 0;

	return false;
}

int Sprite::TakeDamage(Sprite &s, GameObject &attacker, GameObject &defender)
{
	FightMove f;
	int dmg = 0;
	if (s.fight_move.CurrentMove(f))
	{
		dmg = -1 * f.base_dmg - attacker.stat[STAT_ATTACK].val[STVAL_CUR].num;
		dmg -= (dmg * defender.stat[STAT_DEFENSE].val[STVAL_CUR].num) / 30;
		if (dmg >= 0)
			dmg = -1;

		ForceUpdateMove(f.effect);
		gMusicManager.PlayEffect(f.sound);
		img_eff = f.img_eff;
	}

	UndoMove();
	Stop();
	return dmg;
}

void Sprite::Attack(Sprite &s, int vel)
{
	switch (fight_state)
	{
	case STATE_GETNEXTMOVE:
	{
		ai_movement = (AIMovementType)(rand() % 3);
		fight_state = STATE_GETINRANGE;
		movetime.start();

		Uint16 size = attack_moves.size();
		if (size > 1)
			fight_move.next_move = attack_moves[rand() % attack_moves.size()];
		else if (size <= 0)
			fight_state = STATE_CANTFIGHT;
		else
			fight_move.next_move = attack_moves[0];
	}
	break;
	case STATE_GETINRANGE:
	{
		FightMove f;
		if (fight_move.NextMove(f) && FightCollide(s, f.range))
		{
			if (movetime.get_ticks() > f.delay / (difficulty + 1))
				fight_state = STATE_EXECUTEMOVE;
		}
		else if (combo == "")
			MoveToSprite(s, vel, f.range);
	}
	break;
	case STATE_EXECUTEMOVE:
		UpdateMove(fight_move.next_move);
		fight_state = STATE_GETNEXTMOVE;
		movetime.stop();
		break;
	default: break;
	}
}