#include "Utility.h"

void LowercaseInPlace(std::string& s)
{
	for (char& c : s)
	{
		c = tolower(c);
	}
}

std::string LowercaseCopy(const std::string& s)
{
	std::string result = s;
	LowercaseInPlace(result);
	return result;
}
