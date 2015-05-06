#ifndef REQUESTLOGIN_H
#define REQUESTLOGIN_H
#include "RequestAccountAction.h"
#include <string>

namespace RON
{
	class RequestLogin : public RequestAccountAction
	{
	public:
		RequestLogin(const char *name)
		{
			this->name = name;
			this->type = RON::REQUEST_LOGIN;
		}

		virtual CharBuf write(void)
		{
			return writeRequestLogin();
		}

		CharBuf writeRequestLogin(void)
		{
			CharBuf super = writeRequestAccountAction();
			super.append(name);
			return super;
		}

		CommandType getType(void)
		{
			return RON::REQUEST_LOGIN;
		}

	public:
		const char *name;

	};
}

#endif