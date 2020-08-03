//=============================================================================
// Author:   Arvind
// Purpose:  Dialog box!
//=============================================================================
#include "dialogbox.h"

//------------------------------------------------------------------------
// Purpose: Default constructor
//------------------------------------------------------------------------
GameDialogBox :: GameDialogBox()
{
	background = "";
	line_width=0;
	alt_active = false;

	x = 0;
	y = 0;
	w = 0;
	h = 0;
	font = 0;
	alt_x = 315;
	alt_y = 220;
	line_height = 30;
	text_offset.x = 35;
	text_offset.y = 35;
	text_col = black;
	keydown = false;
}
//------------------------------------------------------------------------
// Purpose: Load stuff
//------------------------------------------------------------------------
void GameDialogBox :: Load(rapidxml::xml_node<char> *node)
{
	rapidxml::xml_node<char> *dlg = node->first_node("dialog");
	if(NodeValid(node))
	{
		LoadXY(x, y, dlg->first_node("pos"));
		if(dlg->first_node("alt") != NULL)
			LoadXY(alt_x, alt_y, dlg->first_node("alt"));

		LoadNum(line_width, "line_width", dlg);
		LoadNum(line_height, "line_height", dlg);

		if(NodeValid(dlg->first_node("offset")))
			LoadXY(text_offset.x, text_offset.y, dlg->first_node("offset"));

		LoadNum(font, "font", dlg);
		LoadStr(background, "bg", dlg);
		w = gAssetManager.GetTexture(background).W();
		h = gAssetManager.GetTexture(background).H();
		LoadColor(text_col,dlg);

		//skip_key.Load(node->first_node("skip"));
	}
}
//------------------------------------------------------------------------
// Purpose: Animate the dialog box
//------------------------------------------------------------------------
void GameDialogBox :: InternalEvents()
{
	//skip_key.InternalEvents();
}
//------------------------------------------------------------------------
// Purpose: Draw the dialog box
//------------------------------------------------------------------------
void GameDialogBox :: Draw(std::string message, bool alt)
{
	if(alt)
	{
		gAssetManager.DrawTexture(alt_x, alt_y, background );
		gTextManager.Draw(alt_x + text_offset.x, alt_y + text_offset.y, message, text_col, font, line_width, line_height);
		alt_active = true;
	}
	else
	{
		gAssetManager.DrawTexture(x, y, background);
		gTextManager.Draw(x + text_offset.x, y + text_offset.y, message, text_col, font, line_width, line_height);
		alt_active = false;
	}
}
//------------------------------------------------------------------------
// Purpose: Handle input
//------------------------------------------------------------------------
bool GameDialogBox :: HandleEvents ( SDL_Event Event )
{
	if(gInputVal[I_TALK].Equals(Event.key.keysym.sym))
		if(Event.type == SDL_KEYUP && keydown)
		{
			keydown = false;
			return true;
		}
		else if(Event.type == SDL_KEYDOWN)
			keydown = true;

	if (Event.type == SDL_MOUSEBUTTONDOWN)
	{
		//The mouse button pressed, then released, comprises of a click action
		if(alt_active)
		{
			if(gMouse.ButtonX() >= alt_x && gMouse.ButtonX() < alt_x + w && gMouse.ButtonY() >= alt_y && gMouse.ButtonY() < alt_y + h)
				keydown = true;
		}
		else
		{
			if(gMouse.ButtonX() >= x && gMouse.ButtonX() < x + w && gMouse.ButtonY() >= y && gMouse.ButtonY() < y + h)
				keydown = true;
		}
	}
	else if(Event.type == SDL_MOUSEBUTTONUP && keydown)
	{
		keydown = false;

		if(alt_active)
		{
			if(gMouse.ButtonX() >= alt_x && gMouse.ButtonX() < alt_x + w && gMouse.ButtonY() >= alt_y && gMouse.ButtonY() < alt_y + h)
				return true;
		}
		else
		{
			if(gMouse.ButtonX() >= x && gMouse.ButtonX() < x + w && gMouse.ButtonY() >= y && gMouse.ButtonY() < y + h)
				return true;
		}
	}

	return false;
}