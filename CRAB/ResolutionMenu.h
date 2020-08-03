#ifndef RESOLUTIONMENU_H
#define RESOLUTIONMENU_H

#include "menu.h"
#include "textarea.h"

class ResolutionMenu : public Menu
{
	enum State
	{
		STATE_NORMAL,
		STATE_CHANGE,
		STATE_ENTER_W,
		STATE_ENTER_H
	} state;

	HoverInfo info;
	std::string def_info;
	Button change, cancel, custom;

	//Menu stores the button for each of the item in the dimension array
	std::vector<Dimension> dim;
	Dimension cur;

	//The images used for resolution menu buttons
	ButtonImage slot_img;
	SDL_Rect slot_pos;
	SDL_Color slot_col;
	Uint16 slot_font;

	//The resolution text input areas
	TextArea prompt_w, prompt_h;

public:
	ResolutionMenu(void){ state = STATE_NORMAL; }
	~ResolutionMenu(void){}

	void Load(rapidxml::xml_node<char> *node);
	void Draw();
	void HandleEvents(SDL_Event Event);

	Uint16 CurW() { return cur.w; }
	Uint16 CurH() { return cur.h; }
};

#endif //RESOLUTIONMENU_H