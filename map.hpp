
#ifndef SXF_MAP_HPP
#define SXF_MAP_HPP

#include <vector>
#include <string>
#include <map>

#include <point.hpp>

namespace sxf
{
	enum EObjectType
	{
		EOT_NOT_DRAW, EOT_POINT, EOT_LINE, EOT_POLYGON, EOT_TEXT
	};

	enum ESemanticsType
	{
		EST_ASCIIZ		= 0,
		EST_ANSI		= 126,
		EST_UNICODE		= 127,
		EST_INT8		= 1,
		EST_INT16		= 2,
		EST_INT32		= 4,
		EST_DOUBLE		= 8
	};

	template<typename sem_type>
	class CSemantics
	{
		public:

			ESemanticsType type;
			sem_type value;

			CSemantics()
			{
				;
			};

			CSemantics(const sem_type __value, const ESemanticsType __type) :
				type(__type), value(__value)
			{
				;
			};

			CSemantics & operator=(const CSemantics & sem)
			{
				type = sem.type;
				value = sem.value;

				return * this;
			};

			int cmp(const CSemantics & sem) const
			{
				const double sub = value - sem.value;
				const double threshold = 0.001;

				if(sub < threshold)
					return -1;
				else if(sub > threshold)
					return 1;

				return 0;
			};
	};

	class CObject
	{
		public:

			unsigned id, code, localization;
			std::vector<CVertex> pnts;
			std::vector<CObject> sub_objs;
			EObjectType type;
			std::string label;
			std::map<unsigned, CSemantics<std::string> > string_semantics;
			std::map<unsigned, CSemantics<double> > double_semantics;

			void init(const EObjectType __type, const unsigned __id, const unsigned __code, const unsigned __localization);
			void init(const std::vector<CVertex> __pnt, const EObjectType __type, const unsigned __id, const unsigned __code, const unsigned __localization, const std::string __label);
			CObject & operator=(const CObject & obj);
			void add_semantics(const unsigned code, const CSemantics<std::string> & sem);
			void add_semantics(const unsigned code, const CSemantics<double> & sem);
			std::string semantics_value(const unsigned code, const std::string default_value = "") const;
			double semantics_value(const unsigned code, const double default_value = 0) const;
			int cmp_double_semantics(const unsigned code, const uint64_t value, const uint64_t default_value) const;
	};

	class CMap
	{
		std::vector<unsigned> __codes;
		std::vector< std::vector<CVertex> > cnts;
		unsigned __pnts_num;

		void obj_prepare(CObject & obj, const double dx, const double dy);

		protected:

			struct
			{
				CVertex left_top;
				CVertex size_in_map_coord;
				unsigned height, width;
				std::vector< std::vector<CVertex> > matrix;
			} hm;

			virtual void __load(const std::string map_fname, const std::string height_map_fname) = 0;

		public:

			std::vector<CObject> objs;

			CMap();

			void load(const std::string map_fname, const std::string height_map_fname, const std::vector<unsigned> codes);
			std::vector< std::vector<CVertex> > contours(unsigned & pnts_num);
	};
};

#endif

