собирается как статическая библиотека

содержит:

в пространстве имен my_mesh_cutter::base_entities
Plane - плоскость
Vec3d - вектор из трех точек типа double

пространство имен my_mesh_cutter::mesh
mesh - реализация координатной сетки, поддерживающей разрезание плоскостью

Использование:

#include "mesh.h"

using namespace my_mesh_cutter;

//инициализация разрезающей плоскости
//плоскоть можно инициализировать как по трем точкам:
base_entities::Plane clipp_plane = base_entities::Plane(base_entities::Vec3d(x,y,z), 
							base_entities::Vec3d(x,y,z), 
							base_entities::Vec3d(x,y,z));

//так и по нормали и расстоянию от точки начала координат
base_entities::Plane clipp_plane = base_entities::Plane(base_entities::Vec3d(x,y,z),
							scalar);

//для инициализации координатной сетки необходимо передать в конструктор указатели на массив точек и на массив полигонов
mesh::Mesh mesh = mesh::Mesh( /* указатель на массив */point_array,
			 	points_size, 
				/*указатель на массив полигонов*/faces_array, 
				faces_array_size );

//разрезание
mesh.clippByPlane(clipp_plane);


//обновление массивов точек и полигонов
mesh.updateArrays(poit_array, max_size_point_array,faces_array,max_size_faces_array);