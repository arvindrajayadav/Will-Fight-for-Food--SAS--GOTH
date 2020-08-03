#include "slider.h"

void Slider :: Load(rapidxml::xml_node<char> * node, Uint8 val)
{
	if(NodeValid(node))
	{
		LoadXY(x, y, node);
		value = val;

		ImageData dat;
		LoadStr(background, "bar", node);
		gAssetManager.GetTexture(background,dat);
		w = dat.W();
		h = dat.H();

		LoadStr(knob, "knob", node);
		gAssetManager.GetTexture(knob,dat);
		kw = dat.W();
		kh = dat.H();

		LoadStr(knob_s, "knob_s", node);

		kx = x+ (w*value/MIX_MAX_VOLUME);
		ky = y;
		grabbed = false;

		caption.Load(node->first_node("caption"));
	}
}

bool Slider :: HandleEvents(SDL_Event Event)
{
	switch(Event.type)
	{
		case SDL_MOUSEBUTTONUP:
			grabbed = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			{
				Uint16 mousex = gMouse.ButtonX(), mousey = gMouse.ButtonY();
				if (mousex >= kx && mousex <= kx+kw && mousey >= ky && mousey <= ky+kh)
					grabbed = true; // This knob was clicked on
			}
			break;
		case SDL_MOUSEMOTION:
			{
				Uint16 mousex = gMouse.MotionX();
				if (grabbed)
				{
					int dx = mousex - x;
					if (dx < 0) dx = 0;
					else if (dx > w-kw)
						dx = w-kw;
					value = MIX_MAX_VOLUME * dx / w;
					kx = x+dx;
				}

				return true;
			}
			break;
		default:
			break;
	}

	return false;
}

void Slider :: Draw()
{
	gAssetManager.DrawTexture(x,y,background);
	caption.Draw(false);

	if(grabbed)
		gAssetManager.DrawTexture(kx,ky,knob_s);
	else
		gAssetManager.DrawTexture(kx,ky,knob);
}