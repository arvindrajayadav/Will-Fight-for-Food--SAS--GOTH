#include "OptionMenu.h"
#include "rapidxml/rapidxml_print.hpp"
#include <fstream>

OptionMenu option_menu;

void OptionMenu :: Load(rapidxml::xml_node<char> *node)
{
	if(!LoadDone && NodeValid(node))
	{
		LoadStr(bg,"bg",node);
		keybind.Load(node->first_node("keybind"));
		save.Load(node->first_node("save"));
		restart.Load(node->first_node("restart"));
		resolution.Load(node->first_node("resolution"));

		fs.Load(node->first_node("fullscreen"));
		img_windowed = fs.Image();
		img_fullscreen.normal = img_windowed.select;
		img_fullscreen.hover = img_windowed.hover;
		img_fullscreen.select = img_windowed.normal;

		rapidxml::xml_node<char> *musnode = node->first_node("volume");
		if(NodeValid(musnode))
		{
			vol_music.Load(musnode->first_node("music"),gMusicManager.VolMusic());
			vol_effects.Load(musnode->first_node("effects"),gMusicManager.VolEffects());
		}

		fullscreen = gScreenSettings.FullScreen();
		LoadDone = true;
	}
}

void OptionMenu :: Draw(Button &back)
{
	if(fullscreen)
		fs.Image(img_fullscreen);
	else
		fs.Image(img_windowed);

	gAssetManager.DrawTexture(0,0,bg);
	vol_music.Draw();
	vol_effects.Draw();
	fs.Draw();
	keybind.Draw();
	resolution.Draw();
	save.Draw();
	back.Draw();

	if( gScreenSettings.CurW() != resolution.CurW() || gScreenSettings.CurH() != resolution.CurH() || fullscreen != gScreenSettings.FullScreen())
		restart.Draw();
}

bool OptionMenu :: HandleEvents(Button &back, SDL_Event Event)
{
	if(fs.HandleEvents(Event))
		fullscreen = !fullscreen;

	keybind.HandleEvents(Event);

	if(vol_music.HandleEvents(Event))
		gMusicManager.VolMusic(vol_music.Value());

	if(vol_effects.HandleEvents(Event))
		gMusicManager.VolEffects(vol_effects.Value());

	resolution.HandleEvents(Event);

	if((Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_ESCAPE) || back.HandleEvents(Event) == BUTTON_LCLICK)
	{
		Reset();
		return true;
	}

	if(save.HandleEvents(Event) == BUTTON_LCLICK)
	{
		WriteInputVal(gFilePath.controls);
		SaveState();
	}

	return false;
}

void OptionMenu :: SaveState()
{
	rapidxml::xml_document<char> doc;

	// xml declaration
	rapidxml::xml_node<char> *decl = doc.allocate_node(rapidxml::node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

	// root node
	rapidxml::xml_node<char> *root = doc.allocate_node(rapidxml::node_element, "settings");
	doc.append_node(root);

	rapidxml::xml_node<char> *fsnode = doc.allocate_node(rapidxml::node_element, "fullscreen");
	if(fullscreen)
		fsnode->value("true");
	else
		fsnode->value("false");
	root->append_node(fsnode);

	rapidxml::xml_node<char> *scrnode = doc.allocate_node(rapidxml::node_element, "screen");
	std::string w_str = NumberToString<int>(gScreenSettings.Width()), h_str = NumberToString<int>(gScreenSettings.Height());
	std::string bpp_str = NumberToString<int>(gScreenSettings.BitsPerPixel()), fps_str = NumberToString<int>(gScreenSettings.FramesPerSecond());
	scrnode->append_attribute(doc.allocate_attribute("w", w_str.c_str()));
	scrnode->append_attribute(doc.allocate_attribute("h", h_str.c_str()));
	scrnode->append_attribute(doc.allocate_attribute("bpp", bpp_str.c_str()));
	scrnode->append_attribute(doc.allocate_attribute("fps", fps_str.c_str()));
	root->append_node(scrnode);

	rapidxml::xml_node<char> *resnode = doc.allocate_node(rapidxml::node_element, "resolution");
	std::string rw_str = NumberToString<int>(resolution.CurW()), rh_str = NumberToString<int>(resolution.CurH());
	resnode->append_attribute(doc.allocate_attribute("w", rw_str.c_str()));
	resnode->append_attribute(doc.allocate_attribute("h", rh_str.c_str()));
	root->append_node(resnode);

	rapidxml::xml_node<char> *volnode = doc.allocate_node(rapidxml::node_element, "volume");
	std::string mus_str = NumberToString<int>(vol_music.Value()), cha_str = NumberToString<int>(vol_effects.Value());
	volnode->append_attribute(doc.allocate_attribute("music", mus_str.c_str()));
	volnode->append_attribute(doc.allocate_attribute("effects", cha_str.c_str()));
	root->append_node(volnode);

	rapidxml::xml_node<char> *pathnode = doc.allocate_node(rapidxml::node_element, "paths");
	gFilePath.SaveState(doc,pathnode);
	root->append_node(pathnode);

	std::string xml_as_string;
	rapidxml::print(std::back_inserter(xml_as_string), doc);

	std::ofstream save("res/settings.xml", std::ios::out);
	if(save.is_open())
	{
		save << xml_as_string;
		save.close();
	}

	doc.clear();
}