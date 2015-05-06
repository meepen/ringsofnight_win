#ifndef SETFOLLOW_H
#define SETFOLLOW_H

namespace RON
{
	class SetFollow : public RawCommand
	{
	public:

		CommandType getType(void)
		{
			return RON::SET_FOLLOW;
		}
		SetFollow(int Id)
		{
			id = Id;
		}

		CharBuf write(void)
		{
			CharBuf ret = CharBuf();
			ret.append(id);
			return ret;
		}

	public:
		int id;
	};
}

#endif