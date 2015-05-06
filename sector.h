#ifndef SECTOR_H
#define SECTOR_H

namespace RON
{
	extern char *DirectionNames[];

	enum Direction
	{
		NORTH = 0,
		NORTHEAST,
		EAST,
		SOUTHEAST,
		SOUTH,
		SOUTHWEST,
		WEST,
		NORTHWEST,
		MAX_DIRECTIONS
	};

	class Sector
	{
	public:
		bool isLinked(Direction dir)
		{
			return (linkConfig & (1 << dir)) != 0;
		}

	public:
		char x;
		char y;
		char universe;
		char hasPlanet;
		char linkConfig;
		char faction;
		short corpID;
		char _class;
		char asteroid;
		char hasStarPort;

	};
}


#endif