#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>

#ifdef __APPLE__
#include <SDL_mixer.h>
#elif __GNUC__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif

#include "GameParam.h"

//We store sound effects here
typedef std::unordered_map<std::string, Mix_Chunk*> ChunkMap;

class MusicManager
{
	//The sounds
	struct BackgroundMusic
	{
		std::string name;
		Mix_Music *track;
	};

	BackgroundMusic music[MODE_TOTAL];
	ChunkMap effect;

	void FreeMusic();
	void FreeChunk();

public:
	MusicManager(void);
	~MusicManager(void){}

	bool Init();
	void Quit(){ FreeMusic(); FreeChunk(); }

	void Pause(){ Mix_PauseMusic(); }
	void Resume(){ Mix_ResumeMusic(); }
	void Stop(){ Mix_HaltMusic(); }

	void VolEffects(int volume)	{ Mix_Volume(-1,volume); }
	int VolEffects() { return Mix_Volume(0,-1); }

	void VolMusic(int volume) { Mix_VolumeMusic(volume); }
	int VolMusic() { return Mix_VolumeMusic(-1); }

	bool LoadMusic(std::string talk_track, std::string fight_track);

	void PlayMusic(const GameMode mode, int loops = -1);
	void PlayEffect(std::string id, int loops = 0);
};

extern MusicManager gMusicManager;

#endif //MUSICMANAGER_H