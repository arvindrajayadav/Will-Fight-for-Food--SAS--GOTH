#ifndef GAMEEVENTMANAGER_H
#define GAMEEVENTMANAGER_H

#include "AssetManager.h"
#include "gameevent.h"
#include "menu.h"
#include "Conversation.h"
#include "textarea.h"
#include "sprite.h"
#include "EventSequence.h"
#include "dialogbox.h"
#include "object.h"
#include "ObjectHandler.h"
#include "GameObjective.h"
#include "ReplyMenu.h"
#include "level.h"

typedef std::unordered_map<std::string, EventSequence> EventMap;

class GameEventManager
{
protected:
	//All the events in the game
	EventMap event_map;

	//The currently happening or active sequence
	std::string active_sequence;

	//The event sequences that have ended
	std::vector<std::string> event_end;

	//The objects used to draw the event
	GameDialogBox dlbox;
	ConversationHandler chan;
	TextArea textin;
	PopUpHandler pop;
	GameObjectHandler oh;
	AnimationHandler ah;

	//The reply menu and the colors and font of the text
	ReplyMenu reply;

	//Stuff related to conversation
	struct
	{
		bool visible;
		Uint16 val;
	} conv_ans;

	void LoadPeople(std::string filename);

public:
	//The characters in the game
	ObjMap people;

	//The variables set by the events so far
	VarMap var;

	// Steam UserStats interface
	ISteamUserStats *m_pSteamUserStats;

	GameEventManager() { Init(); }
	~GameEventManager(){}

	void Init();
	void Load(std::string eventfile, std::string layoutfile, std::string charfile, std::string convfile, std::string sentencefile);

	bool EventInProgress()
	{
		if (active_sequence == "")
			return false;
		return true;
	}

	int Health(std::string name) { return people[name].stat[STAT_HEALTH].val[STVAL_CUR].num; }
	void Heal(std::string name, Sint16 mod);

	void AddEventResponse(Uint16 id);
	void Draw();

	void InternalEvents(std::string object, std::string curloc, GameObjective &objective, Inventory &inv,
		std::vector<EventResult> &result, int &CurTrigRect, Level &level);
	void HandleEvents(GameObjective &objective, SDL_Event Event, std::vector<EventResult> &result);
	void CalcActiveSeq(Level &level);

	void ApplyResult(bool end_seq)
	{
		if (end_seq)
		{
			event_map.erase(active_sequence);
			event_end.push_back(active_sequence);
			active_sequence = "";
		}
	}

	void SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root);
	void LoadState(rapidxml::xml_node<char> *node);
};

#endif //GAME_EVENT_MANAGER_H