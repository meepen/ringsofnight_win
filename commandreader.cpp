#define _CRT_SECURE_NO_WARNINGS
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#pragma comment (lib, "Shlwapi.lib")

#include "commandreader.h"
#include "commandwriter.h"
#include "ChatMessage.h"
#include "resources.h"
#include "SetAttack.h"
#include <iostream>
#include "game.h"
#include <Shlwapi.h>
#include "entity.h"
#include "RequestTrade.h"
#include "RemoveEntity.h"

using namespace RON;

extern GAME game;
extern bool chatbot;

typedef unsigned char byte;

int typeChart[] = {
	6, // weapon
	5, // armor
	4, // storage
	8, // harvester
	10,// engine
	6, // computer
	10,// special
};

int CalculateAuggingCost(int type, int rarity, int percent)
{
	int max = rarity * rarity * rarity * rarity;
	max = 110 * (max * typeChart[type]);

	return max / 25 + max * percent / 90;
}

void CommandReader::run(void)
{
	int error = 0;
	int lasterror = 0;
	union
	{
		char chrs[sizeof(short)];
		short size;
	};

	u_long amount;

	ioctlsocket(socket, FIONREAD, &amount);
	if (amount == 0)
		return;
	while (error != sizeof(size))
	{
		lasterror = recv(socket, chrs + error, sizeof(chrs) - error, 0);
		if (lasterror == 0 && error == 0) // no more messages to read as of now
			return;
		if (lasterror == SOCKET_ERROR)
			return close();
		error += lasterror;
	}
	char tmp = chrs[0];
	chrs[0] = chrs[1];
	chrs[1] = tmp;
	
	char *buf = new char[size]; // short max

	error = 0;
	while (error != size)
	{
		lasterror = recv(socket, buf + error, size - error, 0);
		if (lasterror == SOCKET_ERROR)
			return close();
		error += lasterror;
	}
	RawCommand *newcommand = new RawCommand(buf, size);
	if (!commands)
	{
		commands = newcommand;
	}
	else
	{
		// todo: make this not a loop?
		RawCommand *temp = commands;
		while (temp)
			if (temp->next != 0)
				temp = temp->next;
			else
				break;

		temp->next = newcommand;
	}
}

void CommandReader::process(void)
{
	if (!commands)
		return;
	while (commands)
	{
		RawCommand *command = commands;
		commands = command->next;
		char chr;
		char chr2;
		char chr3;
		FILE *file;
		int integer;
		int integer2;
		int integer3;
		bool success;
		char *str;
		char *str2;
		char *str3;
		short shrt;
		short shrt2;
		short shrt3;
		Entity *ent;
		Sector *sector;
		Player *ply;
		Corporation *corp;
		CharBuf buf = CharBuf(command->buffer, command->size);
		buf.read(&chr, &success);
		if (success)
		{
			switch (chr)
			{
			case RON::HANDSHAKE:

				buf.read(&chr, &success);

				game.IV = new char[chr];
				buf.read(game.IV, chr, &success);
				if (!success)
				{
					printf("I failed! @ RON::HANDSHAKE in process (1)\n");
					Sleep(1000);
					exit(0);
				}
				fopen_s(&file, "iv.hex", "wb");
				fwrite(game.IV, sizeof(char), chr, file);
				fclose(file);
				buf.read(&game.version, &success);
				if (!success)
				{
					printf("I failed! @ RON::HANDSHAKE in process (2)\n");
					Sleep(1000);
					exit(0);
				}
				printf("Handshake received! Server version %i received!\nSending login!!\n", game.version);
				game.SendLogon();
				break;

			case RON::SET_MAGIC_VALUES:

				buf.read(&game.magic.auctionFee, &success);
				buf.read(&game.magic.maxItemRank, &success);
				buf.read(&game.magic.hullRepairSpeed, &success);
				buf.read(&game.magic.hullRepairBase, &success);
				buf.read(&game.magic.hullBase, &success);
				buf.read(&game.magic.moveSpeedBase, &success);
				buf.read(&game.magic.npcHullMod, &success);
				buf.read(&game.magic.DailySpecial, &success);

				printf(
					"AuctionFee: %i\n"
					"MaxItemRank: %i\n"
					"HullRepairSpeed: %i\n"
					"HullRepairBase: %f\n"
					"HullBase: %i\n"
					"MoveSpeedBase: %i\n"
					"NpcHullMod: %f\n"
					"DailySpecial: %i\n",
					game.magic.auctionFee,
					game.magic.maxItemRank,
					game.magic.hullRepairSpeed,
					game.magic.hullRepairBase,
					game.magic.hullBase,
					game.magic.moveSpeedBase,
					game.magic.npcHullMod,
					game.magic.DailySpecial);

				break;

			case RON::SHOW_POPUP:

				printf("\nSHOW_POPUP: %s\n\n", buf.read(&success));

				break;
			case RON::UPDATE_PLAYER:
				str = buf.read(&success);
				buf.read(&integer, &success);
				buf.read(&chr, &success);
				ply = game.GetPlayer(integer);
				if (ply)
				{
					ply->name = str;
					ply->accessLevel = chr;
				}
				else
				{
					ply = new Player(str, integer, chr);
					ply->next = game.players;
					game.players = ply;
				}
				break;
			case RON::UPDATE_PLAYER_LOCATION:
				buf.read(&integer, &success);
				buf.read(&chr, &success);
				buf.read(&chr2, &success);
				buf.read(&chr3, &success);
				game.PlayerMove(integer, chr, chr2, chr3);
				break;
			case RON::CHAT_MESSAGE:
			{
				str = buf.read(&success);
				str2 = buf.read(&success);
				str3 = buf.read(&success);
				buf.read(&chr, &success);
				if (chatbot && chr == 1) // CORP
				{
					const char *name = str2;
					const char *message = str;
					if (StrStrI(message, "aug cost") || StrStrI(message, "augging cost") || StrStrI(message, "augcost"))
					{
						int type = -1;
						int rarity = -1;
						int percent = -1;
						char tmp[4];
						auto until = strcspn(message, "123567890");
						const char *numinstring = message + until;
						int i = 0;
						for (i = 0; i < strspn(numinstring, "1234567890"); i++)
							tmp[i] = numinstring[i];
						tmp[i] = 0;
						percent = StrToInt(tmp);
						if (until > 0)
						{
							printf("ayy!!!\n\n\n\n");
							if (StrStrI(message, "weapon"))
								type = 0;
							else if (StrStrI(message, "wep"))
								type = 0;
							else if (StrStrI(message, "armor"))
								type = 1;
							else if (StrStrI(message, "hull"))
								type = 1;
							else if (StrStrI(message, "stor"))
								type = 2;
							else if (StrStrI(message, "harv"))
								type = 3;
							else if (StrStrI(message, "engine"))
								type = 4;
							else if (StrStrI(message, "comp"))
								type = 5;
							else if (StrStrI(message, "spec"))
								type = 6;


							if (StrStrI(message, "uncommon"))
								rarity = 2;
							else if (StrStrI(message, "uc"))
								rarity = 2;
							else if (StrStrI(message, "ultra rare"))
								rarity = 4;
							else if (StrStrI(message, "ur"))
								rarity = 4;
							else if (StrStrI(message, "rare"))
								rarity = 3;
							else if (StrStrI(message, "leg"))
								rarity = 5;
							else if (StrStrI(message, "prec"))
								rarity = 6;

						}
						printf("\n\npercent: %i, type: %i, rarity: %i\n\n", percent, type, rarity);
						if (percent > 0 && percent <= 100 && type != -1 && rarity != -1)
						{
							std::string output("");
							char tmp2[16];
							output.append(_itoa(CalculateAuggingCost(type, rarity, percent), tmp2, 10));
							output += "c!";
							char *tmp = new char[output.length() + 1];
							memcpy(tmp, output.c_str(), output.length() + 1);
							game.writer->write(new RON::ChatMessage(tmp, 1));
						}
					}
				}
				if (strcmp(str2, ""))
				{
					printf("%s", str2);
					if (strcmp(str3, ""))
						printf(" -> %s", str3);

					printf(": ");
				}
				printf("%s\n", str);
				break;
			}
			case RON::SET_HARDCORE:
				buf.read(&integer, &success);
				buf.read(&chr, &success);
				ply = game.GetPlayer(integer);
				if (ply)
				{
					ply->hardcore = (chr != -128);
					if (ply->hardcore)
						printf("Player %s has gone hardcore.\n", ply->name);
					else
						printf("Player %s has gone out of hardcore.\n", ply->name);
				}
				break;
			case RON::UPDATE_SECTOR:
			{
				buf.read(&chr, &success);
				buf.read(&chr2, &success);
				buf.read(&chr3, &success);
				if (chr < 10 && chr >= 0 && chr2 < 40 && chr2 >= 0 && chr3 < 40 && chr2 >= 0)
				{
					sector = &game.sectors[chr][chr2][chr3];
					sector->x = chr2;
					sector->y = chr3;
					sector->universe = chr;
					buf.read(&sector->linkConfig, &success);
					buf.read(&sector->_class, &success);
					buf.read(&sector->faction, &success);
					buf.read(&sector->asteroid, &success);
					buf.read(&sector->hasStarPort, &success);
					if (sector->hasStarPort == -128)
						sector->hasStarPort = 0;
					else
						sector->hasStarPort = 1;
					buf.read(&sector->hasPlanet, &success);
					if (sector->hasPlanet == -128)
						sector->hasPlanet = 0;
					else
						sector->hasPlanet = 1;
					buf.read(&sector->corpID, &success);
				}
				break;
			}
			case RON::UPDATE_ENTITY:
			{
				ent = Entity::instantiate(buf);
				if (ent)
				{
					ent->next = game.currentsector;
					game.currentsector = ent;
				}
				game.Redraw();
				break; // entity entered sector
			}
			case RON::REMOVE_ENTITY:
			{
				RemoveEntity cmd = RemoveEntity(buf);
				Entity *current = game.currentsector;
				Entity *last = 0;
				while (current)
				{
					if (current->id == cmd.id)
					{
						if (last)
							last->next = current->next;
						if (current == game.currentsector)
							game.currentsector = current->next;
						delete current;
						break;
					}
					last = current;
					current = current->next;
				}
				game.Redraw();
				break; // entity left sector
			}
			case RON::UPDATE_PLAYER_STATUS:
				buf.read(&integer, &success);
				ply = game.GetPlayer(integer);
				buf.read(&ply->corpID, &success);
				buf.read(&ply->online, &success);
				if (ply->online == -128)
					ply->online = 0;
				else
					ply->online = 1;
				buf.read(&ply->corpRank, &success);
				buf.read(&ply->hardcore, &success);
				if (ply->hardcore == -128)
					ply->hardcore = 0;
				else
					ply->hardcore = 1;
				break;
			case RON::UPDATE_CORP_TECH:
				buf.read(&shrt, &success);
				corp = game.GetCorp(shrt);
				for (int i = 0; i < 18; i++)
				{
					buf.read(&chr, &success);
					if (chr == -128)
						corp->techs[i] = false;
					else
						corp->techs[i] = true;
				}
				break;
			case RON::UPDATE_CORPORATION:
				str = buf.read(&success);
				buf.read(&shrt, &success); corp = game.corps;
				if (corp)
				{
					while (corp)
					{
						if (corp->id == shrt)
							break;
						if (!corp->next)
							break;
						corp = corp->next;
					}
					if (corp->id != shrt)
					{
						corp->next = new Corporation(str, shrt);
						corp = corp->next;
					}
				}
				else
				{
					game.corps = new Corporation(str, shrt);
					corp = game.corps;
				}
				break;
			case RON::UPDATE_PLAYER_LOAD:
				buf.read(&game.local, &success);
				printf("You are number %i!\n", game.local);
				break;
			case RON::UPDATE_CURRENT_SECTOR:
			{
				printf("UPDATE_CURRENT_SECTOR\n");
				ply = game.GetPlayer(game.local);
				buf.read(&ply->universe, &success);
				buf.read(&ply->x, &success);
				buf.read(&ply->y, &success);

				Sector &sec = game.sectors[ply->universe][ply->x][ply->y];
				sec.universe = ply->universe;
				sec.x = ply->x;
				sec.y = ply->y;

				buf.read(&shrt, &success);
				while (game.currentsector)
				{
					ent = game.currentsector;
					if (!ent->next)
						break;
					game.currentsector = ent->next;
					delete ent;
				}

				while (shrt--)
				{
					ent = Entity::instantiate(buf);
					if (ent)
					{
						ent->next = game.currentsector;
						game.currentsector = ent;
					}
					else
						break; 
				}
				break; 
			}
			case RON::SET_ATTACK:
				buf.read(&game.attack, &success);
				break;
			case RON::UPDATE_SHIP_RESOURCES:
				buf.read(&integer, &success);
				ply = game.GetPlayer(integer);
				if (ply)
				{
					printf("Resources get for %s!:\n", ply->name);
					for (int i = 4; i >= 0; i--)
					{
						chr = ply->resources[i];
						buf.read(&ply->resources[i], &success);
						if (chr != ply->resources[i])
						{
							chr = ply->resources[i] - chr;
							printf("\t%s%i %s\n", chr > 0 ? "+" : "", chr, RON::ResourceNames[i]);
						}
					}
				}
				else
					printf("Unknown player in update ship resources!\n");
				break;
			case RON::GAIN_XP_EVENT:
				buf.read(&integer, &success);
				ply = game.GetPlayer(integer);
				buf.read(&integer, &success);
				if (ply)
					printf("%s: %s%i XP\n", ply->name, integer > 0 ? "+" : "", integer);
				break;
			case RON::ATTACK_EVENT:
				buf.read(&integer, &success);
				buf.read(&integer2, &success);
				buf.read(&integer3, &success);
				buf.read(&chr, &success);

				if (chr <= 3 && chr >= 0 && integer == game.follow && integer != integer2)
				{
					printf("Follow attacked!\n");
					game.attack = integer2;
					//send message now
					game.writer->write(new SetAttack(game.attack));
				}

				break;

			case RON::SET_FOLLOW:
				buf.read(&game.follow, &success);
				ply = game.GetPlayer(game.follow);
				if (ply)
					printf("You are now following %s!\n", ply->name);
				else
					printf("You are not following anyone now!\n");
				break;

			case RON::REQUEST_TRADE:
			{
				RequestTrade *rq = new RequestTrade(buf);
				if (rq->bId == game.follow || rq->aId)
				{
					rq->final = true;
					game.writer->write(rq);
				}
				break;
			}

			case RON::UPDATE_PLAYER_CREDITS:
			{
				buf.read(&game.credits, &success);
				break;
			}
			case RON::UPDATE_PLAYER_BANK:
			{
				printf("Bank update!:\n");
				char amt;
				buf.read(&amt, &success);
				EquipmentItem *cur = game.bank;
				while (cur)
				{
					EquipmentItem *item = cur;
					cur = item->next;
					delete item;
				}
				game.bank = 0;
				while (amt--)
				{
					EquipmentItem *item = EquipmentItem::instantiate(buf);
					if (item)
					{
						printf("\ttype %i rarity %i\n", (int)item->type, (int)item->rarity);
						item->next = game.bank;
						game.bank = item;
					}
				}
				break;
			}

			case 0:
			case RON::UPDATE_SPACEBALL_BETS:
			case RON::UPDATE_SPACEBALL_TIME:
			case RON::UPDATE_PLAYER_COOLDOWNS:
			case RON::ALIVE:
			case RON::AH_ITEM:
			case RON::AH_ITEM_REMOVE:
				break;
			default:

				printf("Processing command with type %i failed! (unknown)\n", chr);

				break;
			}
		}

		delete command;
	}
}

void CommandReader::close(void)
{
	printf("CLOSED!!!!\n");
	closesocket(socket);
	running = false;
	RawCommand *command = commands;
	while (command)
	{
		RawCommand *current = command;
		command = command->next;
		delete current;
	}
	commands = 0;
}