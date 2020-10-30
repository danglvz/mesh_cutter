#include "stdafx.h"
#include "base_entities.h"

using namespace my_mesh_cutter::base_entities;

Vec3d::Vec3d()
{
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
}

Vec3d::Vec3d(double x,double y, double z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

double &Vec3d::operator[](int index)
{
	return v[index];	
}

double Vec3d::operator[](int index) const
{
	return v[index];	
}

Vec3d Vec3d::operator*(double scale) const
{
	return Vec3d(v[0] * scale, v[1] * scale, v[2] * scale);
}

Vec3d Vec3d::operator/(double scale) const
{
	return Vec3d(v[0] / scale, v[1] / scale, v[2] / scale);
}

Vec3d Vec3d::operator+(const Vec3d &other) const
{
	return Vec3d(v[0] + other.v[0], v[1] + other.v[1], v[2] + other.v[2]);
}

Vec3d Vec3d::operator-(const Vec3d &other) const
{
	return Vec3d(v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2]);
}

double Vec3d::dot(const Vec3d &other) const
{
	return v[0] * other.v[0] + v[1] * other.v[1] + v[2] * other.v[2];
}

Vec3d Vec3d::cross(const Vec3d &other) const
{
	return Vec3d(v[1] * other.v[2] - v[2] * other.v[1],
                 v[2] * other.v[0] - v[0] * other.v[2],
                 v[0] * other.v[1] - v[1] * other.v[0]);
}

Vec3d Vec3d::nomalize() const
{
	double m = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] + v[2]);
	return Vec3d(v[0] / m, v[1] / m, v[2] / m);
}




Plane::Plane() :
	normal(),
	scalar()
{}

Plane::Plane(const Vec3d &normal_, double scalar_) :
	normal(normal_),
	scalar(scalar_)
{}

Plane::Plane(const Vec3d &v0, const Vec3d &v1, const Vec3d &v2)
{
	normal = ( ( v1 - v0 ).cross(v2-v0) ).nomalize();
	scalar = v0.dot(normal);
}


Plane Plane::Normalize() const
{
	Plane result;
	double distance = std::sqrt(result.normal[0] * result.normal[0] + result.normal[1] * result.normal[1] + result.normal[2] * result.normal[2]);
	result.normal[0] = result.normal[0] / distance;
	result.normal[1] = result.normal[1] / distance;
	result.normal[2] = result.normal[2] / distance;
	result.scalar = result.scalar / distance;
    return result;
}


