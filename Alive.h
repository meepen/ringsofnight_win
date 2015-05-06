#ifndef ALIVE_H
#define ALIVE_H
#include "commands.h"

namespace RON
{
	class Alive : public RawCommand
	{
	public:
		CommandType getType(void)
		{
			return RON::ALIVE;
		}

	public:
	};
}

#endif