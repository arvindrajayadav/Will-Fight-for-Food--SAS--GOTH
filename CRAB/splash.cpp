//=============================================================================
// Author:   Arvind
// Purpose:  Splash functions
//=============================================================================
#include "splash.h"

//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
Splash :: Splash()
{
	//Load the background
	background.Load("res/gfx/pyrodactyl.png");
	logo_music = Mix_LoadMUS("res/sounds/music/pyrodactyl.ogg");

	x = 0.0f;
	y = 0.0f;
	load_complete = false;
	first_run = true;
	Mix_PlayMusic(logo_music,1);
}
//------------------------------------------------------------------------
// Purpose: Destructor
//------------------------------------------------------------------------
Splash :: ~Splash()
{
	background.Delete();
	Mix_FreeMusic(logo_music);
}
//------------------------------------------------------------------------
// Purpose: Event/input handling InternalEvents
//------------------------------------------------------------------------
void Splash :: HandleEvents(SDL_Event& Event, bool& ShouldChangeState, GameStateID& NewStateID)
{
}
//------------------------------------------------------------------------
// Purpose: Event/Input Independent InternalEvents
//------------------------------------------------------------------------
void Splash :: InternalEvents(bool& ShouldChangeState, GameStateID& NewStateID)
{
	if(first_run == false)
	{
		gLoadScreen.Init();
		gAssetManager.Init();
		gMusicManager.Init();
		gTextManager.Init();
		load_complete = true;
	}

	//Have we loaded everything? If yes, time to exit
	if(load_complete)
	{
		ShouldChangeState = true;
		NewStateID = GAMESTATE_MAIN_MENU;
		return;
	}
}
//------------------------------------------------------------------------
// Purpose: Drawing function
//------------------------------------------------------------------------
void Splash::Draw()
{
	background.Draw(x,y);
	first_run = false;
}