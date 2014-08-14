
#ifndef SXF_SXF_HPP
#define SXF_SXF_HPP

#include <string>
#include <vector>

#include <point.hpp>
#include <map.hpp>
#include <file.hpp>

namespace sxf
{
	struct __attribute__((packed)) SRawPoint_double
	{
		double	x;
		double	y;
	};

	struct __attribute__((packed)) SRawPoint_uint32
	{
		uint32_t	x;
		uint32_t	y;
	};

	struct __attribute__((packed)) SSXFHeader
	{
		uint32_t	id;													// Идентификатор файла
		uint32_t	len;												// Длина записи паспорта
		uint32_t	version;											// Редакция формата
		uint32_t	checksum;											// Контрольная сумма
		char		date[12];											// Дата создания набора данных
		char		nomenclature[32];									// Номенклатура листа
		uint32_t	scale;												// Знаменатель масштаба
		char		list_name[32];										// Условное название листа

		struct
		{
#if __BYTE_ORDER == __LITTLE_ENDIAN
			
			uint8_t		data_state							:	2;		// Флаг состояния данных
			uint8_t		proj_conf							:	1;		// Флаг соответствия проекции
			uint8_t		is_real_coord						:	2;		// Флаг наличия реальных координат
			uint8_t		code_method							:	2;		// Флаг способа кодирования
			uint8_t		gen_table							:	1;		// Таблица генерализации
			uint8_t		label_encoding;									// Флаг кодирования подписи
			uint8_t		coord_accuracy;									// Флаг точности координат
			uint8_t		special_sort						:	1;		// Признак специальной сортировки данных
			uint8_t		not_used_flags						:	7;

#else

			uint8_t		gen_table							:	1;		// Таблица генерализации
			uint8_t		code_method							:	2;		// Флаг способа кодирования
			uint8_t		is_real_coord						:	2;		// Флаг наличия реальных координат
			uint8_t		proj_conf							:	1;		// Флаг соответствия проекции
			uint8_t		data_state							:	2;		// Флаг состояния данных
			uint8_t		label_encoding;									// Флаг кодирования подписи
			uint8_t		coord_accuracy;									// Флаг точности координат
			uint8_t		not_used_flags						:	7;
			uint8_t		special_sort						:	1;		// Признак специальной сортировки данных

#endif
		} flags;

		uint32_t	epsg;												// Код EPSG для системы координат или 0

		// Прямоугольный координаты углов листа (метры)
		struct
		{
			SRawPoint_double	south_west;
			SRawPoint_double	north_west;
			SRawPoint_double	north_east;
			SRawPoint_double	south_east;
		} rectangle;

		// Геодезические координаты углов листа (радианы)
		struct
		{
			SRawPoint_double	south_west;
			SRawPoint_double	north_west;
			SRawPoint_double	north_east;
			SRawPoint_double	south_east;
		} geodesy;

		uint8_t		ellipsoid;											// Вид эллипсоида
		uint8_t		h;													// Система высот
		uint8_t		proj;												// Проекция карты
		uint8_t		coord_system;										// Система координат
		uint8_t		plan_unit;											// Единица измерения в плане
		uint8_t		h_unit;												// Единица измерения по высоте
		uint8_t		border_type;										// Вид рамки
		uint8_t		map_type;											// Обобщенный тип карты

		uint8_t		not_used_1[64];
		double		not_used_2;
		uint32_t	points_per_meter;									// Точек на метр
		
		struct
		{
			SRawPoint_uint32 south_west;
			SRawPoint_uint32 north_west;
			SRawPoint_uint32 north_east;
			SRawPoint_uint32 south_east;
		} frame;														// Расположение рамки на приборе

		uint32_t	frame_classification_code;							// Классификационный код рамки объекта
		double		not_used_6[6];
	};

	struct __attribute__((packed)) SDataDescriptor
	{
		uint32_t	id;													// Идентификатор данных
		uint32_t	len;												// Длина дескриптора
		char		nomenclature[32];									// Номенклатура листа
		uint32_t	record_num;											// Число записей данных

		struct
		{
#if __BYTE_ORDER == __LITTLE_ENDIAN

			uint8_t		data_state							:	2;		// Флаг состояния данных
			uint8_t		proj_conf							:	1;		// Флаг соответствия проекции
			uint8_t		is_real_coord						:	2;		// Флаг наличия реальных координат
			uint8_t		code_method							:	2;		// Флаг способа кодирования
			uint8_t		gen_table							:	1;		// Таблица генерализации
			uint8_t		label_encoding;									// Флаг кодирования подписи
			uint16_t	not_used_flags;

#else

			uint8_t		gen_table							:	1;		// Таблица генерализации
			uint8_t		code_method							:	2;		// Флаг способа кодирования
			uint8_t		is_real_coord						:	2;		// Флаг наличия реальных координат
			uint8_t		proj_conf							:	1;		// Флаг соответствия проекции
			uint8_t		data_state							:	2;		// Флаг состояния данных
			uint8_t		label_encoding;									// Флаг кодирования подписи
			uint16_t	not_used_flags;

#endif
		} flags;

		uint32_t	not_used;
	};

	struct __attribute__((packed)) SRecord
	{
		uint32_t	id;													// Идентификатор начала записи
		uint32_t	len;												// Общая длина записи
		uint32_t	metrics_len;										// Длина метрики
		uint32_t	classification_code;								// Классификационный код

		union __attribute__((packed))
		{
			uint32_t	full;											// Собственный номер объекта
			uint16_t	field[2];										// Номер в группе (0), номер группы (1)
		} record_id;

		struct
		{
#if __BYTE_ORDER == __LITTLE_ENDIAN

			uint8_t		localization						:	4;		// Характер локализации
			uint8_t		not_used_1							:	4;
			
			uint8_t		not_used_2							:	1;
			uint8_t		with_semantics						:	1;		// Наличие семантики
			uint8_t		m_elem_len							:	1;		// Размер элемента метрики
			uint8_t		binding_vector						:	1;		// Наличие вектора привязки
			uint8_t		is_unicode							:	1;		// Признак текста в UNICODE
			uint8_t		not_used_3							:	3;

			uint8_t		m_format							:	1;		// Формат записи метрики
			uint8_t		dim									:	1;		// Размерность представления
			uint8_t		m_elem_type							:	1;		// Тип элемента метрики
			uint8_t		is_m_with_text						:	1;		// Признак метрики с текстом
			uint8_t		graphics							:	1;		// Наличие графики (знака)
			uint8_t		graphics_scale						:	1;		// Масштабируемость графики
			uint8_t		is_m_spline							:	2;		// Признак построения сплайна по метрике

#else

			uint8_t		not_used_1							:	4;
			uint8_t		localization						:	4;		// Характер локализации

			uint8_t		not_used_3							:	3;
			uint8_t		is_unicode							:	1;		// Признак текста в UNICODE
			uint8_t		binding_vector						:	1;		// Наличие вектора привязки
			uint8_t		m_elem_len							:	1;		// Размер элемента метрики
			uint8_t		with_semantics						:	1;		// Наличие семантики
			uint8_t		not_used_2							:	1;

			uint8_t		is_m_spline							:	2;		// Признак построения сплайна по метрике
			uint8_t		graphics_scale						:	1;		// Масштабируемость графики
			uint8_t		graphics							:	1;		// Наличие графики (знака)
			uint8_t		is_m_with_text						:	1;		// Признак метрики с текстом
			uint8_t		m_elem_type							:	1;		// Тип элемента метрики
			uint8_t		dim									:	1;		// Размерность представления
			uint8_t		m_format							:	1;		// Формат записи метрики

#endif
		} flags;

		// Уровень генерализации
		struct __attribute__((packed))
		{
#if __BYTE_ORDER == __LITTLE_ENDIAN

			uint8_t		low								:	4;		// Нижняя граница
			uint8_t		high							:	4;		// Верхняя граница

#else

			uint8_t		high							:	4;		// Верхняя граница
			uint8_t		low								:	4;		// Нижняя граница

#endif
		} gen_level;

		uint32_t	m_elem_num_big;										// Число точек метрики для больших объектов
		uint16_t	sub_object_num;										// Число подобъектов
		uint16_t	m_elem_num;											// Число точек метрики
	};

	struct __attribute__((packed)) SMTRHeader
	{
		uint32_t	id;
		int32_t		x_min;
		int32_t		y_min;
		int32_t		x_max;
		int32_t		y_max;
		float		size_el_mat;
		int32_t		h_min;
		int32_t		h_max;
		uint32_t	width;
		uint32_t	height;
	};

	/*
	struct __attribute__((packed)) SMTRHeader
	{
		uint32_t id;
		uint32_t x_min;
		uint32_t y_min;
		uint32_t x_max;
		uint32_t y_max;
		uint16_t not_used_1;
		uint16_t not_used_2;
		uint16_t not_used_3;
		uint16_t not_used_4;
		uint16_t not_used_5;
		uint16_t not_used_6;
		uint16_t height;
		uint16_t width;
		uint16_t elem_size;
		uint32_t elem_size_mm;
		uint16_t discrete_in_elem;
		uint16_t flags;
		uint16_t unit;
		int32_t h_min;
		int32_t h_max;
		uint32_t code;
		uint32_t point_per_meter;
		int32_t scale;
		uint16_t type;
		char not_used_7[6];
		uint16_t version;
		uint16_t not_used_8;
	};
	*/

	// ############################################################################ 

	class CSXFMap : public CMap
	{
		struct s_map
		{

			double scale;
			SSXFHeader header;
			SDataDescriptor data_desc;
			CVertex frame_min;
			std::vector<CObject> objs;

			uint8_t * read_pnt(std::vector<CVertex> & pnt, const SRecord & desc, uint8_t * buf, const unsigned pnt_num, const unsigned dim, const bool is_float, const bool is_large);
			uint8_t * read_label(std::string & label, uint8_t * buf);
			void read_next_obj(CFile & fl);
			CVertex cmp_rectangle(const bool is_min) const;
			CVertex cmp_frame(const bool is_min) const;
			void unpack_header();
			void unpack_data_desc();
			void unpack_record(SRecord & record);

			std::vector<CObject> load(const std::string fname);
			void check();

		} map;

		struct s_height_map
		{

			SMTRHeader header;
			std::vector< std::vector<CVertex> > pnts;

			void unpack_header();
			void create_point_grid(const std::vector< std::vector<CVertex> > & raw_pnt, const CVertex from, const CVertex to, const unsigned size);
			CVertex left_top();
			CVertex size_in_map_coord();

			std::vector< std::vector<CVertex> > load(const std::string fname);
			void check();

		} height_map;

		protected:

			void __load(const std::string map_fname, const std::string height_map_fname);

		public:

			CSXFMap();
	};

};

#endif

