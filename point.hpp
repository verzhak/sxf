
#ifndef SXF_POINT_HPP
#define SXF_POINT_HPP

namespace sxf
{
	class CVertex
	{
		public:

		double x, y, z;

		CVertex();
		CVertex(const CVertex & v);
		CVertex(const double t_x, const double t_y, const double t_z);

		const CVertex & operator=(const CVertex & v);
		double operator[](const unsigned ind) const;
		double & operator[](const unsigned ind);
		const CVertex operator+(const CVertex & v);
		const CVertex & operator+=(const CVertex & v);
		const CVertex operator-(const CVertex & v);
		double euclidean_distance_to(const CVertex & op) const;
	};
};

#endif

