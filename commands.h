#ifndef COMMANDS_H
#define COMMANDS_H
#include <stdlib.h>
#include <string.h>
#include <fstream>

namespace RON
{
	typedef char CommandType;
	/* be sure to cast to char! (java byte)*/
	enum CommandNumbers
	{
		SET_MAGIC_VALUES = -127,
		HANDSHAKE = -126,
		ALIVE = -125,
		SHOW_POPUP = -123,
		SHOW_REPORT = -122,
		UPDATE_PLAYER = -118,
		UPDATE_CORPORATION = -117,
		UPDATE_CURRENT_SECTOR = -113,
		UPDATE_SPACEBALL_BETS = -110,
		UPDATE_SPACEBALL_TIME = -109,
		UPDATE_PLAYER_LOAD = -108,
		UPDATE_PLAYER_COOLDOWNS = -107,
		UPDATE_PLAYER_BUYABLES = -106,
		UPDATE_PLAYER_CREDITS = -105,
		UPDATE_PLAYER_LOCATION = -104,
		UPDATE_PLAYER_STATUS = -103,
		UPDATE_PLAYER_BANK = -102,
		UPDATE_SHIP_RESOURCES = -101,
		UPDATE_STARPORT_PRICE_MOD = -99,
		UPDATE_SECTOR = -98,
		UPDATE_CORP_TECH = -96,
		EVENT_CORP_INVITE = -88,
		REMOVE_ENTITY = -86,
		UPDATE_ENTITY = -85,
		HARVEST_EVENT = -84,
		ATTACK_EVENT = -83,
		GAIN_XP_EVENT = -78,
		CHANGE_MONEY_EVENT = -77,
		GAIN_ITEM_EVENT = -75,
		REQUEST_NEW_ACCOUNT = -74,
		REQUEST_LOGIN = -73,
		CHAT_MESSAGE = -72,
		REQUEST_PLAYER = -71,
		REQUEST_CORP_ROSTER = -70,
		SET_ATTACK = -68,
		SET_FOLLOW = -67,
		SET_REPAIR = -66,
		SET_HARDCORE = -65,
		REQUEST_HARDCORE = -60,
		REQUEST_JUMP = -59,
		REQUEST_MOVE = -58,
		REQUEST_HARVEST = -57,
		REQUEST_LOOT = -56,
		REQUEST_CAPTURE = -55,
		REQUEST_UPGRADE_SHIP = -54,
		REQUEST_TRADE = -52,
		REQUEST_CANCEL_TRADE = -51,
		REQUEST_BUYABLE_ITEM = -48,
		REQUEST_CORP_ACTION = -47,
		REQUEST_LEADERBOARD = -46,
		REQUEST_REPORT = -45,
		REQUEST_ITEM_REPORT = -44,
		REQUEST_CORP_ACCEPT_INVITE = -38,
		REQUEST_CORP_REJECT_INVITE = -37,
		REQUEST_SET_TECHNOLOGY = -34,
		REQUEST_RESOURCES_RESEARCH = 3,
		REQUEST_RESOURCE_JETTISON = 12,
		REQUEST_RESOURCE_SELL = 14,
		REQUEST_RESOURCE_TRANSFER = 16,
		REQUEST_ITEM_JETTISON = 22,
		REQUEST_ITEM_EQUIP = 23,
		REQUEST_ITEM_UNEQUIP = 24,
		REQUEST_EQUIP_GARRISON = 27,
		REQUST_UNEQUIP_GARRISON = 28,
		REQUEST_ITEM_SCRAP = 30,
		REQUEST_ITEM_ENGINEER = 31,
		AH_ITEM = 42,
		AH_ITEM_REMOVE = 43,
		AH_ITEM_REQUEST_PAGE = 44,
		AH_ITEM_REQUEST_BUYOUT = 45,
		AH_ITEM_REQUEST_BID = 46,
		AH_ITEM_REQUEST_SELL = 47,
		AH_ITEM_REQUEST_REMOVE = 48,
		AH_ITEM_REQUEST_ITEM = 49,
		REQUEST_SPACEBALL_BET = 62,
		REQUEST_SPACEBALL_CLEAR = 63,
	};

	class RawCommand;

	typedef void(__cdecl *RawCommandWriteFn)(RawCommand *command);

	struct CharBuf
	{
		CharBuf()
		{
			buf = 0;
			len = 0;
			shouldfree = true;
		}
		CharBuf(const char *buffer, unsigned int length)
		{
			len = length;
			amtleft = len;
			buf = (char *)buffer;
			current = buf;
			shouldfree = false; // let the user delete the supplied buffer
		}
		~CharBuf()
		{
			if (buf && shouldfree)
				::free(buf);
		}
		void read(short *out, bool *success)
		{
			if (sizeof(short) > amtleft)
			{
				*out = 0;
				*success = false;
				return;
			}
			union
			{
				char chrs[sizeof(*out)];
				short shrt;
			};
			chrs[1] = *current++;
			chrs[0] = *current++;
			*out = shrt;
			amtleft -= sizeof(*out);
			*success = true;
		}
		void read(float *out, bool *success)
		{
			if (sizeof(*out) > amtleft)
			{
				*out = 0;
				*success = false;
				return;
			}
			union
			{
				char chrs[sizeof(*out)];
				float uout;
			};
			char temp[sizeof(*out)];
			for (int i = 0; i < sizeof(*out); i++)
				temp[i] = *current++;
			for (int i = 0; i < sizeof(*out); i++)
				chrs[3 - i] = temp[i];
			*out = uout;
			amtleft -= sizeof(*out);
			*success = true;
		}
		void read(int *out, bool *success)
		{
			if (sizeof(*out) > amtleft)
			{
				*out = 0;
				*success = false;
				return;
			}
			union
			{
				char chrs[sizeof(*out)];
				int uout;
			};
			char temp[sizeof(*out)];
			for (int i = 0; i < sizeof(*out); i++)
				temp[i] = *current++;
			for (int i = 0; i < sizeof(*out); i++)
				chrs[3 - i] = temp[i];
			*out = uout;
			amtleft -= sizeof(*out);
			*success = true;
		}
		void read(char *out, bool *success)
		{
			if (amtleft < sizeof(char))
			{
				*success = false;
				return;
			}
			*out = *current++;
			amtleft -= sizeof(*out);
			*success = true;
		}
		void read(char *out, unsigned int size, bool *success)
		{
			if (size > amtleft)
			{
				*success = false;
				return;
			}
			for (unsigned int i = 0; i < size; i++)
			{
				*out++ = *current++;
			}
			amtleft -= size;
			*success = true;
		}
		__forceinline char *read(bool *success)
		{
			char size;
			read(&size, success);
			if ((unsigned int)size > amtleft)
			{
				*success = false;
				return 0;
			}
			char *ret = new char[size + 1];
			char *retn = ret;
			for (short i = 0; i < size; i++)
				*ret++ = *current++;
			*ret++ = 0;
			amtleft -= size;
			*success = true;
			return retn;
		}
		void append(const char *stuff, unsigned int applen)
		{
			if (!buf)
				buf = (char *)malloc(applen);
			else
				buf = (char *)realloc(buf, len + applen);
			char *end = buf + len;
			for (unsigned int i = 0; i < applen; i++)
				*end++ = *stuff++;
			len += applen;
		}
		void append(const char *string)
		{
			auto real_len = strlen(string);
			if (real_len > 120)
				real_len = 120;
			append((char)real_len);
			append(string, strlen(string));
		}
		void append(char stuff)
		{
			append(&stuff, 1);
		}
		void append(int stuff)
		{
			union
			{
				char chrs[sizeof(stuff)];
				int shrt;
			};
			shrt = stuff;
			char tmp = chrs[0];
			char tmp2 = chrs[1];
			chrs[0] = chrs[3];
			chrs[1] = chrs[2];
			chrs[2] = tmp2;
			chrs[3] = tmp;
			append(chrs, sizeof(stuff));
		}
		void append(short stuff)
		{
			union
			{
				char chrs[sizeof(stuff)];
				short shrt;
			};
			shrt = stuff;
			char tmp = chrs[0];
			chrs[0] = chrs[1];
			chrs[1] = tmp;
			append(chrs, sizeof(stuff));
		}
		void free(void)
		{
			if (buf)
				::free(buf);
		}
		char *buf;
		char *current; // for reading
		unsigned int len;
		unsigned int amtleft;
		bool shouldfree = true;
	};

	class RawCommand
	{
	public:
		RawCommand(void) { }
		RawCommand(char *buf, unsigned int size, CommandType type = 0)
		{
			buffer = buf;
			this->size = size;
			if (type == 0)
			{
				this->type = *(CommandType *)buf;
			}
			else
				this->type = type;
		}
		~RawCommand()
		{
			delete[] buffer;
		}

		virtual CharBuf write(void)
		{
			return CharBuf();
		}

		virtual CommandType getType(void)
		{
			return 0;
		}


	public:
		char *buffer = 0; // for input
		unsigned int size = 0;

		CommandType type = 0;
		RON::RawCommand *next = 0;
	};

};

#endif