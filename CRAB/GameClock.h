#ifndef GAMECLOCK_H
#define GAMECLOCK_H

#include "timer.h"

class GameClock
{
	Timer timer;
	Uint32 start;

public:
	GameClock()
	{
		start = 0;
	}

	void Start(Uint32 initial_time = 0)
	{
		start = initial_time;
		timer.start();
	}

	void Start(std::string str)
	{
		Uint16 found = str.find(':');
		Uint32 ms = 0;

		if(found > 0 && found != std::string::npos)
		{
			std::string str_hours = str.substr(0,found-1);
			Uint16 hr = StringToNumber<Uint16>(str_hours);

			std::string str_min = str.substr(found+1,std::string::npos);
			Uint16 mins = StringToNumber<Uint16>(str_min);

			ms = 3600000 * hr + 60000 * mins;
		}

		Start(ms);
	}

	std::string GetTime()
	{
		Uint32 ms = start + timer.get_ticks();
		Uint32 hr = ms / 3600000;
		Uint32 mins = (ms % 3600000) / (60000);

		return NumberToString<Uint32>(hr) + ':' + NumberToString<Uint32>(mins);
	}
};

#endif //GAMECLOCK_H