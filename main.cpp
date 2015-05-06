#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "awesomium.lib")
#include "RequestNewAccount.h"
#include "game.h"
#include "errors.h"
#include <iostream>
#include "commandreader.h"
#include "ChatMessage.h"
#include "RequestJump.h"
#include "RequestCorpAcceptInvite.h"
#include "RequestHardcore.h"
#include "RequestResourceTransfer.h"
#include "commandwriter.h"
#include "RequestCorpAction.h"
#include "Alive.h"
#include "RequestTrade.h"
#include "RequestItemJettison.h"
#include "SetFollow.h"
#include "RequestPlayer.h"
#include "MainWindow.h"
#include "TutorialApp.h"
#include "RequestItemUnequip.h"
#include "RequestItemEquip.h"
#include "RequestItemEngineer.h"
#include "application.h"
#include <ctime>
#include <windows.h>
#include <string>
#include <iostream>
#define strcmpi _strcmpi

using namespace Awesomium;


const char *rarities[] = {
	"Unknown!",
	"Common",
	"Uncommon",
	"Rare",
	"Ultra-Rare",
	"Legendary",
	"Precursor",
	"Ultimate"
};



GAME game = GAME();
bool chatbot = false;

RON::Error CODE(RON::SUCCESS);
RON::Error MainThread(void)
{
	for (int i = 0; i < 10; i++)
		game.reader->run();
	game.reader->process();
	return RON::Error(RON::SUCCESS);
}

#define GAME_PORT  31040
#define GAME_PORTS "31040"
#define GAME_ADDR  "66.119.27.230"

bool Connect()
{
	//----------------------
	// Declare and initialize variables.
	WSADATA wsaData;
	int iResult;

	SOCKET ConnectSocket = INVALID_SOCKET;
	struct sockaddr_in clientService;

	//----------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return false;
	}

	//----------------------
	// Create a SOCKET for connecting to server
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return false;
	}

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(GAME_ADDR);
	clientService.sin_port = htons(GAME_PORT);
	//----------------------

	// Connect to server.
	iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		printf("Unable to connect to server: %ld\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	game.socket = ConnectSocket;
	return true;
}

clock_t lastTime = 0;
bool wasDoneLast = false;

DWORD WINAPI ReadCommands(LPVOID lpParam)
{
	char command[512];
	char argumentstr[512];
	while (1)
	{
		ZeroMemory(command, 512);
		ZeroMemory(argumentstr, 512);
		scanf("%511s%511[^\n]", command, argumentstr);
		char *arguments = argumentstr;
		arguments++;
		if (!strcmpi(command, "jump"))
		{
			if (!strcmpi(arguments, "up"))
			{
				game.writer->write(new RON::RequestJump(3));
			}
			else if (!strcmpi(arguments, "down"))
			{
				game.writer->write(new RON::RequestJump(4));
			}
			else if (!strcmpi(arguments, "corp"))
			{
				game.writer->write(new RON::RequestJump(2));
			}
			else if (!strcmpi(arguments, "earth"))
			{
				game.writer->write(new RON::RequestJump(1));
			}
		}
		else if (!strcmpi(command, "togglehc"))
		{
			game.writer->write(new RON::RequestHardcore());
		}
		else if (!strcmpi(command, "giveres"))
		{
			RON::Player *ply = game.GetPlayer(arguments);
			if (ply)
				game.writer->write(new RON::RequestResourceTransfer(127, 127, 127, 127, 127, ply->id));
			else
				printf("Player not found!\n");
		}
		else if (!strcmpi(command, "sector"))
		{
			if (!strcmpi(arguments, "here"))
			{
				RON::Player *ply = game.GetPlayer(game.local);
				printf("\nYou are in Sector %i / %i / %i.\n", ply->universe + 1, ply->x + 1, ply->y + 1);
			}
			else
			{
				int u, x, y;
				int num = sscanf(arguments, "%i %i %i", &u, &x, &y);
				if (num == 3)
				{
					u = u - 1;
					x = x - 1;
					y = y - 1;
					RON::Sector &sector = game.sectors[u][x][y];
					if (sector.hasPlanet)
					{
						RON::Corporation *corp = game.GetCorp(sector.corpID);
						if (corp)
						{
							printf("Planet owned by %s\n", corp->name);
						}
						else
							printf("Earth\n");
					}
					if (sector.hasStarPort)
					{
						printf("Starport avaiable!\n");
					}
					if (sector.asteroid)
					{
						printf("Has asteroid\n");
					}
					printf(
						"Faction: %i - ?\n",
						"Class: %i - ?\n",
						sector.faction,
						sector._class
						);

					if (sector.isLinked(RON::NORTHWEST))
						printf("\\");
					else
						printf(" ");

					if (sector.isLinked(RON::NORTH))
						printf("|");
					else
						printf(" ");

					if (sector.isLinked(RON::NORTHEAST))
						printf("/\n");
					else
						printf(" \n");

					if (sector.isLinked(RON::WEST))
						printf("-");
					else
						printf(" ");

					printf("x");

					if (sector.isLinked(RON::EAST))
						printf("-\n");
					else
						printf(" \n");

					if (sector.isLinked(RON::SOUTHWEST))
						printf("/");
					else
						printf(" ");

					if (sector.isLinked(RON::SOUTH))
						printf("|");
					else
						printf(" ");

					if (sector.isLinked(RON::SOUTHEAST))
						printf("\\\n");
					else
						printf(" \n");

				}
				else
				{
					printf("Usage: sector [here | universe x y]\n");
				}
			}
		}
		else if (!strcmpi("corpleave", command))
		{
			if (!strcmpi("veryverysure", arguments))
			{
				game.writer->write(new RON::RequestCorpAction(9, "Okay"));
			}
			else printf("are you sure? type leave veryverysure\n");
		}
		else if (!strcmpi("corpaccept", command))
		{
			game.writer->write(new RON::RequestCorpAcceptInvite());
		}
		else if (!strcmpi("unequip", command))
		{
			RON::Ship *me = game.GetShipInCurrentSector(game.local);
			if (!strcmpi("weapon", arguments))
			{
				if (me->weapon != 0)
				{
					game.writer->write(new RON::RequestItemUnequip(me->weapon));
				}
			}
			else if (!strcmpi("armor", arguments))
			{
				if (me->armor != 0)
				{
					game.writer->write(new RON::RequestItemUnequip(me->armor));
				}
			}
			else if (!strcmpi("storage", arguments))
			{
				if (me->storage != 0)
				{
					game.writer->write(new RON::RequestItemUnequip(me->storage));
				}
			}
			else if (!strcmpi("special", arguments))
			{
				if (me->special != 0)
				{
					game.writer->write(new RON::RequestItemUnequip(me->special));
				}
			}
			else if (!strcmpi("computer", arguments))
			{
				if (me->computer != 0)
				{
					game.writer->write(new RON::RequestItemUnequip(me->computer));
				}
			}
			else if (!strcmpi("harvester", arguments))
			{
				if (me->harvester != 0)
				{
					game.writer->write(new RON::RequestItemUnequip(me->harvester));
				}
			}
			else if (!strcmpi("engine", arguments))
			{
				if (me->engine != 0)
				{
					game.writer->write(new RON::RequestItemUnequip(me->engine));
				}
			}
			else printf("Unknown item!\n");
		}
		else if (!strcmpi("equip", command))
		{
			int itemId = 0;
			int amt = sscanf(arguments, "%i", &itemId);
			if (amt == 1)
			{
				RON::EquipmentItem *item = game.bank;
				while (item)
				{
					itemId--;
					if (itemId == 0)
						break;
					item = item->next;
				}
				if (item != 0)
				{
					game.writer->write(new RON::RequestItemEquip(item));
				}
			}
		}
		else if (!strcmpi("togglechatbot", command))
			chatbot = !chatbot;
		else if (!strcmpi("engineer", command))
		{
			int itemId = 0;
			char tmp[512];
			int amt = sscanf(arguments, "%s %i", tmp, &itemId);
			if (amt == 2)
			{
				RON::EquipmentItem *item = game.bank;
				while (item)
				{
					itemId--;
					if (itemId == 0)
						break;
					item = item->next;
				}
				if (item != 0)
				{
					if (!strcmpi(tmp, "rankup"))
					{
						game.writer->write(new RON::RequestItemEngineer(item, 2));
					}
					else if (!strcmpi(tmp, "rankdown"))
					{
						game.writer->write(new RON::RequestItemEngineer(item, 3));
					}
					else if (!strcmpi(tmp, "rarity"))
					{
						game.writer->write(new RON::RequestItemEngineer(item, 6));
					}
					else if (!strcmpi(tmp, "repair"))
					{
						game.writer->write(new RON::RequestItemEngineer(item, 9));
					}
				}
			}

		}
		else if (!strcmpi("give", command))
		{
			RON::Player *ply = game.GetPlayer(game.follow);
			if (ply)
			{
				int itemId, money;
				int num = sscanf(arguments, "%i %i", &itemId, &money);
				if (num == 1)
				{
					money = 0;
					num = 2;
				}

				if (num == 2)
				{
					RON::RequestTrade *rq = new RON::RequestTrade();
					rq->aId = game.local;
					rq->bId = game.follow;
					rq->aCredits = money;
					rq->bCredits = 0;
					rq->final = false;
					RON::EquipmentItem *item = game.bank;
					if (itemId <= 0)
						item = 0;
					else
						while (item)
						{
							itemId--;
							if (itemId == 0)
								break;
							item = item->next;
						}
					if (itemId <= 0)
					{
						rq->aItem = item;
						rq->bItem = 0;
						game.writer->write(rq);
					}
					else
						printf("unknown item!");
				}
				else
					printf("Incorrect number of arguments! (needs 1 or 2)\n");
			}
			else
				printf("Unknown player!\n");
		}
		else if (!strcmpi(command, "follow"))
		{
			RON::Player *ply = game.GetPlayer(arguments);
			if (ply)
			{
				game.follow = ply->id;
				game.writer->write(new RON::SetFollow(game.follow));
			}
			else
				printf("Player not found!\n");
		}
		else if (!strcmpi(command, "jettison"))
		{
			RON::EquipmentItem *item = game.bank;
			int itemId;
			int num = sscanf(arguments, "%i", &itemId);
			if (num == 1)
			{
				while (item)
				{
					itemId--;
					if (itemId == 0)
						break;
					item = item->next;
				}
				if (item)
					game.writer->write(new RON::RequestItemJettison(item));
				else
					printf("Failed (2)\n");
			}
			else
				printf("Failed (1)\n");
		}
		else if (!strcmpi(command, "listequipment"))
		{
			auto ply = game.GetShipInCurrentSector(game.local);
			if (ply)
			{
				auto weapon = ply->weapon;
				if (weapon)
				{
					printf("Weapon\n"
						"\t%s %s (%i%%)\n",
						rarities[weapon->rarity], weapon->getName(), weapon->durability);
				}
				auto armor = ply->armor;
				if (armor)
				{
					printf("Armor\n"
						"\t%s %s (%i%%)\n",
						rarities[armor->rarity], armor->getName(), armor->durability);
				}
				auto storage = ply->storage;
				if (storage)
				{
					printf("Storage\n"
						"\t%s %s (%i%%)\n",
						rarities[storage->rarity], storage->getName(), storage->durability);
				}
				auto engine = ply->engine;
				if (engine)
				{
					printf("Engine\n"
						"\t%s %s (%i%%)\n",
						rarities[engine->rarity], engine->getName(), engine->durability);
				}
				auto computer = ply->computer;
				if (computer)
				{
					printf("Computer\n"
						"\t%s %s (%i%%)\n",
						rarities[computer->rarity], computer->getName(), computer->durability);
				}
				auto harvester = ply->harvester;
				if (harvester)
				{
					printf("Harvester\n"
						"\t%s %s (%i%%)\n",
						rarities[harvester->rarity], harvester->getName(), harvester->durability);
				}
				auto special = ply->special;
				if (special)
				{
					printf("Computer\n"
						"\t%s %s (%i%%)\n",
						rarities[special->rarity], special->getName(), special->durability);
				}
			}
			else printf("Unable to find player!\n");
		}
		else if (!strcmpi(command, "listitems"))
		{
			int i = 1;
			RON::EquipmentItem *item = game.bank;
			printf("\n\n");
			while (item)
			{
				printf("%02i | %s %s %s\n"
				"\tDurability: %i%%\n",
				i, rarities[item->rarity], item->getName(), item->noDrop ? "(Bound)" : "",
				item->durability);
				i++;
				item = item->next;
			}
		}
		else
			printf("Unknown command! (%s)\n", command);
	}
}
using namespace Awesomium;
TutorialApp *app = 0;

void OnSectorUpdated(RON::Sector *sector)
{
	if (!sector)
		return;
	JSValue &gm = app->view_->web_view()->ExecuteJavascriptWithResult(WSLit("game;"), WSLit("C"));
	if (!gm.IsObject())
		return;
	JSValue &vsectors = ((JSObject &)gm).GetProperty(WSLit("sectors"));
	if (!vsectors.IsArray())
		return;
	JSArray &sectors = (JSArray &)vsectors;
	JSValue &val2 = sectors.At(sector->universe);
	if (val2.IsArray())
	{
		JSArray &val22 = (JSArray &)val2;
		JSValue &val = val22.At(sector->x);
		if (val.IsArray())
		{
			JSValue &jssector = ((JSArray &)val).At(sector->y);
			if (jssector.IsObject())
			{
				JSObject &ret = (JSObject &)jssector; // i made this named ret because i was too lazy to change all the other things from previous functions
				static WebString universe = WSLit("universe");
				static WebString x = WSLit("x");
				static WebString y = WSLit("y");
				static WebString linkConfig = WSLit("linkConfig");
				static WebString hasStarport = WSLit("hasStarport");
				static WebString hasPlanet = WSLit("hasPlanet");
				static WebString corpID = WSLit("corpID");
				static WebString faction = WSLit("faction");
				static WebString asteroid = WSLit("asteroid");
				static WebString _class = WSLit("class");
				ret.SetProperty(universe, JSValue(sector->universe));
				ret.SetProperty(x, JSValue(sector->x));
				ret.SetProperty(y, JSValue(sector->y));
				ret.SetProperty(linkConfig, JSValue(sector->linkConfig));
				ret.SetProperty(hasStarport, JSValue(sector->hasStarPort == 1));
				ret.SetProperty(hasPlanet, JSValue(sector->hasPlanet == 1));
				ret.SetProperty(corpID, JSValue(sector->corpID));
				ret.SetProperty(faction, JSValue(sector->faction));
				ret.SetProperty(asteroid, JSValue(sector->asteroid));
				ret.SetProperty(_class, JSValue(sector->_class));
			}
		}
	}
}

DWORD WINAPI RunWin(LPVOID)
{
	app->Run();
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc < 3)
		exit(0);
	game.name = argv[1];
	game.email = argv[2];
	game.deviceid = argv[3];
	game.logintype = (argc > 4 ? 1 : 0);
	SetConsoleTitle(game.name);
	//app = new TutorialApp();
	//CreateThread(0, 0, &RunWin, 0, 0, 0);

	printf("Loaded!!!!!!!\n");
	if (Connect())
	{
		CreateThread(0, 0, &ReadCommands, 0, 0, 0);
		game.reader = new RON::CommandReader(game.socket);
		game.writer = new RON::CommandWriter(game.socket);
		while (CODE == 0)
		{
			CODE = MainThread();
			Sleep(20);
			clock_t time = clock() - lastTime;
			if ((time / CLOCKS_PER_SEC) > 5)
			{
				lastTime = clock();
				game.writer->write(new RON::Alive());
			}

		}
	}
	else
		printf("Connect failed!\n");
	printf("Error! \n\tCode: \n\tMessage: %s\n\n", CODE.code, CODE.message);
	return 0;
};