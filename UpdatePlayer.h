#ifndef UPDATEPLAYER_H
#define UPDATEPLAYER_H
#include "commands.h"

namespace RON
{
	class UpdatePlayer : public RawCommand
	{
	public:
		CommandType getType(void)
		{
			return RON::UPDATE_PLAYER;
		}
	};
}

#endif