#ifndef GAME_H
#define GAME_H

class GAME;

#include <WinSock2.h>
#include "player.h"

extern RON::Player *eGetPlayer(int id);


#include "sector.h"
#include "corp.h"
#include "entity.h"

namespace RON
{
	class CommandReader;
	class CommandWriter;
}

class GAME
{
public:
	GAME()
	{
		for (int u = 0; u < 10; u++)
		{
			for (int x = 0; x < 40; x++)
			{
				for (int y = 0; y < 40; y++)
				{
					RON::Sector sector = sectors[u][x][y];
					sector.x = -1;
					sector.y = -1;
					sector.universe = -1;
				}
			}
		}
	}

	virtual void UpdateSector(char u, char x, char y);

	RON::Ship *GetShipInCurrentSector(int id)
	{
		RON::Entity *ent = currentsector;
		while (ent)
		{
			if (ent->type == RON::TYPE_SHIP)
			{
				if (ent->id == id)
					return (RON::Ship *)ent;
			}
			ent = ent->next;
		}
		return 0;
	}
	void Tick(void);
	void GAME::Redraw(void);
	RON::Player *GetPlayer(const char *name);
	void SendLogon(void);
	void PlayerMove(int playerId, int uni, int x, int y);
	RON::Player *GetPlayer(int id);
	RON::Corporation *GetCorp(short id);

public:
	RON::CommandReader *reader;
	RON::CommandWriter *writer;
	SOCKET socket;
	char *IV;
	struct 
	{
		int auctionFee;
		int maxItemRank;
		int hullRepairSpeed;
		float hullRepairBase;
		int hullBase;
		int moveSpeedBase;
		float npcHullMod;
		char DailySpecial;
	} magic;
	char version;
	char logintype; 
	RON::Player *players;
	RON::Sector sectors[10][40][40];
	RON::Corporation *corps;

	int local;
	int follow;
	int attack;

	const char *email;
	const char *name;
	const char *deviceid;

	RON::Entity *currentsector;

	int credits;
	RON::EquipmentItem *bank;
};

extern GAME game;

#endif