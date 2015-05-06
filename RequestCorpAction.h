#ifndef REQUESTCORPACTION_H
#define REQUESTCORPACTION_H

#include "commands.h"

namespace RON
{
	class RequestCorpAction : public RawCommand
	{
	public:
		RequestCorpAction(char Action, const char *Text)
		{
			action = Action;
			text = Text;
		}

		CommandType getType(void)
		{
			return RON::REQUEST_CORP_ACTION;
		}

		CharBuf write(void)
		{
			CharBuf ret = CharBuf();
			ret.append(action);
			ret.append(text);
			return ret;
		}

	public:
		char action;
		const char *text;
	};
}

#endif