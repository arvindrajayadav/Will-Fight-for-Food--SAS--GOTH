#include "game.h"
#include "rapidxml/rapidxml_print.hpp"
#include <fstream>

//------------------------------------------------------------------------
// Purpose: Loading stuff
//------------------------------------------------------------------------
void Game::StartNewGame()
{
	Init(gFilePath.mod_cur);
	LoadLevel(curloc);

	//We add this here because load game needs to add it after loading save
	LoadPlayerTexture();

	clock.Start();
}
void Game::LoadGame(std::string filename)
{
	Init(gFilePath.mod_cur);
	LoadState(filename);
}

void Game::Init(std::string filename)
{
	gLoadScreen.Draw();
	curloc = "";
	lastobj = "";
	load_bg = "load_bg";
	TalkKeyDown = false;
	mode = MODE_RPG;
	state = STATE_GAME;
	CurTrigRect = -1;
	savefile.auto_slot = false;
	gem.Init();
	objective.Init();

	gLoadScreen.Draw();
	XMLDoc conf(filename);
	if (conf.ready())
	{
		rapidxml::xml_node<char> *node = conf.Doc()->first_node("config");

		LoadLevelMap(node->first_node("level")->first_attribute("list")->value());
		gLoadScreen.Draw();

		hud.Load(node->first_node("hud")->first_attribute("layout")->value());
		gLoadScreen.Draw();

		level.LoadMoves(node->first_node("fight")->first_attribute("moves")->value());
		level.LoadMovement(node->first_node("movement")->first_attribute("list")->value());
		gLoadScreen.Draw();

		map.Load(node->first_node("map")->first_attribute("layout")->value());
		curloc = node->first_node("level")->first_attribute("start")->value();
		gLoadScreen.Draw();

		rapidxml::xml_node<char> *eventnode = node->first_node("event");
		gem.Load(eventnode->first_attribute("list")->value(), eventnode->first_attribute("layout")->value(),
			eventnode->first_attribute("people")->value(), eventnode->first_attribute("conversation")->value(),
			eventnode->first_attribute("sentence")->value());
		gLoadScreen.Draw();

		objective.Load(node->first_node("objective")->first_attribute("layout")->value());
		inv.Load(node->first_node("inventory")->first_attribute("layout")->value());
		gLoadScreen.Draw();

		itemfile = node->first_node("item")->first_attribute("list")->value();
		gLoadScreen.Draw();

		savefile.Load(node->first_node("save"));
	}
}

void Game::LoadLevel(std::string name, Sint16 player_x, Sint16 player_y)
{
	if (levelmap.count(name) > 0)
	{
		gAssetManager.LoadMap(levelmap[name].asset);
		level.Load(levelmap[name].layout, mode, gem.var, player_x, player_y);
		curloc = name;
	}
}

void Game::LoadLevelMap(std::string filename)
{
	levelmap.clear();
	XMLDoc lev_list(filename);
	if (lev_list.ready())
	{
		rapidxml::xml_node<char> * node = lev_list.Doc()->first_node("world");
		rapidxml::xml_attribute<char> *name, *layout, *res;

		for (rapidxml::xml_node<char> * n = node->first_node("loc"); n != NULL; n = n->next_sibling("loc"))
		{
			name = n->first_attribute("name");
			layout = n->first_attribute("layout");
			res = n->first_attribute("res");

			levelmap[name->value()].layout = layout->value();
			levelmap[name->value()].asset = res->value();
		}
	}
}

Item Game::LoadItem(std::string id)
{
	Item i;
	XMLDoc item_list(itemfile);
	if (item_list.ready())
	{
		rapidxml::xml_node<char> *node = item_list.Doc()->first_node("items");
		for (rapidxml::xml_node<char> *n = node->first_node("item"); n != NULL; n = n->next_sibling("item"))
			if (id == n->first_attribute("id")->value())
			{
				i.Load(n);
				break;
			}
	}

	return i;
}

//------------------------------------------------------------------------
// Purpose: Handle events
//------------------------------------------------------------------------
void Game::HandleEvents(SDL_Event& Event, bool& ShouldChangeState, GameStateID& NewStateID)
{
	gMouse.HandleEvents(Event);

	switch (state)
	{
	case STATE_GAME:
		if (gem.EventInProgress())
		{
			gem.HandleEvents(objective, Event, event_res);
			if (ApplyResult())
				Quit(ShouldChangeState, NewStateID);
		}
		else
		{
			if (gInputVal[I_TALK].Equals(Event.key.keysym.sym) && mode == MODE_RPG)
				if (Event.type == SDL_KEYDOWN)
					TalkKeyDown = true;
				else if (Event.type == SDL_KEYUP)
					TalkKeyDown = false;

			level.HandleEvents(gem.people, mode, Event);

			switch (hud.HandleEvents(level.ShowMap(), Event))
			{
			case HS_OBJ: state = STATE_OBJECTIVE; break;
			case HS_MAP: state = STATE_MAP;	break;
			case HS_PAUSE: state = STATE_OPTION; break;
			case HS_INV: state = STATE_INVENTORY; break;
			default: break;
			}
		}
		break;
	case STATE_MAP:
		if (map.HandleEvents(Event))
			state = STATE_GAME;
		break;
	case STATE_OPTION:
		switch (hud.pause.HandleEvents(Event))
		{
		case PS_RESUME: state = STATE_GAME;	break;
		case PS_SAVE: SaveState(hud.pause.SaveFile()); state = STATE_GAME; break;
		case PS_LOAD:
			//Load new save
			ShouldChangeState = true;
			NewStateID = GAMESTATE_LOAD_GAME;
			return;
		case PS_QUIT: Quit(ShouldChangeState, NewStateID); break;
		default:break;
		}
		break;
	case STATE_OBJECTIVE:
		if ((Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_ESCAPE) || objective.HandleEvents(Event))
			state = STATE_GAME;
		break;
	case STATE_INVENTORY:
		if ((Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_ESCAPE) || inv.HandleEvents(gem.people[level.PlayerName()], Event))
			state = STATE_GAME;
		break;
	case STATE_LOSE_MENU:
		switch (hud.gom.HandleEvents(Event))
		{
		case 0: state = STATE_LOSE_LOAD; gSaveMenu.ScanDir(); break;
		case 1: Quit(ShouldChangeState, NewStateID); break;
		default:break;
		}
		break;
	case STATE_LOSE_LOAD:
		if (gSaveMenu.HandleEvents(Event))
		{
			ShouldChangeState = true;
			NewStateID = GAMESTATE_LOAD_GAME;
			return;
		}

		if ((Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_ESCAPE) || hud.back.HandleEvents(Event) == BUTTON_LCLICK)
			state = STATE_LOSE_MENU;
		break;
	default:break;
	}
}
//------------------------------------------------------------------------
// Purpose: InternalEvents
//------------------------------------------------------------------------
void Game::InternalEvents(bool& ShouldChangeState, GameStateID& NewStateID)
{
	switch (state)
	{
	case STATE_GAME:
		ApplyResult(level.InternalEvents(gem.people, mode, gem.var, CurTrigRect));

		event_res.clear();
		if (TalkKeyDown)
		{
			gem.InternalEvents(lastobj, curloc, objective, inv, event_res, CurTrigRect, level);
			TalkKeyDown = false;
		}
		else
			gem.InternalEvents("", curloc, objective, inv, event_res, CurTrigRect, level);

		if (ApplyResult())
			Quit(ShouldChangeState, NewStateID);

		if (gem.EventInProgress())
			level.PlayerStop();

		break;
	case STATE_MAP:
		if (map.InternalEvents())
		{
			//If we need to change the level, load the new level
			LoadLevel(map.CurLocation());
			state = STATE_GAME;
		}
		break;
	default:break;
	}

	lastobj = "";
}
//------------------------------------------------------------------------
// Purpose: Draw
//------------------------------------------------------------------------
void Game::Draw()
{
	if (state == STATE_MAP)
		map.Draw();
	else
	{
		level.Draw(gem.people);
		switch (state)
		{
		case STATE_GAME:
			if (gem.EventInProgress())
				gem.Draw();
			else
				hud.Draw(level.PlayerInCombat(), gem.Health(level.PlayerName()), level.ShowMap());
			break;
		case STATE_OPTION:
			hud.pause.Draw();
			break;
		case STATE_OBJECTIVE:
			objective.Draw();
			break;
		case STATE_INVENTORY:
			inv.Draw(gem.people[level.PlayerName()]);
			break;
		case STATE_LOSE_MENU:
			hud.gom.Draw();
			break;
		case STATE_LOSE_LOAD:
			gAssetManager.DrawTexture(0, 0, load_bg);
			gSaveMenu.Draw();
			hud.back.Draw();
			break;
		default:break;
		}
	}

	gAssetManager.DrawMouse(gMouse.MotionX(), gMouse.MotionY());
}

//------------------------------------------------------------------------
// Purpose: Apply results of events and levels
//------------------------------------------------------------------------
bool Game::ApplyResult()
{
	bool need_to_save = false;
	for (std::vector<EventResult>::iterator eve_it = event_res.begin(); eve_it != event_res.end(); ++eve_it)
		switch (eve_it->type)
	{
		case ER_MOVE:
			for (std::vector<Sprite>::iterator i = level.objects.begin(); i != level.objects.end(); ++i)
				if (eve_it->val == i->Name())
				{
					i->WalkPattern(level.move_set[eve_it->x]);
					break;
				}
			break;
		case ER_LEVEL:
			if (eve_it->val == "Map")
				state = STATE_MAP;
			else
				LoadLevel(eve_it->val, eve_it->x, eve_it->y);
			break;
		case ER_HEAL:
			gem.Heal(eve_it->val, eve_it->x);
			break;
		case ER_ITEM:
			inv.AddItem(LoadItem(eve_it->val));
			break;
		case ER_SAVE:
			need_to_save = true;
			break;
		case ER_SYNC:
			level.CalcVis(gem.var);
			break;
		case ER_MODE:
			if (eve_it->val == "fight")
				mode = MODE_FIGHT;
			else
				mode = MODE_RPG;
			break;
		case ER_QUIT: return true;
		default: break;
	}

	if (need_to_save)
	{
		if (savefile.auto_slot)
			SaveState(savefile.auto_2);
		else
			SaveState(savefile.auto_1);

		savefile.auto_slot = !savefile.auto_slot;
	}

	event_res.clear();
	return false;
}

void Game::ApplyResult(LevelResult result)
{
	switch (result.type)
	{
	case LR_OBJECT:
		lastobj = result.val;
		break;
	case LR_LEVEL:
		if (result.val == "Map")
			state = STATE_MAP;
		else
			LoadLevel(result.val, result.x, result.y);
		return;
	case LR_GAMEOVER:
		state = STATE_LOSE_MENU;
		hud.gom.Reset();
		break;
	default:break;
	}
}

//------------------------------------------------------------------------
// Purpose: Save/load game
//------------------------------------------------------------------------
void Game::LoadState(std::string filename)
{
	XMLDoc conf(filename);
	if (conf.ready())
	{
		rapidxml::xml_node<char> *node = conf.Doc()->first_node("save");

		int diff = 0;
		LoadNum(diff, "diff", node);
		difficulty = (GameDifficulty)diff;

		if (NodeValid(node->first_node("player_img_path")))
			cur_img_path.Load(node->first_node("player_img_path"));

		LoadPlayerTexture();

		gem.LoadState(node);
		objective.LoadState(node);
		inv.LoadState(node);

		LoadStr(curloc, "loc", node);
		LoadLevel(curloc);

		std::string playtime;
		LoadStr(playtime, "time", node);
		clock.Start(playtime);
	}
}

void Game::SaveState(std::string filename)
{
	rapidxml::xml_document<char> doc;

	// xml declaration
	rapidxml::xml_node<char> *decl = doc.allocate_node(rapidxml::node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

	// root node
	rapidxml::xml_node<char> *root = doc.allocate_node(rapidxml::node_element, "save");
	doc.append_node(root);

	//Save location
	root->append_attribute(doc.allocate_attribute("loc", curloc.c_str()));

	//Difficulty
	std::string diff = NumberToString<int>(difficulty);
	root->append_attribute(doc.allocate_attribute("diff", diff.c_str()));

	//Time played
	std::string playtime = clock.GetTime();
	root->append_attribute(doc.allocate_attribute("time", playtime.c_str()));

	//Player image paths
	cur_img_path.SaveState(doc, root);

	gem.SaveState(doc, root);
	objective.SaveState(doc, root);
	inv.SaveState(doc, root);

	std::string savefile = gFilePath.save_dir + filename + gFilePath.save_ext;

	std::string xml_as_string;
	rapidxml::print(std::back_inserter(xml_as_string), doc);

	std::ofstream save(savefile.c_str(), std::ios::out);
	if (save.is_open())
	{
		save << xml_as_string;
		save.close();
	}

	doc.clear();
}