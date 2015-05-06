#ifndef REQUESTPLAYER_H
#define REQUESTPLAYER_H
#include "commands.h"

namespace RON
{
	class RequestPlayer : public RawCommand
	{
	public:
		CommandType getType(void)
		{
			return RON::REQUEST_PLAYER;
		}
		RequestPlayer(int Id)
		{
			id = Id;
		}
		CharBuf write(void)
		{
			CharBuf ret = CharBuf();
			ret.append(id);
			return ret;
		}
	public:
		int id;
	};
}

#endif