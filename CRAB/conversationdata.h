#ifndef CONVDATA_H
#define CONVDATA_H

#include "ConversationPref.h"

class ConversationData
{
public:
	//The best and worst reply for the conversation
	ConversationPref best, worst;

	//The limit which decides branches in conversations
	Sint16 branch;

	//A small paragraph about the situation
	std::string context;

	//The set of replies, from best to worst
	std::vector<std::string> reply;

	ConversationData()
	{
		branch = -1;
		reply.clear();
		context = "";
	}

	void Reset()
	{
		best.Reset();
		worst.Reset();
	}

	void Load(rapidxml::xml_node<char> *node);
};

#endif //CONVDATA_H