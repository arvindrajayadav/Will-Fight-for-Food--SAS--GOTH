#ifndef SPRITE_H
#define SPRITE_H

#include <math.h>
#include "AssetManager.h"
#include "MusicManager.h"
#include "fight.h"
#include "movement.h"
#include "timer.h"
#include "object.h"
#include "VarCondition.h"

#define PLANE_WIDTH 20

//------------------------------------------------------------------------
// Purpose: sprite class
//------------------------------------------------------------------------
class Sprite
{
	enum AIMovementType { AIM_DIRECT, AIM_XY, AIM_YX };
	enum AICollisionCheck { AICO_TOPRIGHT, AICO_BOTTOMLEFT, AICO_TOPLEFT, AICO_BOTTOMRIGHT };
	enum AIFightState { STATE_GETNEXTMOVE, STATE_GETINRANGE, STATE_EXECUTEMOVE, STATE_CANTFIGHT };

protected:
	std::string name;

	//The position and velocity of the sprite
	Sint16 x, y, vx, vy;

	//The image of the sprite and it's dimensions
	std::string image;
	Uint16 img_w, img_h;

	//Clip is the portion of the sprite map to be drawn
	SDL_Rect clip;

	//The hit boxes of the character - v is vulnerable hit box, d is damage hit box
	SDL_Rect box_v, box_d;

	//The default width and height of the clip rectangle (used when walking)
	Uint16 w_walk, h_walk;

	//The default vulnerable area of the sprite (used when walking)
	SDL_Rect box_v_walk;

	//The direction the image is facing
	Direction dir;

	//The combo is the sprite is executing right now
	std::string combo;

	//The state of the sprite for fighting
	struct SpriteState
	{
		Uint32 val, delay;
		Timer timer;

		SpriteState() { val = 0; delay = 1000; timer.stop(); }

		void Revert()
		{
			if (timer.get_ticks() > delay)
			{
				timer.stop();
				val = 0;
			}
		}
	} state;

	//The type of movement the sprite will do to get to the player
	AIMovementType ai_movement;

	//The currently playing image effect
	ImageEffect img_eff;

	//The timer which makes NPCs wait before using a move
	Timer movetime;

	//The fighting move set for the sprite
	FightMoveSet fight_move;

	//The list of moves that can be performed while attacking
	std::vector<Uint16> attack_moves;

	//The pattern a peaceful sprite walks in
	MovementSet walk_pattern;

	//Is the moveset enabled?
	bool walk_enabled;

	//Which corner of the sprite to check in fight collision
	AICollisionCheck collide_type;

	//The previous displacement of sprite
	Sint16 dx_prev, dy_prev;

	//The state of the sprite
	AIFightState fight_state;

	//The conditions for sprite visibility
	VarCondition visible;

	//This variable is used to see if the sprite is playing it's KO animation
	bool ko_playing;

	void ResetFrame(GameMode mode);
	int MoveToX(Sprite &s, Range &range);
	int MoveToY(Sprite &s, Range &range);
	void MoveToSprite(Sprite &s, int vel, Range &range);
	bool FightCollide(Sprite &s, Range &range);

	bool DamageValid(Sprite &s)
	{
		//Get the y coordinates where these sprites are standing
		Sint16 Y = y + box_v_walk.y + box_v_walk.h, SY = s.y + s.box_v_walk.y + s.box_v_walk.h;

		Sint16 diff = Y - SY;
		if (diff < 0)
			diff *= -1;

		if (diff < PLANE_WIDTH)
			return true;

		return false;
	}

public:

	Sprite();
	~Sprite(){}

	void Visible(bool val){ visible.Result(val); }
	bool Visible(){ return visible.Result(); }

	void KoPlaying(bool val){ ko_playing = val; }
	bool KoPlaying(){ return ko_playing; }

	void CalcVis(VarMap &varmap) { visible.Calc(varmap); }

	void X(Sint16 X){ x = X; }
	void Y(Sint16 Y){ y = Y; }

	Sint16 X(){ return x; }
	Sint16 Y(){ return y; }

	void WalkPattern(MovementSet set) { walk_pattern = set; walk_enabled = true; }

	void Move() { x += vx; y += vy; }
	void UndoMove() { x -= vx; y -= vy; }
	void Stop() { vx = 0; vy = 0; }

	void RunFromSprite(Sprite sp, SDL_Rect bounds);

	void XVel(Sint16 val){ vx = val; }
	void YVel(Sint16 val){ vy = val; }

	Sint16 XVel(){ return vx; }
	Sint16 YVel(){ return vy; }

	std::string Name() { return name; }

	Uint16 Width() { return clip.w; }
	Uint16 Height() { return clip.h; }

	std::string Combo() { return combo; }
	void Combo(std::string val) { combo = val; }

	SDL_Rect BoundRect()
	{
		SDL_Rect rect;
		rect.x = x + box_v_walk.x;
		rect.y = y + box_v_walk.y;
		rect.w = box_v_walk.w;
		rect.h = box_v_walk.h;
		return rect;
	}

	void Clip(Sint16 X, Sint16 Y) { clip.x = clip.w*X; clip.y = clip.h*Y; }
	void Clip(SDL_Rect rect){ clip = rect; }

	void BoxV(SDL_Rect rect){ box_v = rect; }
	void BoxD(SDL_Rect rect){ box_d = rect; }

	SDL_Rect BoxV()
	{
		SDL_Rect rect;
		rect.x = x + box_v.x;
		rect.y = y + box_v.y;
		rect.w = box_v.w;
		rect.h = box_v.h;
		return rect;
	}

	SDL_Rect BoxD()
	{
		SDL_Rect rect;
		rect.x = x + box_d.x;
		rect.y = y + box_d.y;
		rect.w = box_d.w;
		rect.h = box_d.h;
		return rect;
	}

	double Distance(Sprite s) { return sqrt(double((x - s.x)*(x - s.x) + (y - s.y)*(y - s.y))); }
	void EffectImg(bool visible) { img_eff.visible = visible; }
	bool LastFrame() { return fight_move.frame_cur >= (fight_move.frame_total - 1); }

	void SetVel(Direction dir, int val);
	bool TakingDamage(Sprite &sp);
	bool DealingDamage(Sprite &sp);
	int TakeDamage(Sprite &s, GameObject &attacker, GameObject &defender);

	void Load(rapidxml::xml_node<char> *node, std::vector<FightMoveSet> &fight_ms, std::vector<MovementSet> &movement_ms, VarMap &varmap);

	void Draw(Sint16 XOffset = 0, Sint16 YOffset = 0);

	void Walk(GameMode mode, Direction direction);
	void Walk(GameMode mode);

	bool FullCollide(SDL_Rect box);
	bool InsideRect(SDL_Rect box);

	void TraversePath(int speed);
	void UpdateFrame(GameMode mode, bool repeat = false);
	void UndoUpdateFrame();

	void UpdateMove(std::string new_combo);
	void ForceUpdateMove(std::string new_combo);

	void UpdateMove(Uint16 new_combo);
	void ForceUpdateMove(Uint16 new_combo);

	void Attack(Sprite &s, int vel);
};

#endif //SPRITE_H