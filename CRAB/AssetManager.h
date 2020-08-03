//=============================================================================
// Author:   Arvind
// Purpose:  Contains the asset manager class - used to manage in-game assets
//=============================================================================
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "graphics.h"
#include "ImageData.h"
#include "inputval.h"
#include "loaders.h"
#include "GameParam.h"
#include "LoadingScreen.h"
#include "cursor.h"

//We store images here
typedef std::unordered_map<std::string, ImageData> SurfaceMap;

enum Direction { DIR_NONE = -1, DIR_DOWN, DIR_UP, DIR_LEFT, DIR_RIGHT, DIR_TOTAL };

enum MapID { MAP_CURRENT, MAP_COMMON };

const int MAP_TOTAL = 2;

class AssetManager
{
	//Assets are stored in images
	//Common is stuff used everywhere - this is only loaded once
	SurfaceMap map[MAP_TOTAL];

	//The default image for all invalid image names
	ImageData invalid_img, mouse_img;

	//The images used for level exits
	struct ExitData
	{
		std::string img;
		SDL_Rect clip;
		int frames_to_skip, frame_count;

		ExitData() { clip.x = 0; clip.y = 0; clip.h = 44; frames_to_skip = 3; frame_count = 0; }

		void Update(float w)
		{
			if (frame_count >= frames_to_skip)
			{
				frame_count = 0;
				clip.x += clip.w;
				if (clip.x >= w)
					clip.x = 0;
			}
			else
				++frame_count;
		}
	} exit_data[DIR_TOTAL];

public:
	AssetManager(){}
	~AssetManager(){}

	void Quit();

	bool Init();
	void LoadPaths();

	//Load all images specified in an XML file in a map
	void LoadMap(std::string filename, bool load = true, MapID id = MAP_CURRENT);

	//Image related stuff
	void AddTexture(std::string id, SDL_Surface* surface, int mapindex = MAP_COMMON);
	void AddTexture(std::string id, std::string path, int mapindex = MAP_COMMON);

	void FreeTexture(std::string &id, int mapindex = MAP_COMMON) { map[mapindex][id].Delete(); }

	void GetTexture(std::string &id, ImageData &data);
	ImageData& GetTexture(std::string &id);

	void DrawTexture(Sint16 x, Sint16 y, std::string &id, SDL_Rect* clip = NULL);
	void DrawTextureCentered(Sint16 x, Sint16 y, std::string &id, SDL_Rect* clip = NULL);

	//Draw the mouse
	void DrawMouse(Sint16 x, Sint16 y){ mouse_img.Draw(x, y); }
	void DrawExit(Sint16 x, Sint16 y, Direction dir);
};

extern AssetManager gAssetManager;
extern bool unread_obj, unread_inv;

#endif //ASSET_MANAGER_H