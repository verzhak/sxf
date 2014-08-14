
#ifndef CODEPAGE_HPP
#define CODEPAGE_HPP

#include "lib/all.hpp"

namespace codepage
{
	string ansi_to_utf8(const string ansi);
	wstring utf8_to_wstring(const string utf8);
};

#endif

