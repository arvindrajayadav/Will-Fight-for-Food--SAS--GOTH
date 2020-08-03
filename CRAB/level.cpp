#include "level.h"

#define ANALOG_AXIS_THRESHLOD 7000

void Level::Reset()
{
	frames_to_skip = gScreenSettings.FramesPerSecond() / 20;
	frames_skipped = 0;
	player_index = 0;
	obj_layer = 0;

	objects.clear();
	restricted_area.clear();
	layer.clear();
	level_exit.clear();
	obj_seq.clear();
	trig_rect.clear();

	Camera(0, 0, gScreenSettings.Width(), gScreenSettings.Height());
	w = camera.w;
	h = camera.h;
	walk = camera;

	showmap.Set(true);
	goa.Reset();
	inside_exit = false;
	inside_map = false;
	player_in_combat = false;
	allow_mode_switch = true;
}

//------------------------------------------------------------------------
// Purpose: Load the level
//------------------------------------------------------------------------
void Level::Load(std::string filename, const GameMode mode, VarMap &varmap, Sint16 player_x, Sint16 player_y)
{
	Reset();
	XMLDoc conf(filename);
	if (conf.ready())
	{
		rapidxml::xml_node<char> *node = conf.Doc()->first_node("level");
		if (NodeValid(node))
		{
			w = StringToNumber<Uint16>(node->first_attribute("w")->value());
			h = StringToNumber<Uint16>(node->first_attribute("h")->value());

			if (node->first_attribute("map") != NULL)
			{
				std::string str = node->first_attribute("map")->value();
				if (str == "false")
					showmap.Set(false);
			}
			else
				showmap.Set(true);

			LoadRect(walk, node->first_node("walk_area"));

			rapidxml::xml_node<char> *layernode = node->first_node("layers");
			if (NodeValid(layernode))
			{
				if (!LoadNum(obj_layer, "obj", layernode))
					obj_layer = 0;

				for (rapidxml::xml_node<char> *n = layernode->first_node("layer"); n != NULL; n = n->next_sibling("layer"))
				{
					LevelLayer l;
					l.Load(n);
					layer.push_back(l);
				}
			}

			rapidxml::xml_node<char> *spritenode = node->first_node("sprites");
			if (NodeValid(spritenode))
			{
				int count = 0;
				for (rapidxml::xml_node<char> *n = spritenode->first_node(); n != NULL; n = n->next_sibling(), ++count)
				{
					Sprite s;
					s.Load(n, fight_move, move_set, varmap);

					std::string str = n->name();
					if (str == "player")
					{
						player_index = objects.size();
						if (player_x != -1 && player_y != -1)
						{
							s.X(player_x);
							s.Y(player_y);
						}
					}

					objects.push_back(s);
					obj_seq.push_back(count);
				}
			}
			CalcVis(varmap);

			rapidxml::xml_node<char> *nowalknode = node->first_node("nowalk_areas");
			if (NodeValid(nowalknode))
				for (rapidxml::xml_node<char> *n = nowalknode->first_node("area"); n != NULL; n = n->next_sibling("area"))
				{
					SDL_Rect r;
					LoadRect(r, n);
					restricted_area.push_back(r);
				}

			rapidxml::xml_node<char> *trignode = node->first_node("trig_area");
			if (NodeValid(trignode))
				for (rapidxml::xml_node<char> *n = trignode->first_node("trigger"); n != NULL; n = n->next_sibling("trigger"))
				{
					SDL_Rect r;
					LoadRect(r, n);
					trig_rect.push_back(r);
				}

			rapidxml::xml_node<char> *exitnode = node->first_node("exits");
			if (NodeValid(exitnode))
				for (rapidxml::xml_node<char> *n = exitnode->first_node("exit"); n != NULL; n = n->next_sibling("exit"))
				{
					LevelExit l;
					l.Load(n);
					level_exit.push_back(l);
				}

			rapidxml::xml_node<char> *musicnode = node->first_node("music");
			if (NodeValid(musicnode))
			{
				LoadStr(music_peace, "peace", musicnode);
				LoadStr(music_fight, "fight", musicnode);
				if (gMusicManager.LoadMusic(music_peace, music_fight))
					gMusicManager.PlayMusic(mode);
			}

			rapidxml::xml_node<char> *modenode = node->first_node("mode");
			if (NodeValid(modenode, false))
			{
				std::string a = "true";
				LoadStr(a, "allow_switch", modenode);

				if (a == "false")
					allow_mode_switch = false;
				else
					allow_mode_switch = true;
			}
		}
	}
}

void Level::LoadMoves(std::string filename)
{
	XMLDoc mov_list(filename);
	if (mov_list.ready())
	{
		rapidxml::xml_node<char> *node = mov_list.Doc()->first_node("movelist");
		for (rapidxml::xml_node<char> *n = node->first_node("set"); n != NULL; n = n->next_sibling("set"))
		{
			Uint16 pos = fight_move.size();
			LoadNum(pos, "id", n);
			if (pos >= fight_move.size())
				fight_move.resize(pos + 1);
			fight_move[pos].Load(n->first_attribute("path")->value());
		}
	}
}

void Level::LoadMovement(std::string filename)
{
	XMLDoc mov_list(filename);
	if (mov_list.ready())
	{
		rapidxml::xml_node<char> *node = mov_list.Doc()->first_node("movement");
		for (rapidxml::xml_node<char> *n = node->first_node("set"); n != NULL; n = n->next_sibling("set"))
		{
			Uint16 pos = move_set.size();
			LoadNum(pos, "id", n);
			if (pos >= move_set.size())
				move_set.resize(pos + 1);
			move_set[pos].Load(n);
		}
	}
}

//------------------------------------------------------------------------
// Purpose: Checks if the player is colliding with an object,
// returns the object index if collides, -1 if no collision
//------------------------------------------------------------------------
bool Level::CollidingWithObject(ObjMap &people, std::string &name, GameMode mode)
{
	int index = 0;
	for (std::vector<Sprite>::iterator i = objects.begin(); i != objects.end(); ++i, ++index)
		if (i->Visible() && player_index != index && people[i->Name()].State() == OST_STAND && people[i->Name()].Type() != OT_HOSTILE)
			if (objects[player_index].FullCollide(i->BoundRect()))
			{
				name = i->Name();
				if (mode == MODE_RPG)
					talk_notify.Set(*i);
				return true;
			}

	talk_notify.active = false;
	return false;
}

bool Level::CollidingWithLevel(Sprite &s, ObjMap &people, std::string &name, GameMode mode)
{
	for (std::vector<SDL_Rect>::iterator i = restricted_area.begin(); i != restricted_area.end(); ++i)
		if (s.FullCollide(*i))
			return true;

	for (std::vector<Sprite>::iterator i = objects.begin(); i != objects.end(); ++i)
		if (people[i->Name()].Type() == OT_OBSTACLE && i->Visible() && s.FullCollide(i->BoundRect()) && s.Name() != i->Name())
		{
			name = i->Name();
			if (mode == MODE_RPG)
				talk_notify.Set(*i);
			return true;
		}

	if (!s.InsideRect(walk))
		return true;

	talk_notify.active = false;
	return false;
}

int Level::CollidingWithTrigger(Sprite &s)
{
	int index = 0;
	for (std::vector<SDL_Rect>::iterator i = trig_rect.begin(); i != trig_rect.end(); ++i, ++index)
		if (s.FullCollide(*i))
			return index;

	return -1;
}

bool Level::CollidingWithExit(LevelResult &res)
{
	for (std::vector<LevelExit>::iterator i = level_exit.begin(); i != level_exit.end(); ++i)
		if (objects[player_index].FullCollide(i->dim))
		{
			res.val = i->name;
			res.x = i->entry_x;
			res.y = i->entry_y;
			return true;
		}

	return false;
}

bool Level::PlayerInCombat(ObjMap &people)
{
	int index = 0;
	for (std::vector<Sprite>::iterator i = objects.begin(); i != objects.end(); ++i, ++index)
		if (index != player_index && people[i->Name()].Type() == OT_HOSTILE && people[i->Name()].State() != OST_KO && i->Visible())
			return true;

	return false;
}
//------------------------------------------------------------------------
// Purpose: Handle the level movement
//------------------------------------------------------------------------
void Level::HandleEvents(ObjMap &people, GameMode &mode, SDL_Event Event)
{
	if (objects[player_index].Combo() == "")
	{
		Uint8* keys = SDL_GetKeyState(NULL);

		if (gInputVal[I_UP].Pressed(keys))
			objects[player_index].YVel(-1 * people[objects[player_index].Name()].stat[STAT_SPEED].val[STVAL_CUR].num);
		else if (gInputVal[I_DOWN].Pressed(keys))
			objects[player_index].YVel(people[objects[player_index].Name()].stat[STAT_SPEED].val[STVAL_CUR].num);
		else
			objects[player_index].YVel(0);

		if (gInputVal[I_LEFT].Pressed(keys))
			objects[player_index].XVel(-1 * people[objects[player_index].Name()].stat[STAT_SPEED].val[STVAL_CUR].num);
		else if (gInputVal[I_RIGHT].Pressed(keys))
			objects[player_index].XVel(people[objects[player_index].Name()].stat[STAT_SPEED].val[STVAL_CUR].num);
		else
			objects[player_index].XVel(0);

		if (Event.type == SDL_KEYUP)
		{
			if (!allow_mode_switch)
			{
				mode = MODE_RPG;
			}
			else if (gInputVal[I_MODE].Equals(Event.key.keysym.sym))
			{
				if (mode == MODE_RPG)
				{
					objects[player_index].UpdateMove("mode_fight");
					mode = MODE_FIGHT;
				}
				else
				{
					objects[player_index].UpdateMove("mode_talk");
					mode = MODE_RPG;
				}

				objects[player_index].Stop();
				gMusicManager.PlayMusic(mode);
			}
		}

		if ((Event.type != SDL_MOUSEMOTION && Event.type != SDL_KEYDOWN && Event.type != SDL_MOUSEBUTTONDOWN && Event.type != SDL_MOUSEBUTTONUP) &&
			(objects[player_index].XVel() == 0 && objects[player_index].YVel() == 0))
			objects[player_index].Walk(mode, DIR_NONE);
	}
	else
		objects[player_index].Stop();

	//Because of gameplay reasons
	if (input.HandleEvents(Event))
	{
		if (mode != MODE_FIGHT)
		{
			mode = MODE_FIGHT;
			gMusicManager.PlayMusic(mode);
		}
		objects[player_index].Stop();
	}
}
//------------------------------------------------------------------------
// Purpose: InternalEvents
//------------------------------------------------------------------------
LevelResult Level::InternalEvents(ObjMap &people, GameMode mode, VarMap &var, int &CurTrigRect)
{
	LevelResult result;
	player_in_combat = PlayerInCombat(people);

	if (goa.started && goa.finished)
	{
		goa.Reset();
		result.type = LR_GAMEOVER;
		return result;
	}

	if (CollidingWithExit(result))
	{
		if (result.val == "Map")
		{
			if (PlayerInCombat())
			{
				inside_map = true;
			}
			else if (inside_map == false)
			{
				inside_map = true;
				objects[player_index].Stop();
				result.type = LR_LEVEL;
				return result;
			}
		}
		else
		{
			if (PlayerInCombat())
			{
				inside_exit = true;
			}
			else if (inside_exit == false)
			{
				result.type = LR_LEVEL;
				return result;
			}
		}
	}
	else
	{
		inside_exit = false;
		inside_map = false;

		if (PlayerInCombat())
			showmap.current = false;
		else
			showmap.current = showmap.normal;
	}

	Think(people, var);

	objects[player_index].Move();
	if (CollidingWithLevel(objects[player_index], people, result.val, mode))
		objects[player_index].UndoMove();

	CurTrigRect = CollidingWithTrigger(objects[player_index]);

	if (frames_skipped >= frames_to_skip)
	{
		Animate(mode, people);
		frames_skipped = 0;
	}
	else
		frames_skipped++;

	if (result.val != "")
	{
		result.type = LR_OBJECT;
		return result;
	}

	if (CollidingWithObject(people, result.val, mode))
		result.type = LR_OBJECT;

	return result;
}
//------------------------------------------------------------------------
// Purpose: Draw the level
//------------------------------------------------------------------------
void Level::Draw(ObjMap &people)
{
	SetCamera();
	SortObjectsToDraw();

	int layer_count = 0;
	for (std::vector<LevelLayer>::iterator it = layer.begin(); it != layer.end(); ++it, ++layer_count)
	{
		it->Draw(camera);
		if (layer_count == obj_layer)
		{
			DrawObjects(people);

			if (!PlayerInCombat())
			{
				for (std::vector<LevelExit>::iterator it = level_exit.begin(); it != level_exit.end(); ++it)
					it->Draw(camera);
			}
		}
	}

	talk_notify.Draw(camera);
}

void Level::DrawObjects(ObjMap &people)
{
	for (std::vector<Uint16>::iterator i = obj_seq.begin(); i != obj_seq.end(); ++i)
		if (objects[*i].Visible())
			objects[*i].Draw(-1 * camera.x, -1 * camera.y);
}
//------------------------------------------------------------------------
// Purpose: Center the camera on the player for scrolling levels
//------------------------------------------------------------------------
void Level::SetCamera()
{
	//Center the camera over the player
	camera.x = (objects[player_index].X() + objects[player_index].Width() / 2) - gScreenSettings.Width() / 2;
	camera.y = (objects[player_index].Y() + objects[player_index].Height() / 2) - gScreenSettings.Height() / 2;

	//Keep the camera in bounds.
	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.x > w - camera.w) camera.x = w - camera.w;
	if (camera.y > h - camera.h) camera.y = h - camera.h;
}

//------------------------------------------------------------------------
// Purpose: Sort objects to draw them according to their Y coordinates
//------------------------------------------------------------------------
void Level::SortObjectsToDraw()
{
	for (Uint16 i = 0; i < obj_seq.size(); i++)
		for (Uint16 j = i; j > 0 && objects[obj_seq[j - 1]].Y() + objects[obj_seq[j - 1]].Height() > objects[obj_seq[j]].Y() + objects[obj_seq[j]].Height(); j--)
		{
			int temp = obj_seq[j];
			obj_seq[j] = obj_seq[j - 1];
			obj_seq[j - 1] = temp;
		}
}

//------------------------------------------------------------------------
// Purpose: Animate player
//------------------------------------------------------------------------
void Level::AnimateSprite(Sprite &s, GameMode mode, ObjMap &people)
{
	if (s.Combo() == "")
		s.Walk(mode);
	else
	{
		s.UpdateFrame(mode);
		std::string str;
		if (CollidingWithLevel(s, people, str, mode))
			s.UndoUpdateFrame();
	}
}

void Level::Animate(GameMode mode, ObjMap &people)
{
	int index = 0;
	for (std::vector<Sprite>::iterator i = objects.begin(); i != objects.end(); ++i, ++index)
		if (index == player_index)
		{
			if (goa.started && i->LastFrame())
				goa.finished = true;

			if (people[i->Name()].stat[STAT_HEALTH].val[STVAL_CUR].num <= 0 && !goa.started)
			{
				i->ForceUpdateMove("ko_idle");
				goa.started = true;
				continue;
			}

			AnimateSprite(*i, mode, people);
		}
		else if (i->Visible())
		{
			if (people[i->Name()].State() == OST_KO)
			{
				if (!i->KoPlaying())
				{
					i->UpdateMove("ko_idle");
					i->KoPlaying(true);
				}
				else
					i->UpdateFrame(MODE_FIGHT, true);
			}
			else
			{
				switch (people[i->Name()].Type())
				{
				case OT_HOSTILE: AnimateSprite(*i, MODE_FIGHT, people); break;
				case OT_COWARD: AnimateSprite(*i, MODE_RPG, people); break;
				default:
					i->Walk(MODE_RPG);
					i->TraversePath(people[i->Name()].stat[STAT_SPEED].val[STVAL_CUR].num);
					break;
				}

				std::string player_name = objects[player_index].Name();

				if (objects[player_index].TakingDamage(*i))
					people[player_name].ChangeStat(STAT_HEALTH, STVAL_CUR, objects[player_index].TakeDamage(*i, people[i->Name()], people[player_name]));

				if (people[i->Name()].Type() != OT_OBSTACLE)
				{
					if (i->TakingDamage(objects[player_index]))
						people[i->Name()].ChangeStat(STAT_HEALTH, STVAL_CUR, i->TakeDamage(objects[player_index], people[player_name], people[i->Name()]));

					if (people[i->Name()].stat[STAT_HEALTH].val[STVAL_CUR].num <= 0)
					{
						people[i->Name()].State(OST_KO);
						i->Combo("");
						i->EffectImg(false);
					}
				}
			}
		}
}

//------------------------------------------------------------------------
// Purpose: Let the AI think
//------------------------------------------------------------------------
void Level::Think(ObjMap &people, VarMap &var)
{
	int index = 0;
	for (std::vector<Sprite>::iterator i = objects.begin(); i != objects.end(); ++i, ++index)
		if (index == player_index)
			i->UpdateMove(input.InternalEvents(frames_to_skip));
		else if (i->Visible() && people[i->Name()].State() != OST_KO)
		{
			switch (people[i->Name()].Type())
			{
			case OT_NEUTRAL:
				if (i->TakingDamage(objects[player_index]))
				{
					people[i->Name()].Type(OT_HOSTILE);
					people[i->Name()].Opinion(0);
				}
				break;
			case OT_HOSTILE:
				i->Attack(objects[player_index], people[i->Name()].stat[STAT_SPEED].val[STVAL_CUR].num);
				break;
			case OT_COWARD:
				if (i->TakingDamage(objects[player_index]))
				{
					people[i->Name()].State(OST_FLEE);
					people[i->Name()].Opinion(0);
				}

				if (people[i->Name()].State() == OST_FLEE && i->Combo() == "")
					i->RunFromSprite(objects[player_index], walk);
				break;
			default: break;
			}

			std::string str;
			if (CollidingWithLevel(*i, people, str, MODE_FIGHT))
				i->UndoMove();
		}
}