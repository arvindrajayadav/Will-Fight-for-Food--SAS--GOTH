//=============================================================================
// Author:   Arvind
// Purpose:  TextArea functions
//=============================================================================

#include "textarea.h"

//------------------------------------------------------------------------
// Purpose: Handle Input
//------------------------------------------------------------------------
bool TextArea :: HandleEvents(SDL_Event Event)
{
	 //If a key was pressed
	if( Event.type == SDL_KEYDOWN)
	{
		//If the string less than maximum size
		//If the key is a space, 0-9, a-z and A-Z
		if( text.length() <= max_len)
			if( Event.key.keysym.unicode == (Uint16)' ' || Event.key.keysym.unicode == (Uint16)'.' ||
			( ( Event.key.keysym.unicode >= (Uint16)'0' ) && ( Event.key.keysym.unicode <= (Uint16)'9' ) ) ||
			( ( Event.key.keysym.unicode >= (Uint16)'A' ) && ( Event.key.keysym.unicode <= (Uint16)'Z' ) ) ||
			( ( Event.key.keysym.unicode >= (Uint16)'a' ) && ( Event.key.keysym.unicode <= (Uint16)'z' ) ) )
		{
			//Append the character to string
			text += (char)Event.key.keysym.unicode;
		}

		//If backspace was pressed and the string isn't blank
		if( ( Event.key.keysym.sym == SDLK_BACKSPACE ) && ( text.length() != 0 ) )
		{
			//Remove a character from the end
			text.erase( text.length() - 1 );
		}
	}

	if(gInputVal[I_TALK].Equals(Event.key.keysym.sym) && Event.type == SDL_KEYUP && text.length() != 0)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------
// Purpose: Draw
//------------------------------------------------------------------------
void TextArea :: Draw()
{
	title.Draw();
	gTextManager.Draw(x, y, text + "_", color, font);
}