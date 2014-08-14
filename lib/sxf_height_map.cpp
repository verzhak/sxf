
#include "lib/sxf.hpp"

vector< vector<CVertex> > CSXFMap::s_height_map::load(const string fname)
{
	unsigned v, u;

	pnts.clear();
	
#ifdef EMBEDDED // Ограничение - только для МГК

	header.width = 1000;
	header.height = 1000;

	for(v = 0; v < header.height; v++)
	{
		vector<CVertex> row;

		for(u = 0; u < header.width; u++)
			row.push_back(CVertex(u, v, 100));

		pnts.push_back(row);
	}

#else

	unsigned t, elem_num;
	float * p_buf;
	shared_ptr<float> buf;
	CFile fl(fname);
	
	fl(& header, sizeof(header));
	unpack_header();

	check();

	elem_num = header.height * header.width;
	buf.reset(new float[elem_num], std::default_delete<float[]>());
	p_buf = buf.get();
	throw_null(p_buf);

	fl(p_buf, elem_num * sizeof(float));
	CFile::unpack<float>(p_buf, elem_num);

	for(u = 0, t = 0; u < header.width; u++)
	{
		vector<CVertex> col;

		for(v = 0; v < header.height; v++, t++)
			col.push_back(CVertex(u, v, p_buf[t]));

		pnts.push_back(col);
	}

#endif

	return pnts;
}

void CSXFMap::s_height_map::check()
{
	; // TODO
}

void CSXFMap::s_height_map::unpack_header()
{
	CFile::unpack<uint32_t>(& header.id);
	CFile::unpack<uint32_t>((uint32_t *) & header.x_min);
	CFile::unpack<uint32_t>((uint32_t *) & header.y_min);
	CFile::unpack<uint32_t>((uint32_t *) & header.x_max);
	CFile::unpack<uint32_t>((uint32_t *) & header.y_max);
	CFile::unpack<float>(& header.size_el_mat);
	CFile::unpack<uint32_t>((uint32_t *) & header.h_min);
	CFile::unpack<uint32_t>((uint32_t *) & header.h_max);
	CFile::unpack<uint32_t>(& header.width);
	CFile::unpack<uint32_t>(& header.height);
}

CVertex CSXFMap::s_height_map::left_top()
{
	CVertex pnt;

	pnt.x = header.x_min;
	pnt.y = header.y_min;

	return pnt;
}

CVertex CSXFMap::s_height_map::size_in_map_coord()
{
	CVertex pnt;

	pnt.x = header.x_max - header.x_min;
	pnt.y = header.y_max - header.y_min;

	return pnt;
}

