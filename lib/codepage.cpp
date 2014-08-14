
#include "lib/codepage.hpp"

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

	#define my_iconv_t iconv_t
	#define my_iconv_open iconv_open
	#define my_iconv iconv

#else

	// МГК
	
	#define my_iconv_t libiconv_t
	#define my_iconv_open libiconv_open
	#define my_iconv libiconv

#endif

bool is_ansi_to_local_init = false;
bool is_utf8_to_utf16_init = false;

my_iconv_t desc_ansi_to_local;
my_iconv_t desc_utf8_to_utf16;

string codepage::ansi_to_utf8(const string ansi)
{
	shared_ptr<char> in_buf, out_buf;
	char * p_in_buf, * p_out_buf;
	size_t in_bytes = 4096, out_bytes = 4096;

	if(! is_ansi_to_local_init)
	{
		throw_if((desc_ansi_to_local = my_iconv_open("UTF-8", "CP1251")) == (my_iconv_t) -1);

		is_ansi_to_local_init = true;
	}

	in_buf.reset(new char[in_bytes], std::default_delete<char[]>());
	out_buf.reset(new char[out_bytes], std::default_delete<char[]>());

	throw_null(p_in_buf = in_buf.get());
	throw_null(p_out_buf = out_buf.get());

	strncpy(p_in_buf, ansi.c_str(), in_bytes);
	p_in_buf[in_bytes - 1] = '\0';
	in_bytes = strlen(p_in_buf) + 1;

	throw_if(my_iconv(desc_ansi_to_local, & p_in_buf, & in_bytes, & p_out_buf, & out_bytes) == (size_t) -1);

	return string(out_buf.get());
}

wstring codepage::utf8_to_wstring(const string utf8)
{
	char ch;
	const unsigned size = utf8.size();
	unsigned v;
	wstring wstr;

	for(v = 0; v < size; v++)
	{
		ch = utf8[v];

		if((ch & 0xE0) == 0xC0)
		{
			// В данной программе - встречаются только 2-х байтовые символы
			
			const uint16_t high = ch & 0x1F, low = utf8[v + 1] & 0x3F;

			wstr += (wchar_t) ((high << 6) + low);
			v ++;
		}
		else if(! (ch & 0x80))
			wstr += (wchar_t) ch;
	}

	return wstr;
}

