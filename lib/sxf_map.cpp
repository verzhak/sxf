
#include "lib/sxf.hpp"

vector<CObject> CSXFMap::s_map::load(const string fname)
{
	int num;
	CFile fl(fname.c_str());
	fl(& header, sizeof(header));
	unpack_header();

	frame_min = cmp_frame(true);
	scale = header.points_per_meter / (double) header.scale;

	fl(& data_desc, sizeof(data_desc));
	unpack_data_desc();

	check();

	objs.clear();

	for(num = data_desc.record_num; num > 0; num--)
		read_next_obj(fl);

	return objs;
}

void CSXFMap::s_map::check()
{
	; // TODO
}

void CSXFMap::s_map::unpack_header()
{
	CFile::unpack<uint32_t>(& header.id);
	CFile::unpack<uint32_t>(& header.len);
	CFile::unpack<uint32_t>(& header.version);
	CFile::unpack<uint32_t>(& header.checksum);
	CFile::unpack<uint8_t>((uint8_t *) & header.date, 12);
	CFile::unpack<uint8_t>((uint8_t *) & header.nomenclature, 32);
	CFile::unpack<uint32_t>(& header.scale);
	CFile::unpack<uint8_t>((uint8_t *) & header.list_name, 32);
	CFile::unpack<uint32_t>((uint32_t *) & header.flags); // TODO Нужно ли?
	CFile::unpack<uint32_t>(& header.epsg);
	CFile::unpack<double>(& header.rectangle.south_west.x);
	CFile::unpack<double>(& header.rectangle.south_west.y);
	CFile::unpack<double>(& header.rectangle.north_west.x);
	CFile::unpack<double>(& header.rectangle.north_west.y);
	CFile::unpack<double>(& header.rectangle.north_east.x);
	CFile::unpack<double>(& header.rectangle.north_east.y);
	CFile::unpack<double>(& header.rectangle.south_east.x);
	CFile::unpack<double>(& header.rectangle.south_east.y);
	CFile::unpack<double>(& header.geodesy.south_west.x);
	CFile::unpack<double>(& header.geodesy.south_west.y);
	CFile::unpack<double>(& header.geodesy.north_west.x);
	CFile::unpack<double>(& header.geodesy.north_west.y);
	CFile::unpack<double>(& header.geodesy.north_east.x);
	CFile::unpack<double>(& header.geodesy.north_east.y);
	CFile::unpack<double>(& header.geodesy.south_east.x);
	CFile::unpack<double>(& header.geodesy.south_east.y);
	CFile::unpack<uint8_t>(& header.ellipsoid);
	CFile::unpack<uint8_t>(& header.h);
	CFile::unpack<uint8_t>(& header.proj);
	CFile::unpack<uint8_t>(& header.coord_system);
	CFile::unpack<uint8_t>(& header.plan_unit);
	CFile::unpack<uint8_t>(& header.h_unit);
	CFile::unpack<uint8_t>(& header.border_type);
	CFile::unpack<uint8_t>(& header.map_type);
	CFile::unpack<uint8_t>(header.not_used_1, 64);
	CFile::unpack<double>(& header.not_used_2);
	CFile::unpack<uint32_t>(& header.points_per_meter);
	CFile::unpack<uint32_t>(& header.frame.south_west.x);
	CFile::unpack<uint32_t>(& header.frame.south_west.y);
	CFile::unpack<uint32_t>(& header.frame.north_west.x);
	CFile::unpack<uint32_t>(& header.frame.north_west.y);
	CFile::unpack<uint32_t>(& header.frame.north_east.x);
	CFile::unpack<uint32_t>(& header.frame.north_east.y);
	CFile::unpack<uint32_t>(& header.frame.south_east.x);
	CFile::unpack<uint32_t>(& header.frame.south_east.y);
	CFile::unpack<uint32_t>(& header.frame_classification_code);
	CFile::unpack<double>(header.not_used_6, 6);
}

void CSXFMap::s_map::unpack_data_desc()
{
	CFile::unpack<uint32_t>(& data_desc.id);
	CFile::unpack<uint32_t>(& data_desc.len);
	CFile::unpack<uint8_t>((uint8_t *) data_desc.nomenclature, 32);
	CFile::unpack<uint32_t>(& data_desc.record_num);
	CFile::unpack<uint32_t>((uint32_t *) & header.flags);
	CFile::unpack<uint32_t>(& data_desc.not_used);
}

void CSXFMap::s_map::unpack_record(SRecord & record)
{
	CFile::unpack<uint32_t>(& record.id);
	CFile::unpack<uint32_t>(& record.len);
	CFile::unpack<uint32_t>(& record.metrics_len);
	CFile::unpack<uint32_t>(& record.classification_code);
	CFile::unpack<uint32_t>(& record.record_id.full);
	CFile::unpack<uint8_t>((uint8_t *) & record.gen_level);
	CFile::unpack<uint32_t>(& record.m_elem_num_big);
	CFile::unpack<uint16_t>(& record.sub_object_num);
	CFile::unpack<uint16_t>(& record.m_elem_num);
}

uint8_t * CSXFMap::s_map::read_pnt(vector<CVertex> & pnt, const SRecord & desc, uint8_t * buf, const unsigned pnt_num, const unsigned dim, const bool is_float, const bool is_large)
{
	// TODO Векторное представление

	unsigned ind;
	CVertex t_pnt;

	#define SET_POINT(type, unpack_type) \
	for(ind = 0; ind < pnt_num; ind ++)\
	{\
		type * p_buf = (type *) buf;\
		\
		CFile::unpack<unpack_type>((unpack_type *) p_buf, dim);\
		buf += sizeof(type) * dim;\
		\
		t_pnt.x = p_buf[0] * scale + header.rectangle.north_west.x;\
		t_pnt.y = p_buf[1] * scale + header.rectangle.north_west.y;\
		t_pnt.z = desc.flags.dim ? p_buf[2] : 0;\
		\
		pnt.push_back(t_pnt);\
	}

	pnt.clear();

	if(is_float)
	{
		if(is_large)
			SET_POINT(double, double)
		else
			SET_POINT(float, float);
	}
	else
	{
		if(is_large)
			SET_POINT(int32_t, uint32_t)
		else
			SET_POINT(int16_t, uint16_t);
	}

	return buf;
}

uint8_t * CSXFMap::s_map::read_label(string & label, uint8_t * buf)
{
	char * p_buf = (char *) buf;
	const unsigned len = * p_buf;
	unsigned v;

	p_buf++;

	for(v = 0, label = ""; v < len; v++, p_buf++)
		label += * p_buf;

	p_buf++; 

	label = codepage::ansi_to_utf8(label);

	return (uint8_t *) p_buf;
}

void CSXFMap::s_map::read_next_obj(CFile & fl)
{
	shared_ptr<uint8_t> buf;
	uint8_t * p_buf;
	unsigned v;
	SRecord desc;
	EObjectType type;

	fl(& desc, sizeof(desc));
	unpack_record(desc);

	switch(desc.flags.localization)
	{
		case 0:
		case 4:
		{
			type = EOT_LINE;

			break;
		}
		case 1:
		{
			type = EOT_POLYGON;

			break;
		}
		case 2:
		{
			type = EOT_POINT;

			break;
		}
		case 3:
		{
			type = EOT_TEXT;

			break;
		}
		default:
		{
			type = EOT_NOT_DRAW;

			break;
		}
	}

	if(desc.classification_code == header.frame_classification_code)
		type = EOT_NOT_DRAW;

	objs.resize(objs.size() + 1);

	auto current_obj = objs.end() - 1;
	const unsigned buf_size = desc.len - sizeof(desc);
	const unsigned dim = desc.flags.dim ? 3 : 2;
	const bool is_float = desc.flags.m_elem_type;
	const bool is_large = desc.flags.m_elem_len;
	const unsigned pnt_num = (desc.m_elem_num == 0xFFFF) ? desc.m_elem_num_big : desc.m_elem_num;

	current_obj->init(type, desc.record_id.full, desc.classification_code, desc.flags.localization);

	buf.reset(new uint8_t[buf_size], std::default_delete<uint8_t[]>());
	throw_null(p_buf = buf.get());
	fl(p_buf, buf_size);

	p_buf = read_pnt(current_obj->pnts, desc, p_buf, pnt_num, dim, is_float, is_large); 
	
	if(type == EOT_TEXT)
		p_buf = read_label(current_obj->label, p_buf);

	current_obj->sub_objs.resize(desc.sub_object_num);

	for(v = 0; v < desc.sub_object_num; v++)
	{
		string label;
		vector<CVertex> t_pnt;

		CFile::unpack<uint16_t>((uint16_t *) p_buf, 2);

		const uint16_t t_pnt_num = * (((uint16_t *) p_buf) + 1);

		p_buf = read_pnt(t_pnt, desc, p_buf + 4, t_pnt_num, dim, is_float, is_large);

		if(type == EOT_TEXT)
			p_buf = read_label(label, p_buf);

		current_obj->sub_objs[v].init(t_pnt, type, desc.record_id.full, desc.classification_code, desc.flags.localization, label);
	}

	// ############################################################################ 
	// Семантика
	
	p_buf = buf.get() + desc.metrics_len;
	unsigned processing_byte = desc.metrics_len;

	while(processing_byte < buf_size)
	{
		// TODO unpack

		const unsigned code = * ((uint16_t *) p_buf);
		const unsigned type = p_buf[2];
		const int scale = p_buf[3];
		const unsigned scale_u = p_buf[3];
		p_buf += 4;

		switch(type)
		{
			case 0:		// ASCIIZ
			case 126:	// ANSI (Windows)
			case 127:	// Unicode
			{
				current_obj->add_semantics(code, CSemantics<string>((char *) p_buf, (ESemanticsType) type));
				p_buf += scale_u + 1;
				processing_byte += 4 + scale_u + 1;

				break;
			}

#define DIGIT(tp_code, tp)\
			case tp_code:\
			{\
				current_obj->add_semantics(code, CSemantics<double>(* ((tp *) p_buf) * pow(10, scale), (ESemanticsType) type));\
				p_buf += tp_code;\
				processing_byte += 4 + tp_code;\
			\
				break;\
			}

			DIGIT(1, int8_t)
			DIGIT(2, int16_t)
			DIGIT(4, int32_t)
			DIGIT(8, double)
		}
	}
}

CVertex CSXFMap::s_map::cmp_rectangle(const bool is_min) const
{
	vector<double>
		border_x =
		{ 
			header.rectangle.south_west.x,
			header.rectangle.north_west.x,
			header.rectangle.north_east.x,
			header.rectangle.south_east.x
		},
		border_y =
		{ 
			header.rectangle.south_west.y,
			header.rectangle.north_west.y,
			header.rectangle.north_east.y,
			header.rectangle.south_east.y
		};

	return CVertex(
		is_min ? (* min_element(border_x.begin(), border_x.end())) : (* max_element(border_x.begin(), border_x.end())),
		is_min ? (* min_element(border_y.begin(), border_y.end())) : (* max_element(border_y.begin(), border_y.end())),
		0
		);
}

CVertex CSXFMap::s_map::cmp_frame(const bool is_min) const
{
	vector<uint32_t>
		border_x =
		{ 
			header.frame.south_west.x,
			header.frame.north_west.x,
			header.frame.north_east.x,
			header.frame.south_east.x
		},
		border_y =
		{ 
			header.frame.south_west.y,
			header.frame.north_west.y,
			header.frame.north_east.y,
			header.frame.south_east.y
		};

	return CVertex(
		is_min ? (* min_element(border_x.begin(), border_x.end())) : (* max_element(border_x.begin(), border_x.end())),
		is_min ? (* min_element(border_y.begin(), border_y.end())) : (* max_element(border_y.begin(), border_y.end())),
		0
		);
}

