#ifndef COMMANDREADER_H
#define COMMANDREADER_H
#include "commands.h"
#include <WinSock2.h>

namespace RON
{
	typedef unsigned int time;
	class CommandReader
	{
	public:
		CommandReader(SOCKET socket)
		{
			this->socket = socket;
		}
		~CommandReader()
		{

		}

		RawCommand *getNextCommand(void)
		{
			RawCommand *ret = commands;
			if (ret)
				commands = ret->next;
			return ret;
		}

		bool isInactive(RON::time time)
		{
			return false;
		}

		void run(void);
		bool isconnected(void);
		void close(void);
		void process(void);
		bool isConnected(void)
		{
			return true;
		}

	public:

		RON::RawCommand *commands;
		bool running;
		unsigned short numcommands;
		SOCKET socket;
		RON::time lastTime;
	};
};

#endif