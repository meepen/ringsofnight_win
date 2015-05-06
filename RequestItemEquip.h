#ifndef REQUESTITEMEQUIP_H
#define REQUESTITEMEQUIP_H

#include "RequestItemCommand.h"

namespace RON
{
	class RequestItemEquip : public RequestItemCommand
	{
	public:
		RequestItemEquip(EquipmentItem *item)
		{
			this->item = item;
		}

		CommandType getType(void)
		{
			return RON::REQUEST_ITEM_EQUIP;
		}
	};
}

#endif