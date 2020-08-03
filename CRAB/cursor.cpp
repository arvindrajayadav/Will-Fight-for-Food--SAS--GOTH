//=============================================================================
// Author:   Arvind
// Purpose:  Cursor class
//=============================================================================
#include "cursor.h"

//------------------------------------------------------------------------
// Purpose: Handle Events
//------------------------------------------------------------------------
void Cursor :: HandleEvents( SDL_Event Event )
{
	if (Event.type == SDL_MOUSEMOTION)
	{
		motion_x = (Event.motion.x * gScreenSettings.Width()) / gScreenSettings.CurW();
		motion_y = (Event.motion.y * gScreenSettings.Height()) / gScreenSettings.CurH();
	}
	else if(Event.type == SDL_MOUSEBUTTONDOWN || Event.type == SDL_MOUSEBUTTONUP)
	{
		button_x = (Event.button.x * gScreenSettings.Width()) / gScreenSettings.CurW();
		button_y = (Event.button.y * gScreenSettings.Height()) / gScreenSettings.CurH();
	}
}

Cursor gMouse;