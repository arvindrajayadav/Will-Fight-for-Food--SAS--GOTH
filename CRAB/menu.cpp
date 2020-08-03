//=============================================================================
// Author:   Arvind
// Purpose:  Menu functions
//=============================================================================
#include "menu.h"

void Menu :: Reset()
{
	hoverindex = -1;
	for(std::vector<Button>::iterator b = button.begin(); b != button.end(); ++b)
		b->Reset();
}
//------------------------------------------------------------------------
// Purpose: Load the menu from a file
//------------------------------------------------------------------------
void Menu :: Load(rapidxml::xml_node<char> *node)
{
	if(NodeValid(node))
	{
		for(rapidxml::xml_node<char> *n = node->first_node("button"); n != NULL; n = n->next_sibling("button"))
		{
			Button b;
			b.Load(n);
			button.push_back(b);
		}

		std::string usekey;
		LoadStr(usekey,"keyboard",node,false);
		if(usekey == "true")
		{
			usekeyboard = true;
			AssignPaths();
		}
		else
			usekeyboard = false;
	}
}
//------------------------------------------------------------------------
// Purpose: Draw the menu
//------------------------------------------------------------------------
void Menu :: Draw()
{
	for(std::vector<Button>::iterator it = button.begin(); it != button.end(); ++it)
		it->Draw();
}
//------------------------------------------------------------------------
// Purpose: Event Handling InternalEvents
// The reason this function doesn't declare its own Event object is because
// a menu might not be the only object in a game state
//------------------------------------------------------------------------
int Menu :: HandleEvents(SDL_Event Event)
{
	//The mouse event handling bit
	Uint16 i = 0;
	hoverindex = -1;
	for(std::vector<Button>::iterator it = button.begin(); it != button.end(); ++it, ++i)
		if(it->HandleEvents(Event) == BUTTON_LCLICK)
			return i;
		else if(it->Hovered())
			hoverindex = i;

	//The keyboard event handling bit
	if(Event.type == SDL_KEYDOWN && usekeyboard)
	{
		HandleKeyboard(Event.key.keysym.sym);
		if( hoverindex != -1 && gInputVal[I_TALK].Equals(Event.key.keysym.sym))
			return hoverindex;
	}
	return -1;
}
//------------------------------------------------------------------------
// Purpose: Assign traversal paths
//------------------------------------------------------------------------
void Menu :: AssignPaths()
{
	vpath.clear();
	hpath.clear();

	for(Uint16 i = 0; i < button.size(); i++)
	{
		hpath.push_back(i);
		vpath.push_back(i);
	}
}
void Menu :: AssignPaths(std::vector<Uint16> &VerticalPath, std::vector<Uint16> &HorizontalPath)
{
	vpath.clear();
	hpath.clear();

	for(std::vector<Uint16>::iterator i = HorizontalPath.begin(); i != HorizontalPath.end(); ++i)
		hpath.push_back(*i);

	for(std::vector<Uint16>::iterator i = VerticalPath.begin(); i != VerticalPath.end(); ++i)
		vpath.push_back(*i);
}
//------------------------------------------------------------------------
// Purpose: Find the next button in our path
//------------------------------------------------------------------------
void Menu :: NextButton(std::vector<Uint16> &path)
{
	if(hoverindex == -1)
	{
		for(Uint16 pos = 0; pos < path.size(); pos++)
			if(button[path[pos]].Visible() == true)
			{
				hoverindex = path[pos];
				break;
			}
	}
	else
	{
		Uint16 curpos = 0;
		for(; curpos < path.size(); curpos++)
			if(path[curpos] == hoverindex)
				break;

		for(Uint16 nextloc = (curpos+1) % button.size(); nextloc != curpos; nextloc = (nextloc+1) % button.size())
			if(button[nextloc].Visible() == true)
			{
				hoverindex = path[nextloc];
				break;
			}
	}
}

void Menu :: PrevButton(std::vector<Uint16> &path)
{
	if(hoverindex == -1)
	{
		for(Uint16 pos = 0; pos < path.size(); pos++)
			if(button[path[pos]].Visible() == true)
			{
				hoverindex = path[pos];
				break;
			}
	}
	else
	{
		Uint16 curpos = 0;
		for(; curpos < path.size(); curpos++)
			if(path[curpos] == hoverindex)
				break;

		int nextloc = curpos-1;
		while(nextloc != curpos)
		{
			if(nextloc < 0)
				nextloc = button.size()-1;

			if(button[nextloc].Visible() == true)
			{
				hoverindex = path[nextloc];
				break;
			}

			nextloc--;
		}
	}
}

void Menu :: HandleKeyboard(SDLKey Direction)
{
	if(!button.empty() && usekeyboard)
	{
		Uint16 i = 0;
		for(std::vector<Button>::iterator it = button.begin(); it != button.end(); ++it, ++i)
			if(it->Hovered())
			{
				hoverindex = i;
				break;
			}

		if(gInputVal[I_DOWN].Equals(Direction))
			NextButton(vpath);
		else if(gInputVal[I_UP].Equals(Direction))
			PrevButton(vpath);
		else if(gInputVal[I_RIGHT].Equals(Direction))
			NextButton(hpath);
		else if(gInputVal[I_LEFT].Equals(Direction))
			PrevButton(hpath);

	i=0;
	for(std::vector<Button>::iterator it = button.begin(); it != button.end(); ++it, ++i)
		if(i==hoverindex)
			it->Hovered(true);
		else
			it->Hovered(false);
	}
}