#ifndef TWtwString_H_INCLUDED
#define TWtwString_H_INCLUDED

#include <string>

class twString : public std::string
{
	public:
		twString();
		twString(const char *string);
		twString(const twString &string);
		twString(const std::string &string);
		bool fromClipboard();
		void toClipboard();
		static twString intToString(int number);
		static twString floatToString(float number);
		void fromInt(int number);
		int toInt();
		float toFloat();
		twString toMD5();
		twString toLower();
		twString toUpper();
		twString replaceAll(twString replaced, twString Replace);
};

#endif // TWtwString_H_INCLUDED
