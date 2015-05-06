#ifndef REQUESTCORPACCEPTINVITE_H
#define REQUESTCORPACCEPTINVITE_H
#include "commands.h"

namespace RON
{
	class RequestCorpAcceptInvite : public RawCommand
	{
	public:
		CommandType getType(void)
		{
			return RON::REQUEST_CORP_ACCEPT_INVITE;
		}
	};
}

#endif