#ifndef GAME_H
#define GAME_H

#include "gamestate_container.h"
#include "AssetManager.h"
#include "menu.h"
#include "cursor.h"
#include "level.h"
#include "hud.h"
#include "map.h"
#include "gameeventmanager.h"
#include "GameClock.h"
#include "Inventory.h"
#include "savefile.h"
#include "LoadingScreen.h"

class Game : public GameState
{
private:
	enum State
	{
		STATE_GAME,
		STATE_MAP,
		STATE_OPTION,
		STATE_OBJECTIVE,
		STATE_INVENTORY,
		STATE_LOSE_MENU,
		STATE_LOSE_LOAD
	} state;

	//These things don't need to be saved
	Level level;
	Map map;
	HUD hud;
	std::vector<EventResult> event_res;
	std::string load_bg;

	struct LevelPaths
	{
		std::string layout, asset;
		LevelPaths(){ layout = ""; asset = ""; }
	};

	//List of levels
	std::unordered_map<std::string, LevelPaths> levelmap;

	//The file where all the item information is stored
	std::string itemfile;

	GameMode mode;
	bool TalkKeyDown;
	int CurTrigRect;

	//These things need to be saved
	GameEventManager gem;
	Inventory inv;
	GameObjective objective;

	//Player location
	std::string curloc;

	//The last object we interacted with
	std::string lastobj;

	//Keeps track of the time player has spent in the game
	GameClock clock;

	//The name of the auto save and quick save files
	SaveFile savefile;

	void StartNewGame();
	void LoadGame(std::string filename);

public:
	Game(){ StartNewGame(); }
	Game(std::string filename){ LoadGame(filename); }
	~Game(){ levelmap.clear(); }

	void Init(std::string filename);

	void HandleEvents(SDL_Event& Event, bool& ShouldChangeState, GameStateID& NewStateID);
	void InternalEvents(bool& ShouldChangeState, GameStateID& NewStateID);
	void Draw();

	bool ApplyResult();
	void ApplyResult(LevelResult result);

	void Quit(bool& ShouldChangeState, GameStateID& NewStateID)
	{
		ShouldChangeState = true;
		NewStateID = GAMESTATE_MAIN_MENU;
		gAssetManager.LoadMap(gFilePath.mainmenu_r);
	}

	//Stuff to load stuff
	void LoadLevelMap(std::string filename);
	void LoadLevel(std::string name, Sint16 player_x = -1, Sint16 player_y = -1);
	Item LoadItem(std::string id);

	void LoadPlayerTexture()
	{
		gAssetManager.AddTexture("player", cur_img_path.normal);
		gAssetManager.AddTexture("player_d", cur_img_path.digi);
		gAssetManager.AddTexture("player_y", cur_img_path.young);
		gAssetManager.AddTexture("player_f", cur_img_path.face);
		gAssetManager.AddTexture("player_yf", cur_img_path.face_young);
	}

	void LoadState(std::string filename);
	void SaveState(std::string filename);
};

#endif //GAME_H