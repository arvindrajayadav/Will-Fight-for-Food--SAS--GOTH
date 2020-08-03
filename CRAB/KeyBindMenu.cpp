#include "KeyBindMenu.h"

void KeyBindMenu :: Load(rapidxml::xml_node<char> *node)
{
	if(NodeValid(node))
	{
		prim.Load(node->first_node("primary"));
		alt.Load(node->first_node("alt"));
		desc.Load(node->first_node("desc"));
		img.Load(node->first_node("img"));
		prompt.Load(node->first_node("prompt"));

		me_key.button.resize(I_TOTAL);
		me_alt.button.resize(I_TOTAL);

		for(int i=0;i<I_TOTAL;i++)
		{
			me_key.button[i].Image(img);
			me_key.button[i].Init(prim.x,prim.y + prim.inc*i);

			me_alt.button[i].Image(img);
			me_alt.button[i].Init(alt.x,alt.y + alt.inc*i);
		}

		img_w = gAssetManager.GetTexture(img.normal).W();
		img_h = gAssetManager.GetTexture(img.normal).H();
	}
}

void KeyBindMenu :: Draw()
{
	me_key.Draw();
	me_alt.Draw();

	for(int i=0;i<I_TOTAL;i++)
	{
		gTextManager.Draw(desc.x,desc.y+desc.inc*i,gInputVal[i].name,desc.col,desc.font);

		if(i == selected)
		{
			if(curstate == STATE_KEY)
				gTextManager.Draw(alt.x+img_w/2,alt.y+img_h/2+alt.inc*i,SDL_GetKeyName(gInputVal[i].alt),alt.col,alt.font,ALIGN_CENTER);
			else if(curstate == STATE_ALT)
				gTextManager.Draw(prim.x+img_w/2,prim.y+img_h/2+prim.inc*i,SDL_GetKeyName(gInputVal[i].key),prim.col,prim.font,ALIGN_CENTER);
		}
		else
		{
			gTextManager.Draw(alt.x+img_w/2,alt.y+img_h/2+alt.inc*i,SDL_GetKeyName(gInputVal[i].alt),alt.col,alt.font,ALIGN_CENTER);
			gTextManager.Draw(prim.x+img_w/2,prim.y+img_h/2+prim.inc*i,SDL_GetKeyName(gInputVal[i].key),prim.col,prim.font,ALIGN_CENTER);
		}
	}

	switch(curstate)
	{
		case STATE_KEY:
			gTextManager.Draw(prim.x+img_w/2,prim.y+img_h/2+prim.inc*selected,prompt.text,prompt.col,prompt.font,ALIGN_CENTER);
			break;
		case STATE_ALT:
			gTextManager.Draw(alt.x+img_w/2,alt.y+img_h/2+alt.inc*selected,prompt.text,prompt.col,prompt.font,ALIGN_CENTER);
			break;
		default:break;
	}
}

void KeyBindMenu :: HandleEvents(SDL_Event Event)
{
	switch(curstate)
	{
		case STATE_NORMAL:
			selected = me_key.HandleEvents(Event);
			if(selected != -1)
			{
				curstate = STATE_KEY;
				break;
			}

			selected = me_alt.HandleEvents(Event);
			if(selected != -1)
			{
				curstate = STATE_ALT;
				break;
			}

			break;
		case STATE_KEY:
			if(Event.type == SDL_KEYDOWN)
			{
				gInputVal[selected].key = Event.key.keysym.sym;
				curstate = STATE_NORMAL;
			}
			break;
		case STATE_ALT:
			if(Event.type == SDL_KEYDOWN)
			{
				gInputVal[selected].alt = Event.key.keysym.sym;
				curstate = STATE_NORMAL;
			}
			break;
		default:break;
	}
}