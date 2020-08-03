#ifndef FIGHTINPUT_H
#define FIGHTINPUT_H

#include "AssetManager.h"

enum FightKey { FK_PUNCH, FK_KICK, FK_SPECIAL, FK_TOTAL };

class FightInput
{
	bool keystate[FK_TOTAL], keyup[FK_TOTAL], active;
	Uint16 frames_skipped;

public:
	FightInput(void){ Reset(); frames_skipped = 0; }
	~FightInput(void){}

	void Reset();

	bool HandleEvents(SDL_Event Event);

	std::string InternalEvents(Uint16 frames_to_skip);
	std::string GetCombo();
};

#endif //FIGHTINPUT_H