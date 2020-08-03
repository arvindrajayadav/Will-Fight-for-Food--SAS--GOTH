#include "app.h"

bool App::Init(void)
{
	//Load all SDL subsystems and the TrueType font subsystem
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1 || TTF_Init() == -1)
	{
		fprintf(stderr, "Couldn't initialize SDL :(");
		return false;
	}
	Uint32 videoflags = SDL_HWSURFACE | SDL_OPENGL;
	int volume_music = 100, volume_effects = 100;

	XMLDoc settings("res/settings.xml");
	if (settings.ready())
	{
		rapidxml::xml_node<char> *node = settings.Doc()->first_node("settings");
		if (NodeValid(node))
		{
			gScreenSettings.Load(node, videoflags);

			rapidxml::xml_node<char> *volnode = node->first_node("volume");
			if (NodeValid(volnode))
			{
				LoadNum(volume_music, "music", volnode);
				LoadNum(volume_effects, "effects", volnode);
			}

			gFilePath.Load(node->first_node("paths"));
		}
	}

	//Set the window caption
	SDL_WM_SetCaption("Will Fight for Food", "Will Fight for Food");

	//Center the window
	SDL_putenv("SDL_VIDEO_WINDOW_POS=center");

	//Initialize Steam
	SteamAPI_Init();

#ifndef __APPLE__
	//Set the window icon
	SDL_Surface *iconimage = SDL_LoadBMP(gFilePath.icon.c_str());
	Uint32 colorkey = SDL_MapRGB(iconimage->format, 255, 0, 255);
	SDL_SetColorKey(iconimage, SDL_SRCCOLORKEY, colorkey);
	SDL_WM_SetIcon(iconimage, NULL);
#endif

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 4);

	//Set keyboard repeat rate
	SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL / 2);

	//Store the default desktop values before starting our own screen
	gScreenSettings.Desktop(SDL_GetVideoInfo()->current_w, SDL_GetVideoInfo()->current_h);

	//Set up the screen
	screen = SDL_SetVideoMode(gScreenSettings.CurW(), gScreenSettings.CurH(), gScreenSettings.BitsPerPixel(), videoflags);
	if (screen == NULL)
		return false;

	// initialize GLEW
	GLenum status = glewInit();
	if (status != GLEW_NO_ERROR)
	{
		fprintf(stderr, "Failed to initialize GLEW %d: %s\n", status, glewGetErrorString(status));
		return false;
	}

	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearStencil(0);

	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0f, gScreenSettings.Width(), gScreenSettings.Height(), 0.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Disable the SDL stock cursor
	SDL_ShowCursor(SDL_DISABLE);

	//Enable Unicode for text input in text area
	SDL_EnableUNICODE(SDL_ENABLE);

	// Start up audio
#ifndef __GNUC__
	const int audioflags = MIX_INIT_OGG;
	if ((Mix_Init(audioflags) & audioflags) != audioflags)
	{
		fprintf(stderr, "Mix_Init: Failed to init!\n");
		fprintf(stderr, Mix_GetError());
	}
#endif

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	{
		fprintf(stderr, "Mix_OpenAudio: %s\n", Mix_GetError());
		return false;
	}

	//Set the volume from the settings
	gMusicManager.VolEffects(volume_effects);
	gMusicManager.VolMusic(volume_music);

	//Seed the random function
	srand(static_cast<unsigned int>(time(NULL)));

	//Load the controls
	LoadInputVal(gFilePath.controls);

	return true;
}

void App::Run()
{
	//State IDs
	GameStateID CurrentStateID = GAMESTATE_NULL, NextStateID = GAMESTATE_TITLE;
	bool ShouldChangeState = true;

	//Set the current game state object
	GameState* CurrentState = NULL;
	Timer fps;
	SDL_Event Event;

	//While the user hasn't quit - This is the main game loop
	while (CurrentStateID != GAMESTATE_EXIT)
	{
		//Start the frame timer
		fps.start();

		//Change state if needed
		if (ShouldChangeState)
		{
			//Delete the current state
			delete CurrentState;

			if (NextStateID == GAMESTATE_EXIT)
				break;

			//Change the state
			switch (NextStateID)
			{
			case GAMESTATE_TITLE:
				CurrentState = new Splash();
				break;

			case GAMESTATE_MAIN_MENU:
				CurrentState = new MainMenu();
				break;

			case GAMESTATE_NEW_GAME:
				CurrentState = new Game();
				break;

			case GAMESTATE_LOAD_GAME:
				CurrentState = new Game(gSaveMenu.SelectedSave());
				break;

			default:
				//Encountering an undefined state, exit with an error code
				return;
			}

			//Change the current state ID
			CurrentStateID = NextStateID;

			//NULL the next state ID
			NextStateID = GAMESTATE_NULL;

			//No need to change state until further notice
			ShouldChangeState = false;
		}

		//Do state InternalEvents
		CurrentState->InternalEvents(ShouldChangeState, NextStateID);

		while (SDL_PollEvent(&Event))
		{
			//Do state Event handling
			CurrentState->HandleEvents(Event, ShouldChangeState, NextStateID);

			//Do general event handling such as resizing window, exit game common to all states
			//If the user has closed the window or pressed ALT+F4
			if (Event.type == SDL_QUIT || (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_F4 && Event.key.keysym.mod & KMOD_ALT))
			{
				//Quit the program
				ShouldChangeState = true;
				NextStateID = GAMESTATE_EXIT;
			}

			//DOES NOT WORK WITH OPENGL
			//Resize the window if user presses ALT+ENTER
			//if(Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_RETURN && Event.key.keysym.mod & KMOD_ALT)
			//	ToggleFullScreen(screen);
		}

		//Do state Drawing
		CurrentState->Draw();

		//Update the screen
		SDL_GL_SwapBuffers();
		glClear(GL_COLOR_BUFFER_BIT);

		//Cap the frame rate
		if (fps.get_ticks() < 1000u / gScreenSettings.FramesPerSecond())
			SDL_Delay((1000u / gScreenSettings.FramesPerSecond()) - fps.get_ticks());
	}
}

App::~App(void)
{
	SteamAPI_Shutdown();
	gAssetManager.Quit();
	gMusicManager.Quit();
	gTextManager.Quit();
	gLoadScreen.Quit();

	//Quit SDL_ttf
	TTF_Quit();

	//Quit the audio stuff
	Mix_CloseAudio();

#ifndef __GNUC__
	Mix_Quit();
#endif

	//Quit SDL
	SDL_Quit();
}