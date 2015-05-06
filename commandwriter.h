#ifndef COMMANDWRITER_H
#define COMMANDWRITER_H
#include "commands.h"
#include <WinSock2.h>

namespace RON
{
	class CommandWriter
	{
	public:
		CommandWriter(SOCKET s)
		{
			socket = s;
		}

		void write(RawCommand *command)
		{
			CharBuf sending = command->write();
			union
			{
				short length;
				char clength[2];
			};
			length = short(sending.len + sizeof(command->getType()));
			auto type = command->getType();
			char len[2];
			len[0] = clength[1];
			len[1] = clength[0];

			send(socket, (char *)&len, sizeof(length), 0);
			send(socket, (char *)&type, sizeof(type), 0);
			if (length > sizeof(type))
				send(socket, sending.buf, sending.len, 0);

		//	printf("Wrote %i bytes for type %i\n", length, type);

			delete command;
		}

	public:
		SOCKET socket;
	};
}

#endif