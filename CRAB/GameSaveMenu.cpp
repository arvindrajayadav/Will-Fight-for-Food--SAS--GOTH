#include "GameSaveMenu.h"
#include <iomanip>

GameSaveMenu gSaveMenu;

bool PathCompare(const boost::filesystem::path &p1, const boost::filesystem::path &p2)
{
	return last_write_time(p1) > last_write_time(p2);
}

SaveData::SaveData(boost::filesystem::path filepath)
{
	name = boost::filesystem::basename(filepath);

#if defined(__WIN32__) || defined(__APPLE__)
	std::time_t temp = boost::filesystem::last_write_time(filepath);
	last_modified = NumberToString(std::put_time(std::localtime(&temp), "%d %b %Y %H:%M"));
#else
	//Workaround for GNU C++ not having implemented std::put_time
	std::time_t temp = boost::filesystem::last_write_time(filepath);
	char timestr[100];

	std::strftime(timestr, sizeof(timestr), "%d %b %Y %H:%M", std::localtime(&temp));
	last_modified = timestr;
#endif

	path = filepath.string();
	XMLDoc conf(filepath.string());
	if (conf.ready())
	{
		rapidxml::xml_node<char> * node = conf.Doc()->first_node("save");
		if (NodeValid(node))
		{
			LoadStr(diff, "diff", node);
			LoadStr(loc, "loc", node);
		}
	}
}

void GameSaveMenu::Load(rapidxml::xml_node<char> *node)
{
	if (NodeValid(node))
	{
		prev.Load(node->first_node("prev"));
		next.Load(node->first_node("next"));

		img_slot.Load(node->first_node("slot"));
		LoadRect(slot_pos, node->first_node("slot"));

		slot_name.Load(node->first_node("slot")->first_node("name"));
		slot_loc.Load(node->first_node("slot")->first_node("loc"));
		slot_diff.Load(node->first_node("slot")->first_node("diff"));
		slot_time.Load(node->first_node("slot")->first_node("time"));

		ta_name.Load(node->first_node("name"));
	}

	ScanDir();
}

void GameSaveMenu::ScanDir()
{
	using namespace boost::filesystem;

	slot_info.clear();

	for (std::vector<Menu>::iterator m = menu.begin(); m != menu.end(); ++m)
		m->Clear();

	menu.resize(1);
	menu.at(0).UseKeyboard(true);

	path savedir(gFilePath.save_dir);
	if (exists(savedir) && is_directory(savedir))
	{
		directory_iterator dir_it(savedir);
		std::vector<path> file_in_dir;

		std::copy(directory_iterator(savedir), directory_iterator(), std::back_inserter(file_in_dir));
		std::sort(file_in_dir.begin(), file_in_dir.end(), PathCompare);

		int count_slot = 0, count_menu = 0;
		for (std::vector<path>::iterator i = file_in_dir.begin(); i != file_in_dir.end(); ++i)
			if (is_regular_file(*i) && i->extension().string() == gFilePath.save_ext)
			{
				if (count_slot >= slot_per_menu)
				{
					++count_menu;
					count_slot = 0;
					menu.resize(count_menu + 1);
					menu.at(count_menu).UseKeyboard(true);
				}

				slot_info.push_back(SaveData(*i));

				Button b;
				b.Image(img_slot);
				b.Init(slot_pos.x + slot_pos.w*(count_slot % 4), slot_pos.y + slot_pos.h*(count_slot / 4));
				menu[count_menu].button.push_back(b);
				++count_slot;
			}
	}

	for (std::vector<Menu>::iterator m = menu.begin(); m != menu.end(); ++m)
		m->AssignPaths();
}

bool GameSaveMenu::HandleEvents(SDL_Event Event)
{
	if (curmenu > 0)
		if (prev.HandleEvents(Event) == BUTTON_LCLICK || (Event.type == SDL_KEYUP && gInputVal[I_PUNCH].Equals(Event.key.keysym.sym)))
		{
			curmenu--;
			if (curmenu < 0)
				curmenu = 0;
		}

	if (curmenu < menu.size() - 1)
		if (next.HandleEvents(Event) == BUTTON_LCLICK || (Event.type == SDL_KEYUP && gInputVal[I_KICK].Equals(Event.key.keysym.sym)))
		{
			curmenu++;
			if (curmenu >= menu.size())
				curmenu = menu.size() - 1;
		}

	int choice = menu.at(curmenu).HandleEvents(Event);
	if (choice != -1)
	{
		Reset();
		sel_save = slot_info[curmenu*slot_per_menu + choice].path;
		return true;
	}

	return false;
}

void GameSaveMenu::Draw()
{
	menu.at(curmenu).Draw();
	for (Uint16 i = curmenu * slot_per_menu, count = 0; i < (curmenu + 1) * slot_per_menu && i < slot_info.size(); i++, count++)
	{
		Uint16 base_x = slot_pos.x + slot_pos.w*(count % 4), base_y = slot_pos.y + slot_pos.h*(count / 4);
		bool select = (HoverIndex() == i);

		slot_name.Draw(base_x, base_y, slot_info[i].name, select);
		slot_loc.Draw(base_x, base_y, slot_info[i].loc, select);
		slot_time.Draw(base_x, base_y, slot_info[i].last_modified, select);

		if (slot_info[i].diff == "0")      slot_diff.Draw(base_x, base_y, "Easy", select);
		else if (slot_info[i].diff == "1") slot_diff.Draw(base_x, base_y, "Normal", select);
		else if (slot_info[i].diff == "2") slot_diff.Draw(base_x, base_y, "Hard", select);
		else                               slot_diff.Draw(base_x, base_y, "Insane", select);
	}

	if (curmenu > 0)
		prev.Draw();

	if (curmenu < menu.size() - 1)
		next.Draw();
}