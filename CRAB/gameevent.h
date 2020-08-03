#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include "AssetManager.h"
#include "object.h"
#include "Conversation.h"
#include "PopUp.h"
#include "animation.h"

enum EventType
{
	EVENT_DIALOG,
	EVENT_REPLY,
	EVENT_TALK,
	EVENT_TEXT,
	EVENT_POPUP,
	EVENT_ANIM,
	EVENT_SILENT
};

enum TriggerType
{
	TRIG_LOC, //Being in a place on the map
	TRIG_OBJ, //Interacting with an object
	TRIG_HEALTH, //A character's health
	TRIG_OBJ_STATUS, //Status of character (hostile, coward etc)
	TRIG_OBJ_STATE, //Change state of character (stand, fight, flee, KO etc)
	TRIG_VAR, //A variable is present or not
	TRIG_ITEM, //An item is present or not
	TRIG_RECT //The player is colliding with a rectangle
};

struct Trigger
{
	TriggerType type;
	std::string subject, val;
};

enum EffectType
{
	EFF_VAR, //Add or remove a variable
	EFF_AIM, //Add an objective to the player quest book
	EFF_OPINION, //Change opinion of a character
	EFF_OBJ_STATUS, //Change status of character (hostile, coward etc)
	EFF_OBJ_STATE, //Change state of character (stand, fight, flee, KO etc)
	EFF_ACHIEVEMENT, //Provide achievement to player
	EFF_END, //End of the event sequence, remove it from active sequences
	//EFFECT DIVISION HERE
	EFF_MOVE, //Make a character move
	EFF_LEVEL, //Load a new level
	EFF_HEALTH, //Replenish some of the player's health
	EFF_ITEM, //Add/remove an item in the player's inventory
	EFF_SAVE, //Auto save the game
	EFF_MODE, //Change the mode of the game
	EFF_QUIT //Quit to main menu
};

struct Effect
{
	EffectType type;
	std::string subject, val;
};

struct GameEvent
{
	//The event identifier
	Uint32 id;

	//The heading for the dialog spoken
	std::string title;

	//The dialog spoken in the event
	std::string dialog;

	//Has this event taken place?
	bool happened;

	//Event type and related data
	bool alt;
	EventType type;
	std::vector<std::string> reply;
	ConversationData con;
	PopUpData pop;
	AnimationData anim;

	//The variables changed/added in the event
	std::vector<Effect> effect;

	//The triggers for the event
	std::vector<Trigger> trig;

	//The id of the next event
	std::vector<Uint32> nextid;

	//The id in string form
	std::string str_id;

	GameEvent()
	{
		id = 0;
		str_id = "0";
		title = "";
		happened = false;
		type = EVENT_DIALOG;
		dialog = "";
		alt = false;
	}

	GameEvent(rapidxml::xml_node<char> *node) { Load(node); }
	~GameEvent(){}

	void Load(rapidxml::xml_node<char> *node);
};

typedef std::unordered_map<std::string, bool> VarMap;

#endif //GAME_EVENT_H