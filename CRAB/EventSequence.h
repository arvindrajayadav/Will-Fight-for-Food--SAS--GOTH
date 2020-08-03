#ifndef EVENTSEQ_H
#define EVENTSEQ_H

#include <vector>
#include "AssetManager.h"
#include "gameevent.h"
#include "GameObjective.h"
#include "Inventory.h"
#include <steam/steam_api.h>

enum EventResultType
{
	ER_NONE, //Do nothing
	ER_LEVEL, //Change level
	ER_MOVE, //Move sprite
	ER_HEAL, //Heal object
	ER_ITEM, //Give item
	ER_SAVE, //Save game
	ER_SYNC, //Sync the level
	ER_MODE, //Quit to main menu
	ER_QUIT //Quit to main menu
};

struct EventResult
{
	EventResultType type;
	std::string val;
	Sint16 x, y;

	EventResult()
	{
		type = ER_NONE;
		val = "";
		x = -1;
		y = -1;
	}
};

class EventSequence
{
	std::vector<GameEvent> events;
	bool event_in_progress;

	std::vector<Uint32> next_event_id;
	Uint32 cur_event_id;

	//The current event id and next event id in strings, for saving purposes
	std::string str_cur_event_id;
	std::vector<std::string> str_next_event_id;

public:

	EventSequence(void)
	{
		events.clear();
		event_in_progress = false;

		next_event_id.clear();
		next_event_id.push_back(0);
		cur_event_id = 0;
	}
	~EventSequence(void){ events.clear(); }

	GameEvent* CurrentEvent() { return &events[cur_event_id]; }

	//See if we should trigger any event
	void InternalEvents(ObjMap &people, VarMap &varmap, Inventory &inv, std::string &loc, std::string &obj, int CurTrigRect);
	bool NextEvent(ObjMap &people, VarMap &varmap, GameObjective &objective, std::vector<EventResult> &result, ISteamUserStats *m_pSteamUserStats, int NextEventChoice = -1);

	bool EventInProgress() { return event_in_progress; }
	void EventInProgress(bool val) { event_in_progress = val; }

	//Load and save
	void Load(std::string filename);

	void SaveState(rapidxml::xml_document<char> &doc, rapidxml::xml_node<char> *root, const char* name);
	void LoadState(rapidxml::xml_node<char> *node);
};

#endif //EVENTSEQ_H