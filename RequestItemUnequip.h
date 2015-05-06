#ifndef REQUESTITEMUNEQUIP_H
#define REQUESTITEMUNEQUIP_H

#include "RequestItemCommand.h"

namespace RON
{
	class RequestItemUnequip : public RequestItemCommand
	{
	public:
		RequestItemUnequip(EquipmentItem *item)
		{
			this->item = item;
		}

		CommandType getType(void)
		{
			return RON::REQUEST_ITEM_UNEQUIP;
		}
	};
}

#endif