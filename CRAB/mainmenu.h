#ifndef MAINMENU_H
#define MAINMENU_H

#include "gamestates.h"
#include "gamestate_container.h"
#include "menu.h"
#include "cursor.h"
#include "slider.h"
#include "textarea.h"
#include "KeyBindMenu.h"
#include "ModMenu.h"
#include "GameSaveMenu.h"
#include "SlideShow.h"
#include "OptionMenu.h"
#include "CustomizeMenu.h"

//------------------------------------------------------------------------
// Purpose: Main Menu class
//------------------------------------------------------------------------
class MainMenu : public GameState
{
	enum MenuState
	{
		STATE_NORMAL,
		STATE_OPTIONS,
		STATE_CREDITS,
		STATE_LOAD,
		STATE_DIFF,
		STATE_SKIN,
		STATE_MOD,
		STATE_HELP
	} CurState;

	Menu me_main, me_diff, me_mod;
	HoverInfo title_diff;
	Button back;
	ModMenu mod;
	CustomizeMenu me_customize;

public:
	MainMenu();
	~MainMenu(){}
	void HandleEvents(SDL_Event& Event, bool& ShouldChangeState, GameStateID& NewStateID);
	void InternalEvents(bool& ShouldChangeState, GameStateID& NewStateID);

	void Draw();
	void DrawCredits();

	void ChangeState(MenuState ms)
	{
		CurState = ms;

		//Continue button is only enabled if there is a save to load
		if (CurState == STATE_NORMAL)
			me_main.button.at(0).Visible(!gSaveMenu.Empty());
	}
};

#endif //MAINMENU_H