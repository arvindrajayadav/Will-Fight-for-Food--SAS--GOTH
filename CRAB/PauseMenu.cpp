#include "PauseMenu.h"

void PauseMenu :: Load(rapidxml::xml_node<char> *node)
{
	if(NodeValid(node))
	{
		menu.Load(node->first_node("menu"));
		back.Load(node->first_node("back"));

		rapidxml::xml_node<char> *savenode = node->first_node("save");
		if (NodeValid(savenode))
		{
			save_prompt.Load(savenode);

			rapidxml::xml_node<char> *savebgnode = savenode->first_node("bg");
			if (NodeValid(savebgnode))
			{
				LoadStr(save_bg, "img", savebgnode);
				LoadXY(save_x, save_y, savebgnode);
			}
		}

		rapidxml::xml_node<char> *bgnode = node->first_node("bg");
		if(NodeValid(bgnode))
		{
			LoadStr(bg, "img", bgnode);
			LoadXY(x, y, bgnode);
		}
	}
}

void PauseMenu :: Draw()
{
	switch(state)
	{
		case STATE_LOAD:
			gAssetManager.DrawTexture(0, 0, gFilePath.load_bg);
			gSaveMenu.Draw();
			back.Draw();
			break;
		case STATE_OPTION:
			option_menu.Draw(back);
			break;
		case STATE_HELP:
			ss_help.Draw(back);
			break;
		case STATE_NORMAL:
			gAssetManager.DrawTexture(x,y,bg);
			menu.Draw();
			break;
		case STATE_SAVE:
			gAssetManager.DrawTexture(save_x,save_y,save_bg);
			save_prompt.Draw();
			break;
		default:break;
	}
}

PauseSignal PauseMenu :: HandleEvents(SDL_Event Event)
{
	switch(state)
	{
		case STATE_LOAD:
			if(gSaveMenu.HandleEvents(Event))
				return PS_LOAD;

			if(back.HandleEvents(Event) == BUTTON_LCLICK || (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_ESCAPE))
				state = STATE_NORMAL;
			break;
		case STATE_OPTION:
			if(option_menu.HandleEvents(back,Event))
				state = STATE_NORMAL;
			break;
		case STATE_HELP:
			if(ss_help.HandleEvents(back,Event))
				state = STATE_NORMAL;
			break;
		case STATE_SAVE:
			if(save_prompt.HandleEvents(Event))
			{
				state = STATE_NORMAL;
				return PS_SAVE;
			}

			if(Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_ESCAPE)
				state = STATE_NORMAL;
			break;
		case STATE_NORMAL:
			switch(menu.HandleEvents(Event))
			{
				case 0: state = STATE_SAVE; break;
				case 1: state = STATE_LOAD; gSaveMenu.ScanDir(); break; 
				case 2: state = STATE_OPTION; break; 
				case 3: state = STATE_HELP; break; 
				case 4: return PS_QUIT;
				case 5: STATE_NORMAL; return PS_RESUME; 
				default: break;
			}

			if(Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_ESCAPE)
				return PS_RESUME;
		default:break;
	}

	return PS_NONE;
}