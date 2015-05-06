#ifndef REQUESTRESOURCETRANSFER_H
#define REQUESTRESOURCETRANSFER_H

#include "RequestResourceCommand.h"

namespace RON
{
	class RequestResourceTransfer : public RequestResourceCommand
	{
	public:
		RequestResourceTransfer(char Orgs, char Gas, char Metals, char Radioactives, char DarkMatter, int Ship)
		{
			resources[0] = Orgs;
			resources[1] = Gas;
			resources[2] = Metals;
			resources[3] = Radioactives;
			resources[4] = DarkMatter;
			shipId = Ship;
		}
		CommandType getType(void)
		{
			return RON::REQUEST_RESOURCE_TRANSFER;
		}
		CharBuf write(void)
		{
			CharBuf ret = writeRequestResourceCommand();
			ret.append(shipId);
			return ret;
		}

	public:
		int shipId;
	};
}


#endif