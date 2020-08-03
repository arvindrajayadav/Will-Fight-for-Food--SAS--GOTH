#ifndef GAMEPARAM_H
#define GAMEPARAM_H

#include <string>
#include "XMLDoc.h"
#include "loaders.h"

enum GameMode { MODE_RPG, MODE_FIGHT };

enum GameDifficulty
{
	DIFF_EASY, //Health refills after every battle
	DIFF_NORMAL, //Health refills 50% at certain events (recommended)
	DIFF_HARD, //Health refills 25% at certain events
	DIFF_INSANE //Health never refills
};

const int MODE_TOTAL = 2;

struct FilePaths
{
	//Resources common to all levels and states
	std::string common;

	//Mod file location, current mod and their extension
	std::string mod_path, mod_ext, mod_cur;

	//Main menu resources
	std::string mainmenu_l, mainmenu_r;

	//Sounds
	std::string sound_effect, sound_music;

	//Fonts used, window icon and control file
	std::string font, icon, controls;

	//Save directory and extension
	std::string save_dir, save_ext;

	//The backgrounds used in common menus
	std::string main_bg, load_bg, diff_bg, cred_bg, skin_bg;

	FilePaths();
	void Load(rapidxml::xml_node<char> *node);
	void SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root);
};

//The file paths of Jared images
struct ImagePaths
{
	std::string normal, digi, young, face, face_young;

	ImagePaths();
	ImagePaths(rapidxml::xml_node<char> *node) { Load(node); }

	void Load(rapidxml::xml_node<char> *node)
	{
		LoadStr(normal, "normal", node);
		LoadStr(digi, "digital", node);
		LoadStr(young, "young", node);
		LoadStr(face, "face", node);
		LoadStr(face_young, "face_y", node);
	}

	void SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root);
};

extern FilePaths gFilePath;
extern GameDifficulty difficulty;
extern ImagePaths cur_img_path;

#endif //FILEPATH_H