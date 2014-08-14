
#include "lib/map.hpp"

void CObject::init(const EObjectType __type, const unsigned __id, const unsigned __code, const unsigned __localization)
{
	id = __id;
	code = __code;
	localization = __localization;
	type = __type;
}

void CObject::init(const vector<CVertex> __pnts, const EObjectType __type, const unsigned __id, const unsigned __code, const unsigned __localization, const string __label)
{
	id = __id;
	code = __code;
	localization = __localization;
	type = __type;
	label = __label;
	pnts = __pnts;
}

CObject & CObject::operator=(const CObject & obj)
{
	pnts = obj.pnts;
	sub_objs = obj.sub_objs;
	type = obj.type;
	code = obj.code;
	localization = obj.localization;
	label = obj.label;

	return * this;
}

void CObject::add_semantics(const unsigned code, const CSemantics<string> & sem)
{
	string_semantics[code] = sem;
};

void CObject::add_semantics(const unsigned code, const CSemantics<double> & sem)
{
	double_semantics[code] = sem;
};

string CObject::semantics_value(const unsigned code, const string default_value) const
{
	if(string_semantics.count(code))
		return string_semantics.at(code).value;

	return default_value;
}

double CObject::semantics_value(const unsigned code, const double default_value) const
{
	if(double_semantics.count(code))
		return double_semantics.at(code).value;

	return default_value;
}

int CObject::cmp_double_semantics(const unsigned code, const uint64_t value, const uint64_t default_value) const
{
	if(double_semantics.count(code))
	{
		const CSemantics<double> & sem = double_semantics.at(code);
		const ESemanticsType type = sem.type;
		uint64_t __value = value;
		double value_d = * ((double *) & __value);

		return sem.cmp(CSemantics<double>(value_d, type));
	}
	else if(string_semantics.count(code))
	{
		throw_;
	}
	else
	{
		// TODO default_value
	}

	return 0;
}

// ############################################################################ 

CMap::CMap()
{
	;
}

void CMap::load(const string map_fname, const string height_map_fname, const vector<unsigned> codes)
{
	__load(map_fname, height_map_fname);

	// ############################################################################ 
	
	const double dx = hm.size_in_map_coord.x / (double) hm.width;
	const double dy = hm.size_in_map_coord.y / (double) hm.height;

	for(auto & obj : objs)
		obj_prepare(obj, dx, dy);

	// ############################################################################ 
	
	__codes = codes;

	cnts.clear();
	__pnts_num = 0;

	auto add_obj = [ this ](const CObject & obj)
	{
		vector<CVertex> cnt;

		__pnts_num += obj.pnts.size();

		for(auto & pnt : obj.pnts)
			cnt.push_back(pnt);

		cnts.push_back(cnt);
	};

	for(auto & obj : objs)
		if(find(codes.begin(), codes.end(), obj.code) != codes.end())
		{
			add_obj(obj);

			for(auto & sub_obj : obj.sub_objs)
				add_obj(sub_obj);
		}
}

void CMap::obj_prepare(CObject & obj, const double dx, const double dy)
{
	for(auto & pnt : obj.pnts)
	{
		double __dy = (pnt.y - hm.left_top.y) / dy;
		double __dx = (pnt.x - hm.left_top.x) / dx;
		const unsigned from_v = __dy;
		const unsigned from_u = __dx;
		const unsigned to_v = from_v + 1;
		const unsigned to_u = from_u + 1;

		__dy -= from_v;
		__dx -= from_u;

		pnt.z =
			__dx * __dy * hm.matrix[from_v][from_u].z
			+
			__dx * (1 - __dy) * hm.matrix[from_v][to_u].z
			+
			(1 - __dx) * __dy * hm.matrix[to_v][from_u].z
			+
			(1 - __dx) * (1 - __dy) * hm.matrix[to_v][to_u].z;
	}

	for(auto & __obj : obj.sub_objs)
		obj_prepare(__obj, dx, dy);
}

vector< vector<CVertex> > CMap::contours(unsigned & pnts_num)
{
	pnts_num = __pnts_num;

	return cnts;
}

