#include "FightInput.h"

bool FightInput::HandleEvents(SDL_Event Event)
{
	if (Event.type == SDL_KEYDOWN)
	{
		if (keyup[FK_PUNCH] && gInputVal[I_PUNCH].Equals(Event.key.keysym.sym))
		{
			keystate[FK_PUNCH] = true;
			keyup[FK_PUNCH] = false;
			active = true;
		}
		else if (keyup[FK_KICK] && gInputVal[I_KICK].Equals(Event.key.keysym.sym))
		{
			keystate[FK_KICK] = true;
			keyup[FK_KICK] = false;
			active = true;
		}
		else if (keyup[FK_SPECIAL] && gInputVal[I_SPECIAL].Equals(Event.key.keysym.sym))
		{
			keystate[FK_SPECIAL] = true;
			keyup[FK_SPECIAL] = false;
			active = true;
		}
	}
	else if (Event.type == SDL_KEYUP)
	{
		if (gInputVal[I_PUNCH].Equals(Event.key.keysym.sym))
		{
			keyup[FK_PUNCH] = true;
		}
		else if (gInputVal[I_KICK].Equals(Event.key.keysym.sym))
		{
			keyup[FK_KICK] = true;
		}
		else if (gInputVal[I_SPECIAL].Equals(Event.key.keysym.sym))
		{
			keyup[FK_SPECIAL] = true;
		}
	}

	return active;
}

std::string FightInput::InternalEvents(Uint16 frames_to_skip)
{
	if (active)
	{
		//if (frames_skipped == frames_to_skip)
		{
			std::string combo = GetCombo();
			Reset();
			return combo;
		}
		//else
		//	frames_skipped++;
	}

	return "";
}

void FightInput::Reset()
{
	for (int i = 0; i < FK_TOTAL; i++)
	{
		keystate[i] = false;
		keyup[i] = true;
	}

	active = false;
	frames_skipped = 0;
}

std::string FightInput::GetCombo()
{
	if (keystate[FK_PUNCH])
	{
		if (keystate[FK_KICK])
			return "pk";
		else if (keystate[FK_SPECIAL])
			return "sp";

		return "p";
	}
	else if (keystate[FK_KICK])
	{
		if (keystate[FK_PUNCH])
			return "pk";
		else if (keystate[FK_SPECIAL])
			return "sk";

		return "k";
	}
	else if (keystate[FK_SPECIAL])
	{
		if (keystate[FK_PUNCH])
			return "sp";
		else if (keystate[FK_KICK])
			return "sk";

		return "s";
	}

	return "";
}