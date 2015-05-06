#ifndef REQUESTNEWACCOUNT_H
#define REQUESTNEWACCOUNT_H
#include "RequestAccountAction.h"

namespace RON
{
	class RequestNewAccount : public RequestAccountAction
	{
	public:
		CommandType getType(void)
		{
			return RON::REQUEST_NEW_ACCOUNT;
		}

		RequestNewAccount(const char *name)
		{
			this->name = name;
		}

		CharBuf write(void)
		{
			CharBuf ret = RequestAccountAction::write();
			ret.append(name);
			return ret;
		}

	public:
		const char *name;
	};
}

#endif