#ifndef REQUESTACCOUNTACTION_H
#define REQUESTACCOUNTACTION_H
#include "commands.h"
#include <WinSock2.h>

#define BUNDLE_SIZE 128

namespace RON
{
	struct RequestAccountActionBundle
	{
		char *hex;
		short len;
	};
	class RequestAccountAction : public RawCommand
	{
	public:

		RequestAccountAction(void)
		{
			bundle = 0;
			numbundle = 0;
		}

		~RequestAccountAction(void)
		{
			for (int i = 0; i < numbundle; i++)
				delete[] bundle[i];
			delete[] bundle;
		}

		virtual CharBuf write(void)
		{
			return writeRequestAccountAction();
		}

		CharBuf writeRequestAccountAction(void)
		{
			CharBuf ret = CharBuf();
			ret.append((short)numbundle);
			for (int i = 0; i < numbundle; i++)
			{
				auto curbundle = bundle[i];
				ret.append(curbundle->len);
				ret.append(curbundle->hex, curbundle->len);
			}
			return ret;
		}

	public:
		RequestAccountActionBundle **bundle;
		int numbundle;
	};
}

#endif