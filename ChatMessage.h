#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H
#include "commands.h"

namespace RON
{
	class ChatMessage : public RawCommand
	{
	public:
		ChatMessage(const char *Player, const char *Msg)
		{
			message = Msg;
			receiver = Player;
			sound = "";
			channel = 3;
		}
		ChatMessage(const char *Msg, char Chan)
		{
			message = Msg;
			channel = Chan;
			sound = "";
			receiver = "";
		}


		CommandType getType()
		{
			return RON::CHAT_MESSAGE;
		}

		CharBuf write(void)
		{
			CharBuf ret = CharBuf();
			ret.append(message);
			ret.append(sound);
			ret.append(receiver);
			ret.append(channel);
			return ret;
		}

	public:
		char channel;
		const char *message;
		const char *sound;
		const char *receiver; // for pms
	};
}
#endif