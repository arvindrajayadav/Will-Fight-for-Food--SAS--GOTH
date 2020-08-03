//=============================================================================
// Author:   Arvind
// Purpose:  A basic screen that flashes an image for a time interval
//=============================================================================
#ifndef SPLASH_H
#define SPLASH_H

#include "graphics.h"
#include "gamestates.h"
#include "gamestate_container.h"
#include "timer.h"

//------------------------------------------------------------------------
// Purpose: Splash screen class
//------------------------------------------------------------------------
class Splash : public GameState
{
	ImageData background;
	Mix_Music *logo_music;

	float x,y;
	bool first_run, load_complete;

public:
	Splash();
	~Splash();
	void HandleEvents(SDL_Event& Event, bool& ShouldChangeState, GameStateID& NewStateID);
	void InternalEvents(bool& ShouldChangeState, GameStateID& NewStateID);
	void Draw();
};

#endif //SPLASH_H