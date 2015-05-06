#ifndef REQUESTHARDCORE_H
#define REQUESTHARDCORE_H
#include "commands.h"

namespace RON
{
	class RequestHardcore : public RawCommand
	{
	public:
		CommandType getType(void)
		{
			return RON::REQUEST_HARDCORE;
		}

		CharBuf write(void)
		{
			CharBuf ret = CharBuf();
			return ret;
		}
	};
}

#endif