#ifndef ENTITY_H
#define ENTITY_H
#include "sector.h"
#include "commands.h"
#include "EquipmentItem.h"
#include "player.h"
extern RON::Player *eGetPlayer(int id);

namespace RON
{
	enum ENTITY_TYPES
	{
		TYPE_SHIP = 1,
		TYPE_NPC,
		TYPE_GARRISON,
		TYPE_PLANET,
		TYPE_STARPORT,
		TYPE_CARGO_MONEY,
		TYPE_CARGO_ITEM,
		TYPE_CARGO_RESOURCE,
		TYPE_ASTEROID,
		TYPE_TECH,
	};
	class Ship;
	class NPC;
	class StarPort;
	class Entity
	{
	private:
		static bool readentity(Entity *ent, CharBuf &buf)
		{
			bool success;
			buf.read(&ent->id, &success);
			return success;
		}
	public:

		static Entity *instantiate(CharBuf &buf);

	public:
		char x;
		char y;
		char universe;

		char relation;
		char type;
		int id;
		Entity *next;
	};

	class Ship : public Entity
	{
	public:

		bool read(CharBuf &buf)
		{
			bool success;

			buf.read(&hull, &success);
			buf.read(&corpId, &success);

			buf.read(&_class, &success);
			RON::Player *ply = eGetPlayer(id);
			for (int i = 4; i >= 0; i--)
			{
				char chr;
				if (ply)
					buf.read(&ply->resources[i], &success);
				else
					buf.read(&chr, &success);
			}
			
			buf.read(&reborn, &success);
			buf.read(&xp, &success);
			buf.read(&hardcore, &success);
			if (hardcore == -128)
				hardcore = 0;
			else
				hardcore = 1;
			weapon = (WeaponItem *)EquipmentItem::instantiate(buf);
			armor = (ArmorItem *)EquipmentItem::instantiate(buf);
			storage = (StorageItem *)EquipmentItem::instantiate(buf);
			harvester = (HarvesterItem *)EquipmentItem::instantiate(buf);
			engine = (EngineItem *)EquipmentItem::instantiate(buf);
			computer = (ComputerItem *)EquipmentItem::instantiate(buf);
			special = (SpecialItem *)EquipmentItem::instantiate(buf);
			return success;
		}

	public:
		int xp;
		char resources[5];
		char reborn; // transcend
		char _class;
		char hardcore;
		int hull;
		short corpId;

		WeaponItem *weapon;
		ArmorItem *armor;
		StorageItem *storage;
		ComputerItem *computer;
		HarvesterItem *harvester;
		SpecialItem *special;
		EngineItem *engine;
	};

	enum NPC_CLASS
	{
		HETEROCLITE = 0,
		PIRATE,
		WYRD,
	};

	class NPC : public Entity
	{
	public:

		void read(CharBuf &buf)
		{
			bool success;
			buf.read(&_class, &success);
			buf.read(&rank, &success);
			
		}

	public:
		char _class;
		char faction;
		char rank;
		char stuckCount;

	};

	class Garrison : public Entity
	{
	public:

	public:
		char tech;
		long long nextAttackTimesMS[20];

	};

	class Planet : public Entity
	{
	public:

	public:
		short corpId;
		void *item;
		long long research;
	};

	class StarPort : public Entity
	{
	public:
		void read(CharBuf &buf)
		{
			bool success;
			for (int i = 4; i >= 0; i--)
				buf.read(&modifiers[i], &success);
		}
	public:
		float modifiers[5];
	};
}


#endif