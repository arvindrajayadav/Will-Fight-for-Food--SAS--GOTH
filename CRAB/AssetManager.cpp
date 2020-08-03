//=============================================================================
// Author:   Arvind
// Purpose:  Contains the asset manager class - used to manage in-game std::string
//=============================================================================
#include "AssetManager.h"

//Stuff we use throughout the game
AssetManager gAssetManager;

bool unread_obj = false, unread_inv = false;

//------------------------------------------------------------------------
// Purpose: Load assets here.
//------------------------------------------------------------------------
void AssetManager::LoadMap(std::string filename, bool load, MapID id)
{
	for (SurfaceMap::iterator it = map[id].begin(); it != map[id].end(); ++it)
		it->second.Delete();

	map[id].clear();
	XMLDoc image_list(filename);
	if (image_list.ready())
	{
		rapidxml::xml_node<char> *node = image_list.Doc()->first_node("res");
		for (rapidxml::xml_node<char> *n = node->first_node("image"); n != NULL; n = n->next_sibling("image"))
		{
			rapidxml::xml_attribute<char> *name = n->first_attribute("name"), *path = n->first_attribute("path");
			if (name != NULL && path != NULL)
				map[id][name->value()].Load(path->value());

			if (load)
				gLoadScreen.Draw();
		}
	}
}

bool AssetManager::Init()
{
	invalid_img.Load("res/gfx/default.png");

	//Load common assets
	LoadMap(gFilePath.common, false, MAP_COMMON);

	//Load main menu assets
	LoadMap(gFilePath.mainmenu_r, false, MAP_CURRENT);

	mouse_img = map[MAP_COMMON]["mouse"];

	exit_data[DIR_DOWN].img = "exit_down";
	exit_data[DIR_UP].img = "exit_up";
	exit_data[DIR_LEFT].img = "exit_left";
	exit_data[DIR_RIGHT].img = "exit_right";

	exit_data[DIR_DOWN].clip.w = 55;
	exit_data[DIR_UP].clip.w = 55;
	exit_data[DIR_LEFT].clip.w = 65;
	exit_data[DIR_RIGHT].clip.w = 65;

	return true;
}

//------------------------------------------------------------------------
// Purpose: Texture stuff
//------------------------------------------------------------------------
void AssetManager::AddTexture(std::string id, std::string path, int mapindex)
{
	if (map[mapindex].count(id) > 0)
		FreeTexture(id, mapindex);

	map[mapindex][id].Load(path);
}

void AssetManager::AddTexture(std::string id, SDL_Surface* surface, int mapindex)
{
	if (map[mapindex].count(id) > 0)
		FreeTexture(id, mapindex);

	map[mapindex][id].Load(surface);
	SDL_FreeSurface(surface);
}

void AssetManager::GetTexture(std::string &id, ImageData &data)
{
	if (map[MAP_CURRENT].count(id) > 0)
		data = map[MAP_CURRENT][id];
	else if (map[MAP_COMMON].count(id) > 0)
		data = map[MAP_COMMON][id];
	else
		data = invalid_img;
}

ImageData& AssetManager::GetTexture(std::string &id)
{
	if (map[MAP_CURRENT].count(id) > 0)
		return map[MAP_CURRENT][id];
	else if (map[MAP_COMMON].count(id) > 0)
		return map[MAP_COMMON][id];

	return invalid_img;
}

void AssetManager::DrawTexture(Sint16 x, Sint16 y, std::string &id, SDL_Rect* clip)
{
	GetTexture(id).Draw(x, y, clip);
}

void AssetManager::DrawTextureCentered(Sint16 x, Sint16 y, std::string &id, SDL_Rect* clip)
{
	ImageData *i = &GetTexture(id);
	i->Draw(x - i->W() / 2, y - i->H() / 2, clip);
}

void AssetManager::DrawExit(Sint16 x, Sint16 y, Direction dir)
{
	if (dir > DIR_NONE && dir < DIR_TOTAL)
	{
		gAssetManager.DrawTexture(x - (exit_data[dir].clip.w / 2), y - (exit_data[dir].clip.h / 2), exit_data[dir].img, &exit_data[dir].clip);
		exit_data[dir].Update(GetTexture(exit_data[dir].img).W());
	}
}
//------------------------------------------------------------------------
// Purpose: free resources
//------------------------------------------------------------------------
void AssetManager::Quit()
{
	for (int i = 0; i < MAP_TOTAL; i++)
		for (SurfaceMap::iterator it = map[i].begin(); it != map[i].end(); ++it)
			it->second.Delete();

	invalid_img.Delete();
}