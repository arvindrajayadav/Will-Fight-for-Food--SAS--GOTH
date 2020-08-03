#include "SentenceMaker.h"

void SentenceSegment :: Load(rapidxml::xml_node<char> *node)
{
	sentence.clear();

	if(NodeValid(node))
		for(rapidxml::xml_node<char> *n = node->first_node("sentence"); n != NULL; n= n->next_sibling("sentence"))
			sentence.push_back(n->first_attribute("text")->value());
}

void SentenceMaker :: Load(std::string filename)
{
	XMLDoc conf(filename);
	if(conf.ready())
	{
		rapidxml::xml_node<char> *node = conf.Doc()->first_node("wordsmith");
		if(NodeValid(node))
		{
			rapidxml::xml_node<char> *bgnode = node->first_node("bg");
			if(NodeValid(bgnode))
			{
				LoadStr(bg, "img", bgnode);
				LoadXY(bg_x, bg_y, bgnode);
			}

			rapidxml::xml_node<char> *textnode = node->first_node("text");
			if(NodeValid(textnode))
			{
				LoadXY(text_x, text_y, textnode);
				LoadNum(font, "font", textnode);
				LoadColor(col, textnode);
			}

			rapidxml::xml_node<char> *bodynode = node->first_node("body");
			if(NodeValid(bgnode))
			{
				LoadStr(body, "img", bodynode);
				LoadXY(body_x, body_y, bodynode);
				clip.clear();
				for(rapidxml::xml_node<char> *n = bodynode->first_node("clip"); n != NULL; n= n->next_sibling("clip"))
				{
					SDL_Rect r;
					LoadRect(r,n);
					clip.push_back(r);
				}
			}

			content.Load(node->first_node("content"));
			tone.Load(node->first_node("tone"));
		}
	}
}

void SentenceMaker :: Draw()
{
	gAssetManager.DrawTexture(bg_x,bg_y,bg);

	if(body_choice < clip.size())
		gAssetManager.DrawTexture(body_x,body_y,body,&clip[body_choice]);

	gTextManager.Draw(text_x,text_y,final,col,font);
}