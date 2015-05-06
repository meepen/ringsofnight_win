#ifndef PLAYER_H
#define PLAYER_H

namespace RON
{
	class Player
	{
	public:
		Player(const char *Name, int Id, char Access)
		{
			name = Name;
			id = Id;
			accessLevel = Access;
		}

	public:
		const char *name;
		int id;
		short corpID;
		char online;
		char corpRank;
		char hardcore;
		char accessLevel;
		char x;
		char y;
		char universe;
		Player *next;
		char resources[5];
	};
}

#endif