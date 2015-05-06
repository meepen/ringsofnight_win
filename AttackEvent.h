#ifndef ATTACKEVENT_H
#define ATTACKEVENT_H
#include "commands.h"

namespace RON
{
	class AttackEvent : public RawCommand
	{
	public:

		AttackEvent(int attackid)
		{
			attack = attackid;
		}
		CommandType getType(void)
		{
			return RON::ATTACK_EVENT;
		}

		CharBuf write(void)
		{
			CharBuf ret = CharBuf();
			ret.append(attack);
			return ret;
		}
	public:
		int attack;
	};
}

#endif