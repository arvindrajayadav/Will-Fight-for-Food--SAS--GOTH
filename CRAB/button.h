//=============================================================================
// Author:   Arvind
// Purpose:  Button class
//=============================================================================
#ifndef BUTTON_H
#define BUTTON_H

#include "AssetManager.h"
#include "MusicManager.h"
#include "TextManager.h"
#include "Caption.h"
#include "HoverInfo.h"

enum ButtonType
{
	BUTTON_SIMPLE,
	BUTTON_TOGGLE
};

// This is used to determine what happened to a button
// Was it dragged? was it dropped?
enum InteractionResult
{
	BUTTON_IGNORE,
	BUTTON_GRABBED,
	BUTTON_LCLICK,
	BUTTON_RCLICK
};

struct ButtonImage
{
	std::string normal, select, hover;

	void Load(rapidxml::xml_node<char> *node)
	{
		if(NodeValid(node))
		{
			LoadStr(normal, "img_b", node);
			LoadStr(select, "img_s", node);
			LoadStr(hover, "img_h", node);
		}
	}
};

class Button
{
protected:
	HoverInfo tooltip;
	Caption caption;

	//The position and dimensions of the button
	SDL_Rect dim;

	bool visible, hovered, mousepressed;

	//Can the player move this button?
	bool canmove;

	//Type of the button
	ButtonType type;

	//The button images
	ButtonImage image;

	//The sound effect played when button is clicked
	std::string click;

public:
	Button();
	~Button(){}

	void Load(rapidxml::xml_node<char> * node);
	void Init(Sint16 X, Sint16 Y, ButtonType Type=BUTTON_SIMPLE, Uint16 W=0, Uint16 H=0);

	void AddCaption(std::string str, Sint16 x, Sint16 y, SDL_Color col, SDL_Color col_s, Uint16 font, TextAlign align = ALIGN_LEFT);
	void AddHoverInfo(std::string str, Uint16 x, Uint16 y, SDL_Color col, Uint16 font, TextAlign align = ALIGN_LEFT);

	void Image(Button &b){ image = b.image; }
	void Image(ButtonImage &img){ image = img; }
	ButtonImage Image() { return image; }

	void Draw();
	InteractionResult HandleEvents(SDL_Event Event);
	void Reset();

	void Visible(bool val) { visible = val; }
	bool Visible() { return visible; }

	void Hovered(bool val) { hovered = val; }
	bool Hovered() { return hovered; }

	void Type(ButtonType newtype) { type = newtype; }
	ButtonType Type() { return type; }

	void X(Sint16 val) { dim.x = val; }
	Sint16 X() { return dim.x; }

	void Y(Sint16 val) { dim.y = val; }
	Sint16 Y() { return dim.y; }

	void W(Uint16 val) { dim.w = val; }
	Uint16 W() { return dim.w; }

	void H(Uint16 val) { dim.h = val; }
	Uint16 H() { return dim.h; }
};

#endif //BUTTON_H