#ifndef REQUESTTRADE_H
#define REQUESTTRADE_H
#include "commands.h"
#include "EquipmentItem.h"

namespace RON
{
	class RequestTrade : public RawCommand
	{
	public:
		CommandType getType(void)
		{
			return RON::REQUEST_TRADE;
		}

		CharBuf write(void)
		{
			CharBuf ret = CharBuf();
			ret.append(aId);
			ret.append(bId);
			ret.append(aCredits);
			if (aItem)
			{
				CharBuf item = aItem->write();
				ret.append(item.buf, item.len);
			}
			else
				ret.append((char)-128);
			ret.append(bCredits);
			if (bItem)
			{
				CharBuf item = bItem->write();
				ret.append(item.buf, item.len);
			}
			else
				ret.append((char)-128);
			if (final)
				ret.append((char)127);
			else
				ret.append((char)-128);
			return ret;
		}
		RequestTrade() { }
		RequestTrade(CharBuf &buf)
		{
			bool success;
			buf.read(&aId, &success);
			buf.read(&bId, &success);
			buf.read(&aCredits, &success);
			aItem = EquipmentItem::instantiate(buf);
			buf.read(&bCredits, &success);
			bItem = EquipmentItem::instantiate(buf);
			char finaall;
			buf.read(&finaall, &success);
			if (finaall == -128)
				final = false;
			else
				final = true;
		}

	public:

		EquipmentItem *aItem;
		EquipmentItem *bItem;

		int aCredits;
		int bCredits;

		int aId;
		int bId;

		bool final;

	};
}

#endif