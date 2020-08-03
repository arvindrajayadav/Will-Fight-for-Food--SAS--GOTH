#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <string>

struct SaveFile
{
	bool auto_slot;
	std::string auto_1, auto_2, quick;

	SaveFile()
	{
		auto_1 = "autosave_1";
		auto_2 = "autosave_2";
		quick = "quick";
		auto_slot = false;
	}

	void Load(rapidxml::xml_node<char> *node)
	{
		auto_1 = node->first_attribute("auto_1")->value();
		auto_2 = node->first_attribute("auto_2")->value();
		quick = node->first_attribute("quick")->value();
	}
};

#endif //SAVEFILE_H