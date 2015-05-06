#include "entity.h"
#include <Windows.h>

RON::Entity *RON::Entity::instantiate(CharBuf &buf)
{
	bool success;
	char type;
	buf.read(&type, &success);

	if (type == TYPE_SHIP)
	{
		Ship *ship = new Ship;
		ship->type = type;
		Entity::readentity(ship, buf);
		ship->read(buf);
		return ship;
	}
	else if (type == TYPE_NPC)
	{
		NPC *npc = new NPC;
		npc->type = type;
		Entity::readentity(npc, buf);
		npc->read(buf);
		return npc;
	}
	else if (type == TYPE_STARPORT)
	{
		StarPort *sp = new StarPort;
		sp->type = type;
		Entity::readentity(sp, buf);
		sp->read(buf);
		return sp;
	}
	return 0;
}