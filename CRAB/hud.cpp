//=============================================================================
// Author:   Arvind
// Purpose:  The tray where you have inventory, map and objectives icons
//=============================================================================
#include "hud.h"

void HUD::Load(std::string filename)
{
	XMLDoc conf(filename);
	if (conf.ready())
	{
		rapidxml::xml_node<char> *node = conf.Doc()->first_node("hud");
		if (NodeValid(node))
		{
			rapidxml::xml_node<char> *traynode = node->first_node("tray");
			if (NodeValid(traynode))
			{
				map.Load(traynode->first_node("map"));
				opt.Load(traynode->first_node("opt"));
				obj.Load(traynode->first_node("obj"));
				inv.Load(traynode->first_node("inv"));
				mode_img.Load(traynode->first_node("mode"));
				health_meter.Load(traynode->first_node("health"));
			}

			pause.Load(node->first_node("pause"));
			gom.Load(node->first_node("game_over"));
			back.Load(node->first_node("back"));

			rapidxml::xml_node<char> *notifynode = node->first_node("notify");
			if (NodeValid(notifynode))
			{
				notify_inv.Load(notifynode->first_node("inv"));
				notify_obj.Load(notifynode->first_node("obj"));
			}
		}
	}
}

void HUD::Draw(bool IsAnyoneViolent, int health, bool ShowMap)
{
	if (ShowMap)
		map.Draw();

	obj.Draw();
	inv.Draw();
	opt.Draw();

	if (unread_obj)
		notify_obj.Draw();

	if (unread_inv)
		notify_inv.Draw();

	if (IsAnyoneViolent)
		gAssetManager.DrawTexture(mode_img.x, mode_img.y, mode_img.img[MODE_FIGHT]);
	else
		gAssetManager.DrawTexture(mode_img.x, mode_img.y, mode_img.img[MODE_RPG]);

	health_meter.Draw(health);
}

HUDSignal HUD::HandleEvents(bool ShowMap, SDL_Event Event)
{
	if (Event.type == SDL_KEYDOWN)
		keydown = true;
	else if (Event.type == SDL_KEYUP && keydown)
	{
		keydown = false;

		if (gInputVal[I_MAP].Equals(Event.key.keysym.sym) && ShowMap)
			return HS_MAP;
		else if (gInputVal[I_PAUSE].Equals(Event.key.keysym.sym))
			return HS_PAUSE;
		else if (gInputVal[I_OBJECTIVE].Equals(Event.key.keysym.sym))
		{
			unread_obj = false;
			return HS_OBJ;
		}
		else if (gInputVal[I_INVENTORY].Equals(Event.key.keysym.sym))
		{
			unread_inv = false;
			return HS_INV;
		}
	}
	else
	{
		if (map.HandleEvents(Event) == BUTTON_LCLICK && ShowMap)
			return HS_MAP;
		else if (obj.HandleEvents(Event) == BUTTON_LCLICK)
		{
			unread_obj = false;
			return HS_OBJ;
		}
		else if (opt.HandleEvents(Event) == BUTTON_LCLICK)
			return HS_PAUSE;
		else if (inv.HandleEvents(Event) == BUTTON_LCLICK)
		{
			unread_inv = false;
			return HS_INV;
		}
	}

	return HS_NONE; //Nothing selected
}