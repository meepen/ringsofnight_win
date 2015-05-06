#ifndef REMOVEENTITY_H
#define REMOVEENTITY_H
#include "commands.h"

namespace RON
{
	enum JUMP_TYPES
	{
		DESTROYED = 1,
		MOVED,
		EARTH_JUMP,
		CORP_JUMP,
		RIFT_JUMP,
		LOGOUT,
		DRAGGED,
	};

	class RemoveEntity : public RawCommand
	{
		typedef RawCommand super;
	public:

		RemoveEntity(CharBuf &buf)
		{
			bool success;
			buf.read(&id, &success);
			buf.read(&_event, &success);
			if (_event == DRAGGED || _event == MOVED)
				buf.read(&direction, &success);
		}

	public:
		char direction;
		char _event;
		int id;

	};
}

#endif