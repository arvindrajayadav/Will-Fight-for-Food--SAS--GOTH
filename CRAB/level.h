#ifndef LEVEL_H
#define LEVEL_H

#include "AssetManager.h"
#include "sprite.h"
#include "button.h"
#include "fight.h"
#include "FightInput.h"
#include "object.h"
#include "LevelLayer.h"
#include "gameevent.h"
#include "LevelExit.h"

enum LevelResultType { LR_NONE, LR_OBJECT, LR_LEVEL, LR_GAMEOVER };

struct LevelResult
{
	LevelResultType type;
	std::string val;
	Sint16 x, y;

	LevelResult()
	{
		type = LR_NONE;
		val = "";
		x = -1;
		y = -1;
	}
};

class Level
{
	//The layers in the level
	std::vector<LevelLayer> layer;

	//The dimensions
	Uint16 w, h, obj_layer;

	//The area you can move in
	SDL_Rect walk;

	//The non-walk able areas in the level
	std::vector<SDL_Rect> restricted_area;

	//The area we display to the player
	SDL_Rect camera;

	//The player sprite location in the object list
	int player_index;

	//The order in which to draw the sprites
	std::vector<Uint16> obj_seq;

	//Number of frames skipped while animating sprites
	Uint16 frames_to_skip, frames_skipped;

	//The set of keys and their required info
	FightInput input;

	//The exits to different levels
	std::vector<LevelExit> level_exit;

	//The background music for peace and fight mode
	std::string music_peace, music_fight;

	//The trigger rectangles in the level
	std::vector<SDL_Rect> trig_rect;

	//Is the world map accessible from this level?
	struct MapVis
	{
		bool normal, current;

		MapVis() { Set(true); }
		void Set(bool val) { normal = val; current = val; }
	} showmap;

	struct GameOverAnim
	{
		bool started, finished;

		GameOverAnim() { Reset(); }
		void Reset() { started = false; finished = false; }
	} goa;

	struct TalkNotify
	{
		Sint16 x, y;
		std::string img;
		bool active;

		TalkNotify() { x = 0; y = 0; active = false; img = "talk_notify"; }
		void Set(Sprite &s)
		{
			x = s.X() + s.Width() / 2 - gAssetManager.GetTexture(img).W() / 2;
			y = s.Y() - gAssetManager.GetTexture(img).H();
			active = true;
		}
		void Draw(SDL_Rect camera)
		{
			if (active)
				gAssetManager.DrawTexture(x - camera.x, y - camera.y, img);
		}
	} talk_notify;

	bool inside_exit, inside_map, player_in_combat;

	//Protected level functions
	bool CollidingWithObject(ObjMap &people, std::string &name, GameMode mode);
	bool CollidingWithLevel(Sprite &s, ObjMap &people, std::string &name, GameMode mode);
	int CollidingWithTrigger(Sprite &s);
	bool CollidingWithExit(LevelResult &res);

	void SortObjectsToDraw();
	void AnimateSprite(Sprite &s, GameMode mode, ObjMap &people);
	void Animate(GameMode mode, ObjMap &people);
	void Think(ObjMap &people, VarMap &var);

	void DrawObjects(ObjMap &people);
	void SetCamera();

	bool PlayerInCombat(ObjMap &people);

	//The fighting moves of all characters
	std::vector<FightMoveSet> fight_move;

	//Should we allow the player to switch between modes? Used only for tutorial level
	bool allow_mode_switch;

public:
	//The movement sets for sprites in levels
	std::vector<MovementSet> move_set;

	//The objects in the level, and the player character
	std::vector<Sprite> objects;

	Level(){ Reset(); }
	~Level(){}

	void Reset();

	void Camera(Sint16 x, Sint16 y, Sint16 w, Sint16 h)
	{
		camera.x = x;
		camera.y = y;
		camera.w = w;
		camera.h = h;
	}
	SDL_Rect Camera() { return camera; }

	void PlayerStop() { objects[player_index].Stop(); }
	std::string PlayerName() { return objects[player_index].Name(); }

	void ShowMap(bool val) { showmap.normal = val; }
	bool ShowMap() { return showmap.current; }

	void CalcVis(VarMap &varmap)
	{
		for (std::vector<Sprite>::iterator i = objects.begin(); i != objects.end(); ++i)
			i->CalcVis(varmap);
	}

	//Uses a cached value instead of recalculating it
	bool PlayerInCombat() { return player_in_combat; }

	void Load(std::string filename, const GameMode mode, VarMap &varmap, Sint16 player_x = -1, Sint16 player_y = -1);
	void LoadMoves(std::string filename);
	void LoadMovement(std::string filename);

	void HandleEvents(ObjMap &people, GameMode &mode, SDL_Event Event);
	LevelResult InternalEvents(ObjMap &people, GameMode mode, VarMap &var, int &CurTrigRect);

	void Draw(ObjMap &people);
};

#endif //LEVEL_H