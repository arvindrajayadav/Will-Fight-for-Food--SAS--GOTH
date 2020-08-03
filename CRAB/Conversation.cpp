#include "Conversation.h"

void ConversationHandler :: Load(std::string layoutfile, std::string sentencefile)
{
	XMLDoc conf(layoutfile);
	if(conf.ready())
	{
		rapidxml::xml_node<char> *node = conf.Doc()->first_node("conversation");
		if(NodeValid(node))
		{
			LoadStr(bg, "bg", node);
			LoadStr(help_overlay, "help_overlay", node);

			help.Load(node->first_node("help"));

			me_top.Load(node->first_node("top"));
			me_body.Load(node->first_node("body"));
			me_opinion.Load(node->first_node("opinion"));
			me_tone.Load(node->first_node("tone"));
			bu_submit.Load(node->first_node("submit"));

			img_top = me_top.button[0].Image();

			hintbox.Load(node->first_node("personality"));
			contextbox.Load(node->first_node("context"));

			value.Load(node->first_node("value"));

			result_threshold.clear();
			for(rapidxml::xml_node<char> *n = node->first_node("threshold")->first_node(); n != NULL; n = n->next_sibling())
			{
				int num = 0;
				LoadNum(num,"val",n);
				std::string str = n->name();
				if(str =="neg")
					num *= -1;

				result_threshold.push_back(num);
			}
		}
	}

	sm.Load(sentencefile);
}

void ConversationHandler :: Draw(std::vector<std::string> &hint, std::string context)
{
	me_top.Draw();
	bu_submit.Draw();

	sm.Draw();
	hintbox.Draw(hint);
	contextbox.Draw(context,false);

	switch(curstate)
	{
		case STATE_BODYLANG: me_body.Draw(); break;
		case STATE_OPINION:	me_opinion.Draw(); break;
		case STATE_TONE: me_tone.Draw(); break;
		default: break;
	}

	if(help_show)
		gAssetManager.DrawTexture(0,0,help_overlay);

	help.Draw();
}

bool ConversationHandler :: HandleEvents(SDL_Event Event, int &result)
{
	if(help_show)
	{
		if(help.HandleEvents(Event) == BUTTON_LCLICK)
			help_show = false;
	}
	else
	{
		int choice = me_top.HandleEvents(Event);
		if(choice != -1 && choice < 3)
			curstate = (ConvState)choice;

		if(bu_submit.HandleEvents(Event) == BUTTON_LCLICK && input.Valid())
		{
			result = Result();
			sm.Reset();

			for(std::vector<Button>::iterator i= me_top.button.begin(); i != me_top.button.end(); ++i)
				i->Image(img_top);

			return true;
		}

		switch(curstate)
		{
			case STATE_BODYLANG:
				{
					int langchoice = me_body.HandleEvents(Event);
					if(langchoice != -1)
					{
						input.body = langchoice;
						me_top.button[0].Image(me_body.button[langchoice]);
						curstate = STATE_OPINION;

						sm.SetBody(langchoice);
						sm.MakeSentence();
					}
				}
				break;
			case STATE_OPINION:
				{
					int opchoice = me_opinion.HandleEvents(Event);
					if(opchoice != -1)
					{
						input.opinion = opchoice;
						me_top.button[1].Image(me_opinion.button[opchoice]);
						curstate = STATE_TONE;

						sm.SetContent(opchoice);
						sm.MakeSentence();
					}
				}
				break;
			case STATE_TONE:
				{
					int tonechoice = me_tone.HandleEvents(Event);
					if(tonechoice != -1)
					{
						input.tone = tonechoice;
						me_top.button[2].Image(me_tone.button[tonechoice]);
						curstate = STATE_SUBMIT;

						sm.SetTone(tonechoice);
						sm.MakeSentence();
					}
				}
				break;
			default: break;
		}

		if(help.HandleEvents(Event) == BUTTON_LCLICK)
			help_show = true;
	}
	return false;
}

int ConversationHandler :: Result()
{
	int change = 0;

	//Body language check
	if(ref.best.body == input.body)
		change += value.ref_body;
	else if(ref.worst.body == input.body)
		change -= value.ref_body;

	if(like.body == input.body)
		change += value.like_body;
	else if(hate.body == input.body)
		change -= value.like_body;

	//Tone check
	if(ref.best.tone == input.tone)
		change += value.ref_tone;
	else if(ref.worst.tone == input.tone)
		change -= value.ref_tone;

	if(like.tone == input.tone)
		change += value.like_tone;
	else if(hate.tone == input.tone)
		change -= value.like_tone;

	//Opinion check
	if(ref.best.opinion == input.opinion)
		change += value.ref_opinion;
	else if(ref.worst.opinion == input.opinion)
		change -= value.ref_opinion;

	return change;
}