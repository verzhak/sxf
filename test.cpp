
#include <cstdio>
#include <string>
#include <amv/amv.hpp>
#include <sxf.hpp>

using namespace std;
using namespace sxf;

int main(const int argc, const char * argv[])
{
	amv_init();

	throw_if(argc != 3);

	CSXFMap map;
	vector< vector<CVertex> > cnts;
	unsigned pnts_num;
	
	vector<unsigned> codes =
	{
	//	0x1DADA80, // ОЗЕРА (ПОСТОЯННЫЕ)  = НАБОР
	//	0x1DB0578, // ВОДОХРАНИЛИЩА ДЕЙСТВУЮЩИЕ = НАБОР
		0x1DF4750, // РЕКИ ПОСТОЯНЫЕ (от 20-120 м) = ЛИНИЯ
	//	0x1DF9958, // КАНАЛЫ = НАБОР (ЛИНИЯ)
	//	0x1E0CDF0, // УРЕЗЫ ВОДЫ = ТОЧКА
	//	0x44F5CF0, // БОЛОТА НЕПРОХОДИМЫЕ = ШТРИХОВАННАЯ ПЛОЩАДЬ

		0x3A7AB40, // ДОРОГИ ГРУНТОВЫЕ ПРОСЕЛОЧНЫЕ = ЛИНИЯ
	//	0x3A7D250, // ДОРОГИ ПОЛЕВЫЕ,ЛЕСНЫЕ = ПУНКТИРНАЯ ЛИНИЯ
	//	0x3A78430, // УЛУЧШЕННЫЕ ГРУНТ.ДОРОГИ ДЕЙСТВ = НАБОР
	//	0x3B12120, // ДОРОГИ ТРУДНОПРОЕЗЖИЕ(улуч.гр) = НАБОР
	//	0x3B6D9F8, // МОСТЫ ПРОЧИЕ = 149 (TODO)

	//	0x3A624A0, // ШОССЕ УСОВЕРШЕНСТВ.(ДЕЙСТВ.) = НАБОР
		0x3A64BB0, // ШОССЕ ДЕЙСТВУЮЩИЕ  = НАБОР
	//	0x3B6CE40, // МОСТЫ НА РАЗОБЩЕНН.ОСНОВАНИЯХ = 149 (TODO)
	};

	map.load(argv[1], argv[2], codes);
	cnts = map.contours(pnts_num);

	printf_TODO("Контуров = %u, точек = %u\n", cnts.size(), pnts_num);

	amv_destroy();
}

