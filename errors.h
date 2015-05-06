#ifndef ERRORS_H
#define ERRORS_H

namespace RON
{
	enum ErrorCodes
	{
		SUCCESS = 0,
		NULL_POINTER = 1,
	};
	struct Error
	{
		Error(unsigned int code = 0, const char *msg = 0)
		{
			this->code = code;
			this->message = msg;
		}
		bool operator ==(unsigned int code)
		{
			return this->code == code;
		}
		unsigned int code = 0;
		const char *message = 0;
	};
}; 

#endif