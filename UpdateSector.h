#ifndef UPDATESECTOR_H
#define UPDATESECTOR_H

// needs to be finished
#include "commands.h"

namespace RON
{
	class UpdateSector : public RawCommand
	{
	public:
		CommandType getType(void)
		{
			return RON::UPDATE_SECTOR;
		}

	public:



	};
}

#endif