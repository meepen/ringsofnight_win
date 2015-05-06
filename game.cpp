#include "game.h"
#include "ChatMessage.h"
#include "RequestLogin.h"
#include "SetFollow.h"
#include "commandwriter.h"
#include "SetAttack.h"
#include "RequestNewAccount.h"
#include <fstream>
#include <ctime>

RON::RequestAccountActionBundle *encrypt(const char *str)
{
	RON::RequestAccountActionBundle *ret = new RON::RequestAccountActionBundle;
	FILE *f;
	fopen_s(&f, "input.hex", "wb");
	fwrite(str, sizeof(char), strlen(str), f);
	fclose(f);
	system("java Main");
	fopen_s(&f, "output.hex", "rb");
	fseek(f, 0, SEEK_END);
	ret->len = (short)ftell(f);
	printf("Len: %i", ret->len);
	fseek(f, 0, SEEK_SET);
	ret->hex = new char[ret->len];
	fread(ret->hex, 1, ret->len, f);
	fclose(f);
	return ret;
}

void GAME::SendLogon(void)
{
	const char *serial = "0000-0000-0000-0001";
	const char *info = "MICROSOFT WINDOWS 7 USER PRODUCT";
	const char *version = "1.1.04-K";
	/*

	const char *serial = "aaaaaaaaa";
	const char *info = "aaaaaaaaa aaaaaaaaa aaaaaaaaa aaaaaaaaa aaaaaaaaa";
	const char *version = "1.1.04-K";
	*/
	RON::RequestAccountActionBundle *bundle[5];
	bundle[0] = encrypt(email);
	bundle[1] = encrypt(deviceid);
	bundle[2] = encrypt(serial);
	bundle[3] = encrypt(info);
	bundle[4] = encrypt(version);

	RON::RequestAccountAction *send = 0;
	if (logintype == 0)
		send = new RON::RequestLogin(name);
	else
		send = new RON::RequestNewAccount(name);
	send->bundle = bundle;
	send->numbundle = 5;

	game.writer->write(send);

}

RON::Player *GAME::GetPlayer(int id)
{
	RON::Player *ply = players;
	if (ply)
		while (ply)
		{
			if (ply->id == id)
				break;
			ply = ply->next;
		}
	return ply;
}

RON::Player *eGetPlayer(int id)
{
	return game.GetPlayer(id);
}

void GAME::Redraw(void)
{
//	RECT rect;
//	GetClientRect(GetActiveWindow(), &rect);
//	InvalidateRect(GetActiveWindow(), &rect, TRUE);
}

RON::Player *GAME::GetPlayer(const char *name)
{
	RON::Player *ply = players;
	if (ply)
		while (ply)
		{
			if (!_strcmpi(name, ply->name))
				break;
			ply = ply->next;
		}
	return ply;
}

RON::Corporation *GAME::GetCorp(short id)
{
	RON::Corporation *ply = corps;
	if (ply)
		while (ply)
		{
			if (ply->id == id)
				break;
			ply = ply->next;
		}
	return ply;
}

void GAME::Tick(void)
{
	clock_t time = clock();
	static clock_t lastAttack = 0;
	RON::Ship *ship = game.GetShipInCurrentSector(game.local);
	if (!ship)
	{
		printf("Ship not found!!!!!\n");
		return;
	}
	if (ship->weapon && ((time - lastAttack) / CLOCKS_PER_SEC * 1000) >= ship->weapon->getShootSpeedMS() - 100)
	{
		game.writer->write(new RON::SetAttack(game.attack));
		lastAttack = time;
	}
	else
		printf("Weapon: %p\tshootspeedms: %i\n", ship->weapon, ship->weapon ? ship->weapon->getShootSpeedMS() : -1);
}

extern void OnSectorUpdated(RON::Sector *sector);

void GAME::UpdateSector(char u, char x, char y)
{
	OnSectorUpdated(&game.sectors[u][x][y]);
}

void GAME::PlayerMove(int playerId, int uni, int x, int y)
{
	RON::Player *ply = GetPlayer(playerId);
	if (!ply)
	{
		printf("Received PlayerMove command for %i but it didn't exist!\n", playerId);
		return;
	}
	ply->x = x;
	ply->universe = uni;
	ply->y = y;
	/*
	if (!strcmp(ply->name, "MeepDM"))
	{
		RON::Player *localplayer = GetPlayer(local);

		if (localplayer->universe == ply->universe && localplayer->x == ply->x && localplayer->y == ply->y)
		{
			printf("Following!\n");
			writer->write(new RON::SetFollow(playerId));
		}
	}
	*/
	/*
	if (uni == 0 && x == 19 && y == 19)
	{
		this->writer->write(new RON::ChatMessage(ply->name, "Welcome to Earth!"));
	}
	*/
}