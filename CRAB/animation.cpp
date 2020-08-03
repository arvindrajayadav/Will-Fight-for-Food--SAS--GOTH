#include "animation.h"

void AnimationData :: Load(rapidxml::xml_node<char> *node)
{
	if(NodeValid(node))
	{
		LoadXY(x, y, node);
		LoadNum(interval, "delay", node);

		for(rapidxml::xml_node<char> *n = node->first_node("img"); n != NULL; n = n->next_sibling("img"))
			img.push_back(n->value());
	}
}

void AnimationHandler :: InternalEvents()
{
	if(!data.img.empty())
	{
		if(!timer.Started())
			timer.start();

		if(timer.get_ticks() > data.interval)
		{
			++cur_image;
			if(cur_image >= data.img.size())
				cur_image = 0;

			timer.stop();
		}
	}
}

void AnimationHandler :: Draw()
{
	if(!data.img.empty())
		gAssetManager.DrawTexture(data.x, data.y, data.img[cur_image]);
}