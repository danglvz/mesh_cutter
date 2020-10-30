#ifndef MY_MESH_CUTTER_BASE_ENTITIES_H
#define MY_MESH_CUTTER_BASE_ENTITIES_H

namespace my_mesh_cutter
{
	namespace base_entities
	{
		class Vec3d
		{
			double v[3];
		public:
			explicit Vec3d();
			Vec3d(double x, double y, double z);
			double &operator[](int index);
			double operator[](int index) const;
			Vec3d nomalize() const;
			Vec3d operator*(double scale) const;
			Vec3d operator/(double scale) const;
			Vec3d operator+(const Vec3d &other) const;
			Vec3d operator-(const Vec3d &other) const;
			double dot(const Vec3d &other) const;
			Vec3d cross(const Vec3d &other) const;
		};

		class Plane
		{
		public:
			Vec3d normal;
			double scalar;
		public:
			explicit Plane();
			Plane(const Vec3d &normal_, double scalar_);
			Plane(const Vec3d &v1, const Vec3d &v2, const Vec3d &v3);

			Plane Normalize() const;
		};
	}
	
}



#endif