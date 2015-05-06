#ifndef ATTACKEVENT_H
#define ATTACKEVENT_H
#include "commands.h"

namespace RON
{
	class RequestResourceCommand : public RawCommand
	{
	public:

		RequestResourceCommand(void) { }
		CommandType getType(void)
		{
			return 0;
		}

		CharBuf write(void)
		{
			return writeRequestResourceCommand();
		}

		CharBuf writeRequestResourceCommand(void)
		{
			CharBuf ret = CharBuf();
			for (int i = 0; i < 5; i++)
				ret.append(resources[i]);
			return ret;
		}
	public:
		char resources[5];
	};
}

#endif