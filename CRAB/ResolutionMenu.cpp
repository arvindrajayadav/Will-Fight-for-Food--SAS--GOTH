#include "ResolutionMenu.h"

void ResolutionMenu :: Load(rapidxml::xml_node<char> *node)
{
	if(NodeValid(node))
	{
		cancel.Load(node->first_node("cancel"));
		change.Load(node->first_node("change"));

		custom.Load(node->first_node("custom"));
		prompt_w.Load(node->first_node("width"));
		prompt_h.Load(node->first_node("height"));

		info.Load(node->first_node("info"));
		def_info = info.Text();

		slot_img.Load(node->first_node("slot"));
		LoadRect(slot_pos,node->first_node("slot"));
		LoadColor(slot_col,node->first_node("slot"));
		LoadNum(slot_font, "font", node->first_node("slot"));

		info.Text( def_info + NumberToString(gScreenSettings.CurW()) + " x " + NumberToString(gScreenSettings.CurH()) );

		int count_slot = 0;
		for(rapidxml::xml_node<char> *n = node->first_node("dim"); n != NULL; n = n->next_sibling("dim"), count_slot++)
		{
			Dimension d;
			LoadNum(d.w, "x", n);
			LoadNum(d.h, "y", n);

			if(gScreenSettings.ValidDimension(d))
			{
				dim.push_back(d);

				std::string str = NumberToString(d.w) + "x" + NumberToString(d.h);
				Sint16 X = slot_pos.x + slot_pos.w*(count_slot%4), Y = slot_pos.y + slot_pos.h*(count_slot/4);

				Button b;
				b.Image(slot_img);
				b.Init(X,Y);
				b.AddCaption(str,X+b.W()/2,Y+b.H()/2,slot_col,slot_col,slot_font,ALIGN_CENTER);
				button.push_back(b);
			}
		}

		usekeyboard = false;

		cur.w = gScreenSettings.CurW();
		cur.h = gScreenSettings.CurH();
	}
}

void ResolutionMenu :: Draw()
{
	info.Draw();

	switch(state)
	{
	case STATE_NORMAL:
		change.Draw();
		break;
	case STATE_CHANGE:
		Menu::Draw();
		cancel.Draw();
		custom.Draw();
		break;
	case STATE_ENTER_W:
		prompt_w.Draw();
		cancel.Draw();
		break;
	case STATE_ENTER_H:
		prompt_h.Draw();
		cancel.Draw();
		break;
	default:break;
	}
}

void ResolutionMenu :: HandleEvents(SDL_Event Event)
{
	switch(state)
	{
	case STATE_NORMAL:
		if(change.HandleEvents(Event) == BUTTON_LCLICK)
			state = STATE_CHANGE;
		break;
	case STATE_CHANGE:
		{
			int choice = Menu::HandleEvents(Event);
			if(choice != -1)
			{
				cur = dim[choice];
				state = STATE_NORMAL;
			}

			if(custom.HandleEvents(Event) == BUTTON_LCLICK)
				state = STATE_ENTER_W;

			if(cancel.HandleEvents(Event) == BUTTON_LCLICK)
				state = STATE_NORMAL;
		}
		break;
	case STATE_ENTER_W:
		if(prompt_w.HandleEvents(Event))
		{
			cur.w = StringToNumber<Uint16>(prompt_w.Text());
			state = STATE_ENTER_H;
		}

		if(cancel.HandleEvents(Event) == BUTTON_LCLICK)
			state = STATE_NORMAL;
		break;
	case STATE_ENTER_H:
		if(prompt_h.HandleEvents(Event))
		{
			cur.h = StringToNumber<Uint16>(prompt_h.Text());
			state = STATE_NORMAL;
		}

		if(cancel.HandleEvents(Event) == BUTTON_LCLICK)
			state = STATE_NORMAL;
		break;
	default:break;
	}
}