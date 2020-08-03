#include "ModMenu.h"

void ModMenu :: Load(rapidxml::xml_node<char> *node)
{
	if(NodeValid(node))
	{
		LoadStr(bg, "bg", node);
		img.Load(node);
		LoadNum(font, "font", node);
		LoadColor(col, node);
		LoadRect(slot_pos, node);
		LoadXY(off_name.x, off_name.y, node->first_node("name"));

		prev.Load(node->first_node("prev"));
		next.Load(node->first_node("next"));
	}

	ScanDir();
}

void ModMenu :: ScanDir()
{
	using namespace boost::filesystem;

	mod_list.clear();

	for(std::vector<Menu>::iterator m = menu.begin(); m != menu.end(); ++m)
		m->Clear();

	menu.resize(1);
	menu.at(0).UseKeyboard(true);

	path moddir(gFilePath.mod_path);
	if(exists(moddir) && is_directory(moddir))
	{
		directory_iterator dir_it(moddir);
		std::vector<path> file_in_dir;

		std::copy(directory_iterator(moddir), directory_iterator(), std::back_inserter(file_in_dir));
		std::sort(file_in_dir.begin(), file_in_dir.end());

		int count_slot = 0, count_menu = 0;
		for(std::vector<path>::iterator i = file_in_dir.begin(); i != file_in_dir.end(); ++i)
			if(is_regular_file(*i) && i->extension().string() == gFilePath.mod_ext)
			{
				mod_list.push_back(ModDetails(*i));

				Button b;
				b.Image(img);
				b.Init(slot_pos.x + slot_pos.w*(count_slot%4), slot_pos.y + slot_pos.h*(count_slot/4));
				menu[count_menu].button.push_back(b);

				if(++count_slot >= slot_per_menu)
				{
					++count_menu;
					count_slot = 0;
					menu.resize(count_menu+1);
					menu.at(count_menu).UseKeyboard(true);
				}
			}
	}

	for(std::vector<Menu>::iterator m = menu.begin(); m != menu.end(); ++m)
		m->AssignPaths();
}

void ModMenu :: Draw()
{
	gAssetManager.DrawTexture(0,0,bg);
	menu.at(curmenu).Draw();
	for(Uint16 i = curmenu * slot_per_menu, count = 0; i < (curmenu+1) * slot_per_menu && i < mod_list.size(); i++, count++)
		gTextManager.Draw(slot_pos.x+slot_pos.w*(count%4)+off_name.x, slot_pos.y+slot_pos.h*(count/4)+off_name.y,mod_list[i].name,col,font);

	if (curmenu > 0)
		prev.Draw();

	if (curmenu < menu.size() - 1)
		next.Draw();
}

bool ModMenu :: HandleEvents(SDL_Event Event)
{
	if(curmenu > 0)
		if(prev.HandleEvents(Event) == BUTTON_LCLICK || (Event.type == SDL_KEYUP && gInputVal[I_PUNCH].Equals(Event.key.keysym.sym)) )
		{
			curmenu--;
			if(curmenu < 0)
				curmenu = 0;
		}

	if(curmenu < menu.size() - 1)
		if(next.HandleEvents(Event) == BUTTON_LCLICK || (Event.type == SDL_KEYUP && gInputVal[I_KICK].Equals(Event.key.keysym.sym)) )
		{
			curmenu++;
			if(curmenu >= menu.size())
				curmenu = menu.size() - 1;
		}

	int choice = menu.at(curmenu).HandleEvents(Event);
	if(choice != -1)
	{
		gFilePath.mod_cur = mod_list[curmenu*slot_per_menu+choice].path;
		return true;
	}

	return false;
}