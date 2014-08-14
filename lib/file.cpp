
#include "lib/file.hpp"

CFile::CFile(const string fname)
{
	fl = NULL;
	throw_null(fl = fopen(fname.c_str(), "rb"));
}

CFile::~CFile()
{
	if(fl != NULL)
		fclose(fl);
}

void CFile::operator()(void * buf, const size_t size)
{
	throw_if(fread(buf, 1, size, fl) != size);
}

void CFile::seek(const uint32_t offset)
{
	throw_if(fseek(fl, offset, SEEK_SET));
}

long CFile::offset()
{
	return ftell(fl);
}

