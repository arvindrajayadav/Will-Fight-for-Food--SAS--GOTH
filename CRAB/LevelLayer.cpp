#include "LevelLayer.h"

void LevelLayer :: Load(rapidxml::xml_node<char> *node)
{
	LoadXY(x,y,node);

	clip.x = 0;
	clip.y = 0;
	LoadNum(clip.w,"w",node);
	LoadNum(clip.h,"h",node);

	LoadNum(vx,"vx",node);
	LoadNum(vy,"vy",node);

	LoadNum(delay,"delay",node);
	LoadStr(image,"img",node);

	if(node->first_attribute("repeat") != NULL)
		repeat = true;
	else
		repeat = false;
}

void LevelLayer :: Draw(SDL_Rect camera)
{
	gAssetManager.DrawTexture(x-camera.x,y-camera.y,image,&clip);
}

void LevelLayer :: InternalEvents()
{
	if(vx > 0 && vy > 0)
	{
		if(!timer.Started())
			timer.start();

		if(timer.get_ticks() > delay)
		{
			timer.stop();
			clip.x += vx;
			clip.y += vy;

			if(clip.x > x + gAssetManager.GetTexture(image).W() && clip.y > y + gAssetManager.GetTexture(image).H())
				if(repeat)
				{
					clip.x = 0;
					clip.y = 0;
				}
				else
				{
					vx = 0;
					vy = 0;
				}
		}
	}
}