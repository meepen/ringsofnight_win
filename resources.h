#ifndef RESOURCES_H
#define RESOURCES_H

namespace RON
{
	enum ResourceType
	{
		ORGANIC = 0,
		GAS,
		METAL,
		RADIOACTIVE,
		DARKMATTER,
		MAX_RESOURCES
	};
	extern char *ResourceNames[MAX_RESOURCES];
}

#endif