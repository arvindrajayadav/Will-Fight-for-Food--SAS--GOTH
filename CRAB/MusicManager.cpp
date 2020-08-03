#include "MusicManager.h"

MusicManager gMusicManager;

MusicManager :: MusicManager()
{
	music[MODE_RPG].track = NULL;
	music[MODE_FIGHT].track = NULL;
}

void MusicManager :: FreeMusic()
{
	Mix_FreeMusic(music[0].track);
	Mix_FreeMusic(music[1].track);
}

void MusicManager :: FreeChunk()
{
	for(ChunkMap::iterator it = effect.begin(); it != effect.end(); ++it)
		Mix_FreeChunk(it->second);
}

//------------------------------------------------------------------------
// Purpose: Play music
//------------------------------------------------------------------------
void MusicManager :: PlayMusic(const GameMode mode, int loops)
{
	if(music[mode].track != NULL)
		Mix_FadeInMusic(music[mode].track,loops,1000);
}

void MusicManager :: PlayEffect(std::string id, int loops)
{
	if(effect.count(id) > 0)
		Mix_PlayChannel(-1, effect[id], loops);
}

//------------------------------------------------------------------------
// Purpose: Load music
//------------------------------------------------------------------------
bool MusicManager :: Init()
{
	XMLDoc track_list(gFilePath.sound_effect);
	if(track_list.ready())
	{
		rapidxml::xml_node<char> *node = track_list.Doc()->first_node("effects");
		for(rapidxml::xml_node<char> *n = node->first_node("track"); n != NULL; n = n->next_sibling("track"))
		{
			rapidxml::xml_attribute<char> *name = n->first_attribute("name"), *path = n->first_attribute("path");
			if(name != NULL && path != NULL)
				effect[name->value()] = Mix_LoadWAV(path->value());
		}
	}

	return true;
}

bool MusicManager :: LoadMusic(std::string talk_track, std::string fight_track)
{
	if(talk_track != music[0].name && fight_track != music[0].name)
	{
		FreeMusic();
		XMLDoc track_list(gFilePath.sound_music);
		bool talkload = false, fightload = false;

		if(track_list.ready())
		{
			rapidxml::xml_node<char> *node = track_list.Doc()->first_node("music");
			for(rapidxml::xml_node<char> *n = node->first_node("track"); n != NULL; n = n->next_sibling("track"))
			{
				rapidxml::xml_attribute<char> *name = n->first_attribute("name"), *path = n->first_attribute("path");
				if(name != NULL && path != NULL)
				{
					if(talk_track == name->value())
					{
						music[0].name = talk_track;
						music[0].track = Mix_LoadMUS(path->value());
						talkload = true;
					}

					if(fight_track == name->value())
					{
						music[1].name = fight_track;
						music[1].track = Mix_LoadMUS(path->value());
						fightload = true;
					}
				}

				if(fightload && talkload)
					return true;
			}
		}
	}
	return false;
}