//=============================================================================
// Author:   Arvind
// Purpose:  Dialog box!
//=============================================================================

#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H

#include "AssetManager.h"
#include "animation.h"
#include "button.h"

//------------------------------------------------------------------------
// Purpose: Dialog Box
//------------------------------------------------------------------------
class GameDialogBox
{
	//The skip key
	//Animation skip_key;

	//Coordinates of the dialog box
	Sint16 x, y, alt_x, alt_y;

	//line_width of a single line inside the dialog box
	Uint16 line_width, line_height, font, w, h;

	//If key is held down (key being held down and up comprises a press)
	bool keydown;

	struct
	{
		Sint16 x,y;
	} text_offset;

	//Fonts n surfaces
	std::string background;
	SDL_Color text_col;
	bool alt_active;

public:
	GameDialogBox();
	~GameDialogBox(){}
	void Load(rapidxml::xml_node<char> *node);

	void Draw(std::string message, bool alt = false);
	bool HandleEvents(SDL_Event);
	void InternalEvents();
};

#endif //DIALOG_BOX_H