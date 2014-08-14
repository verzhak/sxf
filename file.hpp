
#ifndef SXF_FILE_HPP
#define SXF_FILE_HPP

#include <string>
#include <typeinfo>

namespace sxf
{
	class CFile
	{
		FILE * fl;

		public:

			CFile(const std::string fname);
			~CFile();

			void operator()(void * buf, const size_t size);
			void seek(const uint32_t offset);
			long offset();

			#define PACK(__TYPE, fun)\
			else if(typeid(TYPE) == typeid(__TYPE))\
			{\
				for(v = 0; v < num; v++)\
					buf[v] = fun(buf[v]);\
			}

			#define PACK_REAL(__TYPE, __D_TYPE, fun)\
			else if(typeid(TYPE) == typeid(__TYPE))\
			{\
				__D_TYPE val;\
				\
				for(v = 0; v < num; v++)\
				{\
					val = fun(* (__D_TYPE *) (buf + v));\
					buf[v] = * (TYPE *) & val;\
				}\
			}

			template<typename TYPE>
				static void pack(TYPE * buf, const unsigned num = 1)
				{
					#ifndef __MINGW32__

						unsigned v;

						if(typeid(TYPE) == typeid(uint8_t))
						{
							;
						}
						PACK(uint16_t, htole16)
						PACK(uint32_t, htole32)
						PACK(uint64_t, htole64)
						PACK_REAL(float, uint32_t, htole32)
						PACK_REAL(double, uint64_t, htole64)
						else
						{
							throw_;
						}

					#endif
				}

			template<typename TYPE>
				static void unpack(TYPE * buf, const unsigned num = 1)
				{
					CFile::pack<TYPE>(buf, num);
				}
	};
};

#endif

