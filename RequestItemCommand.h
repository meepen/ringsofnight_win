#ifndef REQUESTITEMCOMMAND_H
#define REQUESTITEMCOMMAND_H
#include "commands.h"
#include "EquipmentItem.h"


namespace RON
{
	class RequestItemCommand : public RawCommand
	{
	public:

		RequestItemCommand() { }

		RequestItemCommand(EquipmentItem *item)
		{
			this->item = item;
		}

		CharBuf write(void)
		{
			CharBuf ret = item->write();
			return ret;
		}

	public:
		EquipmentItem *item;
	};
}

#endif