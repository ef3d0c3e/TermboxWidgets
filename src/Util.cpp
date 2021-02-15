#include "Util.hpp"

int Util::SizeWide(const String& s)
{
	int sz = 0;
	for (const auto& c : s)
		sz += wcwidth(c);
	return sz;
}
