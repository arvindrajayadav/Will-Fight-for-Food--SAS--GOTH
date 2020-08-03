#ifndef CONVERSATIONSET_H
#define CONVERSATIONSET_H

#include "AssetManager.h"
#include "menu.h"
#include "object.h"
#include "dialogbox.h"
#include "HintBox.h"
#include "conversationdata.h"
#include "SentenceMaker.h"

class ConversationHandler
{
protected:
	enum ConvState
	{
		STATE_BODYLANG,
		STATE_OPINION,
		STATE_TONE,
		STATE_SUBMIT
	} curstate;

	//The background of the conversation (mainly used to dim the background)
	std::string bg;

	//The 4 menus for selecting emotion menus, and the 3 emotions
	Menu me_top, me_body, me_opinion, me_tone;
	Button bu_submit;

	//Conversation data we are comparing
	ConversationData ref;
	ConversationPref input, like, hate;

	//The default images for the top buttons
	ButtonImage img_top;

	HintBox hintbox;
	GameDialogBox contextbox;

	//The sentence maker previews the player reply
	SentenceMaker sm;

	//The bonuses you get in conversation
	struct ConversationBonus
	{
		int ref_body, ref_tone, ref_opinion;
		int like_body, like_tone;

		ConversationBonus()
		{
			ref_body = 7; ref_tone = 7; ref_opinion = 7;
			like_body = 4; like_tone = 4;
		}

		void Load(rapidxml::xml_node<char> *node)
		{
			if(NodeValid(node))
			{
				LoadNum(ref_body, "r_body", node);
				LoadNum(ref_tone, "r_tone", node);
				LoadNum(ref_opinion, "r_opinion", node);
				LoadNum(like_body, "l_body", node);
				LoadNum(like_tone, "l_tone", node);
			}
		}
	} value;

	std::vector<int> result_threshold;

	//Help related stuff
	Button help;
	std::string help_overlay;
	bool help_show;

public:
	ConversationHandler(){ curstate = STATE_BODYLANG; help_show = false; }
	~ConversationHandler(){}

	void Load(std::string layoutfile, std::string sentencefile);

	void Draw(std::vector<std::string> &hint, std::string context);
	bool HandleEvents(SDL_Event Event, int &result);
	int Result();

	void SetReference(ConversationData data, ConversationPref npc_likes, ConversationPref npc_hates)
	{
		ref = data;
		like = npc_likes;
		hate = npc_hates;
	}

	Uint16 GetSelectedReply(int result)
	{
		for(Uint16 i = 0; i < result_threshold.size(); ++i)
			if(result < result_threshold[i])
				return i;

		return result_threshold.size()-1;
	}

	void DrawBackground() { gAssetManager.DrawTexture(0,0,bg); }
};

#endif //CONVERSATIONSET_H