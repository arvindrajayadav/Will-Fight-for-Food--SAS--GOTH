#include "mainmenu.h"

//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
MainMenu::MainMenu()
{
	XMLDoc conf(gFilePath.mainmenu_l);
	if (conf.ready())
	{
		rapidxml::xml_node<char> * node = conf.Doc()->first_node("main_menu");
		if (NodeValid(node))
		{
			me_main.Load(node->first_node("main"));

			rapidxml::xml_node<char> *diffnode = node->first_node("difficulty");
			if (NodeValid(diffnode))
			{
				me_diff.Load(diffnode);
				title_diff.Load(diffnode->first_node("title"));
			}

			back.Load(node->first_node("back"));
			option_menu.Load(node->first_node("option"));
			mod.Load(node->first_node("mod"));
			gSaveMenu.Load(node->first_node("load"));
			ss_help.Load(node->first_node("help"));
			me_customize.Load(node->first_node("customize"));
		}
	}

	gMusicManager.Stop();
	gMusicManager.LoadMusic("main_menu", "main_menu");
	gMusicManager.PlayMusic(MODE_RPG);

	ChangeState(STATE_NORMAL);
}
//------------------------------------------------------------------------
// Purpose: Event/input handling InternalEvents
//------------------------------------------------------------------------
void MainMenu::HandleEvents(SDL_Event& Event, bool& ShouldChangeState, GameStateID& NewStateID)
{
	gMouse.HandleEvents(Event);
	switch (CurState)
	{
	case STATE_OPTIONS:
		if (option_menu.HandleEvents(back, Event))
			ChangeState(STATE_NORMAL);
		break;

	case STATE_NORMAL:
	{
		int choice = me_main.HandleEvents(Event);
		switch (choice)
		{
		case 0:
			if (gSaveMenu.SelectNewestFile())
			{
				ChangeState(STATE_NORMAL);
				ShouldChangeState = true;
				NewStateID = GAMESTATE_LOAD_GAME;
			}
			break;
		case 1: ChangeState(STATE_DIFF); break;
		case 2:	ChangeState(STATE_LOAD); gSaveMenu.ScanDir(); break;
		case 3: ChangeState(STATE_OPTIONS); break;
		case 4:	ChangeState(STATE_MOD); break;
		case 5:	ChangeState(STATE_HELP); break;
		case 6:	ChangeState(STATE_CREDITS); break;
		case 7: ShouldChangeState = true; NewStateID = GAMESTATE_EXIT; break;
		default:break;
		}
	}
	break;

	case STATE_CREDITS:
		if ((Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_ESCAPE) || back.HandleEvents(Event))
			ChangeState(STATE_NORMAL);
		break;

	case STATE_LOAD:
		if (gSaveMenu.HandleEvents(Event))
		{
			ChangeState(STATE_NORMAL);
			ShouldChangeState = true;
			NewStateID = GAMESTATE_LOAD_GAME;
			return;
		}

		if ((Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_ESCAPE) || back.HandleEvents(Event))
			ChangeState(STATE_NORMAL);
		break;

	case STATE_DIFF:
		if ((Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_ESCAPE) || back.HandleEvents(Event))
			ChangeState(STATE_NORMAL);
		else
		{
			int choice = me_diff.HandleEvents(Event);
			if (choice >= 0)
			{
				difficulty = (GameDifficulty)choice;
				ChangeState(STATE_SKIN);
			}
		}
		break;

	case STATE_SKIN:
		if ((Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_ESCAPE) || back.HandleEvents(Event))
			ChangeState(STATE_DIFF);
		else
		{
			if (me_customize.HandleEvents(Event))
			{
				ShouldChangeState = true;
				NewStateID = GAMESTATE_NEW_GAME;
			}
		}
		break;

	case STATE_MOD:
	{
		if (mod.HandleEvents(Event))
			ChangeState(STATE_NORMAL);

		if ((Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_ESCAPE) || back.HandleEvents(Event))
			ChangeState(STATE_NORMAL);
	}
	break;

	case STATE_HELP:
	{
		if (ss_help.HandleEvents(back, Event))
			ChangeState(STATE_NORMAL);
	}
	break;

	default:break;
	}
}
//------------------------------------------------------------------------
// Purpose: Event/Input Independent InternalEvents
//------------------------------------------------------------------------
void MainMenu::InternalEvents(bool& ShouldChangeState, GameStateID& NewStateID)
{
}
//------------------------------------------------------------------------
// Purpose: Drawing function
//------------------------------------------------------------------------
void MainMenu::Draw()
{
	switch (CurState)
	{
	case STATE_NORMAL:
		//Draw the menu
		gAssetManager.DrawTexture(0, 0, gFilePath.main_bg);
		if (gFilePath.mod_cur != "res/default.xml")
			gTextManager.Draw(gScreenSettings.Width() - 20, 50, gFilePath.mod_cur, white, 2, ALIGN_RIGHT);
		me_main.Draw();
		break;

	case STATE_OPTIONS:
		option_menu.Draw(back);
		break;

	case STATE_CREDITS:
		DrawCredits();
		break;

	case STATE_LOAD:
		gAssetManager.DrawTexture(0, 0, gFilePath.load_bg);
		gSaveMenu.Draw();
		back.Draw();
		break;

	case STATE_DIFF:
		gAssetManager.DrawTexture(0, 0, gFilePath.diff_bg);
		title_diff.Draw();
		me_diff.Draw();
		back.Draw();
		break;

	case STATE_SKIN:
		gAssetManager.DrawTexture(0, 0, gFilePath.skin_bg);
		me_customize.Draw();
		back.Draw();
		break;

	case STATE_MOD:
		mod.Draw();
		back.Draw();
		break;

	case STATE_HELP:
		ss_help.Draw(back);
		break;

	default:break;
	}

	gAssetManager.DrawMouse(gMouse.MotionX(), gMouse.MotionY());
}

void MainMenu::DrawCredits()
{
	gAssetManager.DrawTexture(0, 0, gFilePath.cred_bg);
	gTextManager.Draw(640, 60, "Thank you for playing Will Fight for Food: Super Actual Sellout: Game of the Hour!", white, 2, ALIGN_CENTER);

	Sint16 x1 = 100, y1 = 190, y2 = 187, inc = 60;

	gTextManager.Draw(x1, y1, "Art :", yellow, 2, ALIGN_LEFT);
	gTextManager.Draw(x1 + 40, y2, "Mikk Luige, Rashi Chandra, Vinh Duong", white, 1, ALIGN_LEFT);
	y1 += inc; y2 += inc;

	gTextManager.Draw(x1, y1, "Writer :", yellow, 2, ALIGN_LEFT);
	gTextManager.Draw(x1 + 60, y2, "Adam \"Rutskarn\" DeCamp", white, 1, ALIGN_LEFT);
	y1 += inc; y2 += inc;

	gTextManager.Draw(x1, y1, "Music :", yellow, 2, ALIGN_LEFT);
	gTextManager.Draw(x1 + 60, y2, "Aaron Kearns", white, 1, ALIGN_LEFT);
	y1 += inc; y2 += inc;

	gTextManager.Draw(x1, y1, "Designer :", yellow, 2, ALIGN_LEFT);
	gTextManager.Draw(x1 + 80, y2, "Ian Nicholls", white, 1, ALIGN_LEFT);
	y1 += inc; y2 += inc;

	gTextManager.Draw(x1, y1, "Scripting :", yellow, 2, ALIGN_LEFT);
	gTextManager.Draw(x1 + 80, y2, "Ross Zevenhuizen", white, 1, ALIGN_LEFT);
	y1 += inc; y2 += inc;

	gTextManager.Draw(x1, y1, "Mac OS X Port :", yellow, 2, ALIGN_LEFT);
	gTextManager.Draw(x1 + 120, y2, "Sanchit Gulati", white, 1, ALIGN_LEFT);
	y1 += inc; y2 += inc;

	gTextManager.Draw(x1, y1, "Programming and Design :", yellow, 2, ALIGN_LEFT);
	gTextManager.Draw(x1 + 200, y2, "Arvind Raja Yadav", white, 1, ALIGN_LEFT);
	y1 += inc; y2 += inc;

	gTextManager.Draw(1080, 600, "http://pyrodactyl.com", yellow, 1, ALIGN_CENTER);
	gTextManager.Draw(1080, 640, "@pyrodactylgames", yellow, 1, ALIGN_CENTER);

	back.Draw();
}