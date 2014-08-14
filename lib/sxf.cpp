
#include "lib/sxf.hpp"

CSXFMap::CSXFMap() :
	CMap()
{
	;
}

void CSXFMap::__load(const string map_fname, const string height_map_fname)
{
	objs = map.load(map_fname);
	hm.matrix = height_map.load(height_map_fname);

	hm.left_top = height_map.left_top();
	hm.size_in_map_coord = height_map.size_in_map_coord();
	hm.height = height_map.header.height;
	hm.width = height_map.header.width;
}

