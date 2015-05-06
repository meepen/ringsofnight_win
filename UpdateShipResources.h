#ifndef REQUESTRESOURCETRANSFER_H
#define REQUESTRESOURCETRANSFER_H

#include "RequestResourceCommand.h"

namespace RON
{
	class UpdateShipResources
	{
	public:
		CommandType getType(void)
		{
			return RON::UPDATE_SHIP_RESOURCES;
		}
		CharBuf write(void)
		{
			CharBuf ret = CharBuf();
			ret.append(shipId);
			for (int i = 0; i < 5; i++)
				ret.append(resources[i]);
			return ret;
		}

	public:
		int shipId;
		char resources[5];
	};
}


#endif