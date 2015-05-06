#ifndef SETATTACK_H
#define SETATTACK_H
#include "commands.h"

namespace RON
{
	class SetAttack : public RawCommand
	{
	public:

		SetAttack(int attackid)
		{
			attack = attackid;
		}
		CommandType getType(void)
		{
			return RON::SET_ATTACK;
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