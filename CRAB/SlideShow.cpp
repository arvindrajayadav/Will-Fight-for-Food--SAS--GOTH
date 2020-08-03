#include "SlideShow.h"

SlideShow ss_help;

void SlideShow :: Load(rapidxml::xml_node<char> *node)
{
	if(NodeValid(node))
	{
		LoadXY(x, y, node);
		prev.Load(node->first_node("prev"));
		next.Load(node->first_node("next"));

		img.clear();
		for(rapidxml::xml_node<char> *n = node->first_node("slide"); n != NULL; n = n->next_sibling("slide"))
			img.push_back(n->first_attribute("img")->value());

		cur_image = 0;

		std::string usekey;
		LoadStr(usekey,"keyboard",node,false);
		if(usekey == "true")
			usekeyboard = true;
		else
			usekeyboard = false;
	}
}

void SlideShow :: Draw(Button &back)
{
	if(!img.empty())
		gAssetManager.DrawTexture(x, y, img[cur_image]);

	if(cur_image > 0)
		prev.Draw();

	if(cur_image < img.size()-1)
		next.Draw();

	back.Draw();
}

bool SlideShow :: HandleEvents(Button &back, SDL_Event Event)
{
	if(cur_image > 0)
		if(prev.HandleEvents(Event) == BUTTON_LCLICK || (usekeyboard && Event.type == SDL_KEYDOWN && gInputVal[I_LEFT].Equals(Event.key.keysym.sym)))
			cur_image--;

	if(cur_image < img.size()-1)
		if(next.HandleEvents(Event) == BUTTON_LCLICK || (usekeyboard && Event.type == SDL_KEYDOWN && gInputVal[I_RIGHT].Equals(Event.key.keysym.sym)))
			cur_image++;

	if(back.HandleEvents(Event) == BUTTON_LCLICK || (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_ESCAPE))
		return true;

	return false;
}