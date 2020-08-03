#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#ifdef __GNUC__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include <vector>
#include "numstr.h"
#include "XMLDoc.h"
#include "loaders.h"

enum InputType
{
	I_NONE = -1,
	I_UP,
	I_DOWN,
	I_RIGHT,
	I_LEFT,
	I_PUNCH,
	I_KICK,
	I_SPECIAL,
	I_MODE,
	I_TALK,
	I_MAP,
	I_OBJECTIVE,
	I_INVENTORY,
	I_PAUSE,
	I_TOTAL,
	I_QUIT
};

struct InputVal
{
	//Keyboard keys
	SDLKey key, alt;

	//Name of the key
	std::string name;

	//Controller keys

	bool Equals(const SDLKey val) { return (val == key || val == alt); }

	bool Pressed(const Uint8* state) { return state[key] || state[alt]; }
};

void LoadInputVal(std::string filename);
void WriteInputVal(std::string filename);

extern InputVal gInputVal[I_TOTAL];

#endif //INPUTMANAGER_H