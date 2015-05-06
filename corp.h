#ifndef CORP_H
#define CORP_H

namespace RON
{
	class Corporation
	{
	public:
		Corporation(const char *Name, short Id)
		{
			id = Id;
			name = Name;
		}
	public:
		const char *name;
		short id;
		bool techs[18];
		Corporation *next;
	};
}

#endif