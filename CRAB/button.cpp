//=============================================================================
// Author:   Arvind
// Purpose:  Contains the button functions
//=============================================================================
#include "button.h"

//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
Button :: Button()
{
	dim.x = 0; dim.y = 0; dim.w = 0; dim.h = 0;
	visible = false;
	canmove = false;
	hovered = false;
	mousepressed = false;
	type = BUTTON_SIMPLE;
}
//------------------------------------------------------------------------
// Purpose: Load a new Button from a file
//------------------------------------------------------------------------
void Button :: Load(rapidxml::xml_node<char> * node)
{
	if(NodeValid(node))
	{
		LoadXY(dim.x, dim.y, node);
		image.Load(node);

		int num = 0;
		if(LoadNum(num, "type", node))
			type = (ButtonType) num;
		else
			type = BUTTON_SIMPLE;

		if(node->first_attribute("w") == NULL)
			dim.w = gAssetManager.GetTexture(image.select).W();
		else
			LoadNum(dim.w, "w", node);

		if(node->first_attribute("h") == NULL)
			dim.h = gAssetManager.GetTexture(image.select).H();
		else
			LoadNum(dim.h, "h", node);

		LoadStr(click, "click", node);

		tooltip.Load(node->first_node("tooltip"));
		caption.Load(node->first_node("caption"));

		visible = true;
		canmove = false;
		Reset();
	}
}
//------------------------------------------------------------------------
// Purpose: Load a new Button
//------------------------------------------------------------------------
void Button :: Init(Sint16 X, Sint16 Y, ButtonType Type, Uint16 W, Uint16 H)
{
	dim.x = X;
	dim.y = Y;
	type = Type;

	if(W != 0 || H != 0)
	{
		dim.w = W;
		dim.h = H;
	}
	else
	{
		ImageData dat = gAssetManager.GetTexture(image.select);
		dim.w = dat.W();
		dim.h = dat.H();
	}

	visible = true;
	canmove = false;
	Reset();
}
//------------------------------------------------------------------------
// Purpose: Reset the button
//------------------------------------------------------------------------
void Button :: Reset()
{
	mousepressed = false;
	hovered = false;
}
//------------------------------------------------------------------------
// Purpose: Draw
//------------------------------------------------------------------------
void Button :: Draw()
{
	if(visible)
	{
		if(mousepressed)
		{
			gAssetManager.DrawTexture(dim.x, dim.y, image.select);
			tooltip.Draw();
			caption.Draw(true);
		}
		else if(hovered)
		{
			if(type == BUTTON_TOGGLE)
				gAssetManager.DrawTexture(dim.x, dim.y, image.normal);

			gAssetManager.DrawTexture(dim.x, dim.y, image.hover);
			tooltip.Draw();
			caption.Draw(true);
		}
		else
		{
			gAssetManager.DrawTexture(dim.x, dim.y, image.normal);
			caption.Draw(false);
		}
	}
}
//------------------------------------------------------------------------
// Purpose: Handle input and stuff
//------------------------------------------------------------------------
InteractionResult Button :: HandleEvents(SDL_Event Event)
{
	if(visible)
		if(Event.type == SDL_MOUSEMOTION)
		{
			if(InsideBoundingBox(gMouse.MotionX(), gMouse.MotionY(), dim))
				hovered = true;
			else
				hovered = false;

			if(canmove && mousepressed)
			{
				dim.x = gMouse.MotionX() - (dim.w/2);
				dim.y = gMouse.MotionY() - (dim.h/2);
				return BUTTON_GRABBED;
			}
		}
		else if(Event.type == SDL_MOUSEBUTTONDOWN)
		{
			//The gMouse button pressed, then released, comprises of a click action
			if(InsideBoundingBox(gMouse.ButtonX(), gMouse.ButtonY(), dim))
				mousepressed = true;
		}
		else if(Event.type == SDL_MOUSEBUTTONUP && mousepressed)
		{
			Reset();
			if(InsideBoundingBox(gMouse.ButtonX(), gMouse.ButtonY(), dim))
			{
				mousepressed = false;
				if(Event.button.button == SDL_BUTTON_LEFT)
				{
					gMusicManager.PlayEffect(click);
					return BUTTON_LCLICK;
				}
				else if(Event.button.button == SDL_BUTTON_RIGHT)
					return BUTTON_RCLICK;
			}
		}
	return BUTTON_IGNORE;
}

//------------------------------------------------------------------------
// Purpose: Initialize caption or tooltip
//------------------------------------------------------------------------
void Button :: AddCaption(std::string str, Sint16 x, Sint16 y, SDL_Color col, SDL_Color col_s, Uint16 font, TextAlign align)
{
	caption.Init(str,x,y,col,col_s,font,align);
}

void Button :: AddHoverInfo(std::string str, Uint16 x, Uint16 y, SDL_Color col, Uint16 font, TextAlign align)
{
	tooltip.Init(str,x,y,col,font,align);
}