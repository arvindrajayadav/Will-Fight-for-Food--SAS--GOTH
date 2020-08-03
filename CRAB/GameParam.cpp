#include "GameParam.h"

//The global file path object
FilePaths gFilePath;

//The difficulty level of the game
GameDifficulty difficulty = DIFF_EASY;

//The paths of various player sprites and faces
ImagePaths cur_img_path;

ImagePaths::ImagePaths()
{
	normal = "res/gfx/common/player/p1_n.png";
	digi = "res/gfx/common/player/p1_d.png";
	young = "res/gfx/common/player/p1_y.png";
	face = "res/gfx/common/player/faces/p1_f.png";
	face_young = "res/gfx/common/player/faces/p1_yf.png";
}

void ImagePaths::SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root)
{
	rapidxml::xml_node<char> *imgnode = doc.allocate_node(rapidxml::node_element, "player_img_path");
	imgnode->append_attribute(doc.allocate_attribute("normal", normal.c_str()));
	imgnode->append_attribute(doc.allocate_attribute("digital", digi.c_str()));
	imgnode->append_attribute(doc.allocate_attribute("young", young.c_str()));
	imgnode->append_attribute(doc.allocate_attribute("face", face.c_str()));
	imgnode->append_attribute(doc.allocate_attribute("face_y", face_young.c_str()));
	root->append_node(imgnode);
}

FilePaths::FilePaths()
{
	common = "res/levels/common.xml";
	mod_path = "mods/";
	mod_ext = ".wffmod";
	mainmenu_l = "res/layout/main_menu_l.xml";
	mainmenu_r = "res/layout/main_menu_r.xml";
	sound_effect = "res/sounds/effects.xml";
	sound_music = "res/sounds/music.xml";
	font = "res/fonts/fonts.xml";
	mod_cur = "res/default.xml";
	icon = "res/gfx/icon.bmp";
	controls = "res/controls.xml";
	save_dir = "save/";
	save_ext = ".wff";
	main_bg = "main_bg";
	load_bg = "load_bg";
	diff_bg = "diff_bg";
	cred_bg = "cred_bg";
	skin_bg = "skin_bg";
}

void FilePaths::Load(rapidxml::xml_node<char> *node)
{
	if (NodeValid(node))
	{
		rapidxml::xml_node<char> *iconode = node->first_node("icon");
		if (NodeValid(iconode))
			icon = iconode->value();

		rapidxml::xml_node<char> *commonnode = node->first_node("common");
		if (NodeValid(commonnode))
			common = commonnode->value();

		rapidxml::xml_node<char> *fontnode = node->first_node("font");
		if (NodeValid(fontnode))
			font = fontnode->value();

		rapidxml::xml_node<char> *contnode = node->first_node("controls");
		if (NodeValid(contnode))
			controls = contnode->value();

		rapidxml::xml_node<char> *modnode = node->first_node("mod");
		if (NodeValid(modnode))
		{
			LoadStr(mod_path, "path", modnode);
			LoadStr(mod_ext, "ext", modnode);
			LoadStr(mod_cur, "cur", modnode);
		}

		rapidxml::xml_node<char> *menunode = node->first_node("main_menu");
		if (NodeValid(menunode))
		{
			LoadStr(mainmenu_l, "l", menunode);
			LoadStr(mainmenu_r, "r", menunode);
		}

		rapidxml::xml_node<char> *soundnode = node->first_node("sound");
		if (NodeValid(soundnode))
		{
			LoadStr(sound_effect, "effect", soundnode);
			LoadStr(sound_music, "music", soundnode);
		}

		rapidxml::xml_node<char> *savenode = node->first_node("save");
		if (NodeValid(savenode))
		{
			LoadStr(save_dir, "dir", savenode);
			LoadStr(save_ext, "ext", savenode);
		}

		rapidxml::xml_node<char> *bgnode = node->first_node("backgrounds");
		if (NodeValid(bgnode))
		{
			LoadStr(main_bg, "main", bgnode);
			LoadStr(load_bg, "load", bgnode);
			LoadStr(diff_bg, "diff", bgnode);
			LoadStr(cred_bg, "cred", bgnode);
			LoadStr(skin_bg, "skin", bgnode);
		}
	}
}

void FilePaths::SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root)
{
	rapidxml::xml_node<char> *iconnode = doc.allocate_node(rapidxml::node_element, "icon");
	iconnode->value(icon.c_str());
	root->append_node(iconnode);

	rapidxml::xml_node<char> *commonnode = doc.allocate_node(rapidxml::node_element, "common");
	commonnode->value(common.c_str());
	root->append_node(commonnode);

	rapidxml::xml_node<char> *controlsnode = doc.allocate_node(rapidxml::node_element, "controls");
	controlsnode->value(controls.c_str());
	root->append_node(controlsnode);

	rapidxml::xml_node<char> *fontnode = doc.allocate_node(rapidxml::node_element, "font");
	fontnode->value(font.c_str());
	root->append_node(fontnode);

	rapidxml::xml_node<char> *modnode = doc.allocate_node(rapidxml::node_element, "mod");
	modnode->append_attribute(doc.allocate_attribute("path", mod_path.c_str()));
	modnode->append_attribute(doc.allocate_attribute("ext", mod_ext.c_str()));
	modnode->append_attribute(doc.allocate_attribute("cur", mod_cur.c_str()));
	root->append_node(modnode);

	rapidxml::xml_node<char> *main_menunode = doc.allocate_node(rapidxml::node_element, "main_menu");
	main_menunode->append_attribute(doc.allocate_attribute("l", mainmenu_l.c_str()));
	main_menunode->append_attribute(doc.allocate_attribute("r", mainmenu_r.c_str()));
	root->append_node(main_menunode);

	rapidxml::xml_node<char> *soundnode = doc.allocate_node(rapidxml::node_element, "sound");
	soundnode->append_attribute(doc.allocate_attribute("effect", sound_effect.c_str()));
	soundnode->append_attribute(doc.allocate_attribute("music", sound_music.c_str()));
	root->append_node(soundnode);

	rapidxml::xml_node<char> *savenode = doc.allocate_node(rapidxml::node_element, "save");
	savenode->append_attribute(doc.allocate_attribute("dir", save_dir.c_str()));
	savenode->append_attribute(doc.allocate_attribute("ext", save_ext.c_str()));
	root->append_node(savenode);

	rapidxml::xml_node<char> *bgnode = doc.allocate_node(rapidxml::node_element, "backgrounds");
	bgnode->append_attribute(doc.allocate_attribute("main", main_bg.c_str()));
	bgnode->append_attribute(doc.allocate_attribute("load", load_bg.c_str()));
	bgnode->append_attribute(doc.allocate_attribute("diff", diff_bg.c_str()));
	bgnode->append_attribute(doc.allocate_attribute("cred", cred_bg.c_str()));
	bgnode->append_attribute(doc.allocate_attribute("skin", skin_bg.c_str()));
	root->append_node(bgnode);
}