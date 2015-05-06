#ifndef REQUESTITEMENGINEER_H
#define REQUESTITEMENGINEER_H

#include "RequestItemCommand.h"

namespace RON
{
	class RequestItemEngineer : public RequestItemCommand
	{
		typedef RequestItemCommand super;
	public:
		RequestItemEngineer(EquipmentItem *item, char mode)
		{
			this->item = item;
			this->mode = mode;
		}

		CharBuf write(void)
		{
			CharBuf ret = super::write();
			ret.append(mode);
			return ret;
		}

		CommandType getType(void)
		{
			return RON::REQUEST_ITEM_ENGINEER;
		}

	public:
		char mode;
	};
}

#endif