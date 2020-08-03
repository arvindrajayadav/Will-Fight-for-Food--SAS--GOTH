#include "ObjectHandler.h"

void GameObjectHandler::Load(rapidxml::xml_node<char> *node)
{
	if (NodeValid(node))
	{
		rapidxml::xml_node<char> *Img = node->first_node("image");
		if (Img != NULL)
		{
			LoadXY(pos.img_x, pos.img_y, Img->first_node("pos"));
			LoadXY(alt.img_x, alt.img_y, Img->first_node("alt"));
		}

		rapidxml::xml_node<char> *Name = node->first_node("name");
		if (Name != NULL)
		{
			LoadXY(pos.name_x, pos.name_y, Name->first_node("pos"));
			LoadXY(alt.name_x, alt.name_y, Name->first_node("alt"));
			LoadNum(font_name, "font", Name);
			LoadColor(col_name, Name);
		}

		rapidxml::xml_node<char> *Approval = node->first_node("approval");
		if (Approval != NULL)
		{
			LoadXY(pos.approval_x, pos.approval_y, Approval->first_node("pos"));
			LoadXY(alt.approval_x, alt.approval_y, Approval->first_node("alt"));
			LoadNum(font_approval, "font", Approval);
			LoadColor(col_approval, Approval);
		}

		rapidxml::xml_node<char> *Notify = node->first_node("notify");
		if (Notify != NULL)
		{
			LoadXY(pos.notify_x, pos.notify_y, Notify->first_node("pos"));
			LoadXY(alt.notify_x, alt.notify_y, Notify->first_node("alt"));
			LoadNum(notify_delay, "delay", Notify);
			LoadNum(font_notify, "font", Notify);
		}
	}
}

void GameObjectHandler::Draw(GameObject &obj, bool alt_active)
{
	if (refresh_rating == true)
	{
		notify_start_time = SDL_GetTicks();
		refresh_rating = false;
	}

	//Draw the background
	if (alt_active)
	{
		if (obj.Opinion() < 34)
			gAssetManager.DrawTexture(alt.img_x, alt.img_y, obj.pic_hostile);
		else if (obj.Opinion() > 74)
			gAssetManager.DrawTexture(alt.img_x, alt.img_y, obj.pic_friendly);
		else
			gAssetManager.DrawTexture(alt.img_x, alt.img_y, obj.pic_normal);

		//Draw the name
		gTextManager.Draw(alt.name_x, alt.name_y, obj.name, col_name, font_name, ALIGN_CENTER);

		if (SDL_GetTicks() - notify_start_time < notify_delay)
		{
			if (rating_change > 0)
				gTextManager.Draw(alt.notify_x, alt.notify_y, "Opinion increased!", green, font_notify);
			else if (rating_change < 0)
				gTextManager.Draw(alt.notify_x, alt.notify_y, "Opinion decreased!", red, font_notify);
			else
				gTextManager.Draw(alt.notify_x, alt.notify_y, "No change!", black, font_notify);
		}
	}
	else
	{
		if (obj.Opinion() < 34)
			gAssetManager.DrawTexture(pos.img_x, pos.img_y, obj.pic_hostile);
		else if (obj.Opinion() > 74)
			gAssetManager.DrawTexture(pos.img_x, pos.img_y, obj.pic_friendly);
		else
			gAssetManager.DrawTexture(pos.img_x, pos.img_y, obj.pic_normal);

		//Draw the name
		gTextManager.Draw(pos.name_x, pos.name_y, obj.name, col_name, font_name, ALIGN_CENTER);

		if (SDL_GetTicks() - notify_start_time < notify_delay)
		{
			if (rating_change > 0)
				gTextManager.Draw(pos.notify_x, pos.notify_y, "Opinion increased!", green, font_notify);
			else if (rating_change < 0)
				gTextManager.Draw(pos.notify_x, pos.notify_y, "Opinion decreased!", red, font_notify);
			else
				gTextManager.Draw(pos.notify_x, pos.notify_y, "No change!", black, font_notify);
		}
	}
}