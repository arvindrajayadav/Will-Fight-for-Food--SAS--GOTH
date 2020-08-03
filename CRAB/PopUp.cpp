#include "PopUp.h"

bool PopUpHandler :: InternalEvents()
{
	if(!t.Started())
		t.start();

	if(t.get_ticks() > param.delay)
	{
		t.stop();
		return true;
	}

	return false;
}

void PopUpHandler :: Draw(std::string text)
{
	gTextManager.Draw(param.x,param.y,text,param.col,param.font);
	param.y += param.speed;
}