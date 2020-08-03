#ifndef OBJECT_H
#define OBJECT_H

#include "AssetManager.h"
#include "ConversationPref.h"
#include "Stat.h"

enum GameObjectType
{
	OT_NEUTRAL,
	OT_HOSTILE,
	OT_COWARD,
	OT_IMMOBILE,
	OT_OBSTACLE,
	OT_PROP
};

enum GameObjectState
{
	OST_STAND,
	OST_FIGHT,
	OST_FLEE,
	OST_KO
};

extern int MAX_OPINION, MIN_OPINION;

class GameObject
{
	//Type of object
	GameObjectType type;

	//The state of the object, defines what behavior it is doing right now
	GameObjectState state;

	//The opinion of the character about the player
	//Range 0 to 100 , 100 = absolutely adore you, and 0 = really hate you
	int opinion;

	//Strings of attributes to store them in save files
	std::string str_opinion;
public:

	//The stats of the character
	Stat stat[STAT_TOTAL];

	//Name of object
	std::string name;

	//The picture of the object
	std::string pic_normal, pic_friendly, pic_hostile;

	//The hints you receive about the character
	std::vector<std::string> hint;

	//The conversation options the person prefers
	ConversationPref cp_like, cp_hate;

	GameObject(std::string Name = "", int Op = MAX_OPINION / 2);
	void Load(rapidxml::xml_node<char> *node);

	int Opinion() { return opinion; }
	void Opinion(int val);
	void ChangeOpinion(int val);

	GameObjectState State() { return state; }
	void State(GameObjectState newstate);

	GameObjectType Type() { return type; }
	void Type(GameObjectType newtype);

	void ChangeStat(StatType type, StatVal st, int change)
	{
		if (type == STAT_HEALTH)
			stat[type].Change(st, change, true);
		else
			stat[type].Change(st, change, false);
	}
	void SetStat(StatType type, StatVal st, int val)
	{
		if (type == STAT_HEALTH)
			stat[type].Set(st, val, true);
		else
			stat[type].Set(st, val, false);
	}

	void ResetStat()
	{
		stat[STAT_HEALTH].Set(STVAL_FULL, 100, false);
		stat[STAT_ATTACK].Reset();
		stat[STAT_DEFENSE].Reset();
		stat[STAT_SPEED].Reset();
	}

	void ValidateStat()
	{
		stat[STAT_HEALTH].Validate();
		stat[STAT_ATTACK].Validate();
		stat[STAT_DEFENSE].Validate();
		stat[STAT_SPEED].Validate();
	}

	void SaveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root);
	void LoadState(rapidxml::xml_node<char> *node);
};

typedef std::unordered_map<std::string, GameObject> ObjMap;

#endif //OBJECT_H