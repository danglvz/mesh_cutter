#ifndef MESH_H_
#define MESH_H_

#include "stdafx.h"
#include "base_entities.h" 
#include "input_arrays.h"



namespace my_mesh_cutter 
{
	namespace mesh
	{
		class Vertex
		{
		public:
			base_entities::Vec3d point;
			double distance;
			int occurence;
			bool visible;
		public:
			explicit Vertex();
			explicit Vertex(const base_entities::Vec3d &point);
			
		};


		class Edge
		{
		public:
			size_t vertex_arr[2];
			std::set<size_t> faces;
			bool visible;
		public:
			Edge(size_t first, size_t second);
			size_t &operator[]( size_t index );
			size_t operator[]( size_t index ) const;
			bool operator==(const Edge &other) const;
			
		};

		class Face
		{
		public:
			std::set<size_t> edges;
			std::set<size_t> holes;
			bool visible, isAHole;
		public:
			explicit Face();
			explicit Face(std::set<size_t> edge_array);
			Face(std::set<size_t> edge_array, std::set<size_t> holes_array);			
		};


		class Mesh
		{
		public:
			std::vector<Vertex> vertex_arr;
			std::vector<Edge> edges_arr;
			std::vector<Face> faces_arr;
		public:
			explicit Mesh();
			Mesh(const std::vector<Vertex> &vertices, const std::vector<Edge> &edges, const std::vector<Face> &faces);
			Mesh(const double input_points[POINT_ARRAY_CAPACITY][3], 
				size_t count_of_points, 
				const int input_faces[FACE_ARRAY_CAPACITY][MAX_SIZE_OF_FACE_ARRAY], 
				size_t count_if_faces);
						
			void clipByPlane(const base_entities::Plane &clipplane);
			void updateArray(double output_points[POINT_ARRAY_CAPACITY][3], 
							int output_faces[FACE_ARRAY_CAPACITY][MAX_SIZE_OF_FACE_ARRAY]); 
		};

	}

}







#endif