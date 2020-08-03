#include "conversationdata.h"

void ConversationData :: Load(rapidxml::xml_node<char> *node)
{
	if(NodeValid(node))
	{
		best.Load(node->first_node("best"));
		worst.Load(node->first_node("worst"));

		if(!LoadNum(branch, "branch", node))
			branch = -1;

		LoadStr(context, "context", node);

		rapidxml::xml_node<char> *replynode = node->first_node("replies");
		if(NodeValid(replynode))
		{
			for(rapidxml::xml_node<char> *n = replynode->first_node("reply"); n != NULL; n = n->next_sibling("reply"))
				reply.push_back(n->first_attribute("text")->value());
		}
	}
}