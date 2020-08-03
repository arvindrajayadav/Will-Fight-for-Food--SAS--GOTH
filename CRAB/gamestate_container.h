//=============================================================================
// Author:   Arvind
// Purpose:  The game state virtual class
//=============================================================================
#ifndef GAMESTATE_CONTAINER_H
#define GAMESTATE_CONTAINER_H

#include "gamestates.h"
#include "AssetManager.h"
#include "MusicManager.h"
#include "TextManager.h"
#include "LoadingScreen.h"

//------------------------------------------------------------------------
// Purpose: Game State base class
//------------------------------------------------------------------------
class GameState
{
public:
	virtual void HandleEvents(SDL_Event& Event, bool& ShouldChangeState, GameStateID& NewStateID) = 0;
	virtual void InternalEvents(bool& ShouldChangeState, GameStateID& NewStateID) = 0;
	virtual void Draw() = 0;
	virtual ~GameState(){};
};

#endif //GAMESTATES_CONTAINER_H