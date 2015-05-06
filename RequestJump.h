#ifndef REQUESTJUMP_H
#define REQUESTJUMP_H
#include "commands.h"

namespace RON
{
	class RequestJump : public RawCommand
	{
	public:
		RequestJump(char Jump)
		{
			mode = Jump;
		}

		CommandType getType(void)
		{
			return RON::REQUEST_JUMP;
		}

		CharBuf write(void)
		{
			CharBuf ret = CharBuf();
			ret.append(mode);
			return ret;
		}

	public:

		char mode;
	};
}


#endif