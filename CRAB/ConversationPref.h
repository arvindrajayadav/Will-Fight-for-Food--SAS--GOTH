#ifndef CONVERSATIONPREF_H
#define CONVERSATIONPREF_H

#define INVALID_PREF -1

#include "AssetManager.h"

class ConversationPref
{
public:
	int body, opinion, tone;

	ConversationPref() { Reset(); }

	void Reset()
	{
		body = INVALID_PREF;
		opinion = INVALID_PREF;
		tone = INVALID_PREF;
	}

	bool Valid()
	{
		if(body != INVALID_PREF && opinion != INVALID_PREF && tone != INVALID_PREF)
			return true;
		return false;
	}

	void Load(rapidxml::xml_node<char> *node)
	{
		if(NodeValid(node))
		{
			LoadNum(body, "body", node);
			LoadNum(opinion, "opinion", node);
			LoadNum(tone, "tone", node);
		}
	}
};

#endif //CONVERSATIONPREF_H