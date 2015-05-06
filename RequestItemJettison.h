#ifndef REQUESTITEMJETTISON_H
#define REQUESTITEMJETTISON_H
#include "RequestItemCommand.h"

namespace RON
{
	class RequestItemJettison : public RequestItemCommand
	{
	public:
		RequestItemJettison(EquipmentItem *item)
		{
			this->item = item;
		}
		CommandType getType(void)
		{
			return RON::REQUEST_ITEM_JETTISON;
		}
	};
}

#endif