#include "stdafx.h"
#include "mesh.h"


using namespace my_mesh_cutter;
using namespace my_mesh_cutter::mesh;
using namespace my_mesh_cutter::base_entities;

Vertex::Vertex() : 
	point(), 
	distance(), 
	occurence(), 
	visible(true)
{}

Vertex::Vertex(const Vec3d &point) : 
	point(point),
	distance(), 
	occurence(), 
	visible(true)
{}




Edge::Edge(size_t first, size_t second) : 
	visible(true)
{
	vertex_arr[0] = first;
	vertex_arr[1] = second;
}

size_t Edge::operator[](size_t index) const
{
	return vertex_arr[index];
}

size_t &Edge::operator[](size_t index)
{
	return vertex_arr[index];
}


bool Edge::operator==(const Edge &other) const
{
	return vertex_arr[0] == other.vertex_arr[0] && vertex_arr[1] == other.vertex_arr[1];
}






Face::Face(): 
	edges(), 
	visible(true),
	isAHole(false)
{}

Face::Face(std::set<size_t> edges_array) : 
	edges(edges_array),
	holes(),
	visible(true),
	isAHole(false)
{}

Face::Face(std::set<size_t> edge_array, std::set<size_t> holes_array):
	edges(edge_array),
	holes(holes_array),
	visible(true),
	isAHole(false)
{}





Mesh::Mesh() :vertex_arr(), edges_arr(), faces_arr() {}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<Edge> &edges, const std::vector<Face> &faces) : 
	vertex_arr(vertices),
	edges_arr(edges),
	faces_arr(faces)
{}

Mesh::Mesh(const double input_points[][3], size_t input_points_array_size, const int input_faces[][MAX_SIZE_OF_FACE_ARRAY], size_t input_face_array_size)
{
	for (size_t index = 0; index < input_points_array_size; ++index)
	{
		vertex_arr.emplace_back(base_entities::Vec3d(input_points[index][0], input_points[index][1], input_points[index][2]));
	}

	for (size_t input_face_index = 0; input_face_index < input_face_array_size; ++input_face_index)
	{
		faces_arr.emplace_back();
		size_t last_face_index = faces_arr.size() - 1;
		size_t last_edge_index;

		int point_index = 1, points_count = input_faces[input_face_index][0];
		if ( points_count < 0 )
		{
			faces_arr[last_face_index].isAHole = true;
			points_count *= -1;
		}

		for (; point_index < points_count; ++point_index)
		{
			Edge temp_edge = Edge(input_faces[input_face_index][point_index], input_faces[input_face_index][point_index + 1]);
			auto edge_iter = std::find(edges_arr.begin(), edges_arr.end(), temp_edge);

			if( edge_iter != edges_arr.end() )
			{
				last_edge_index =  edge_iter - edges_arr.begin();
				(*edge_iter).faces.insert(last_face_index);
			} else
			{
				temp_edge.faces.insert(last_face_index);
				edges_arr.emplace_back(temp_edge);
				last_edge_index =  edges_arr.size() - 1;
			}
				faces_arr[last_face_index].edges.insert(last_edge_index);
		}

		Edge temp_edge = Edge(input_faces[input_face_index][point_index], input_faces[input_face_index][1]);
		auto edge_iter = std::find(edges_arr.begin(), edges_arr.end(), temp_edge);
		if (edge_iter != edges_arr.end())
		{
			last_edge_index =  edge_iter - edges_arr.begin();
			(*edge_iter).faces.insert(last_face_index);
		} else
		{
			temp_edge.faces.insert(last_face_index);
			edges_arr.emplace_back(temp_edge);

			last_edge_index =  edges_arr.size() - 1;
		}

		faces_arr[last_face_index].edges.insert(last_edge_index);
	}

}



void Mesh::clipByPlane(const Plane &clipplane)
{

	for ( auto &vertex: vertex_arr  )
	{
		if ( vertex.visible )
		{
			vertex.distance = clipplane.normal.dot(vertex.point) - clipplane.scalar;
			if ( vertex.distance < 0 )
				vertex.visible = false;
			else if ( vertex.distance == 0 )
				vertex.occurence = 1;
		}
	}



	for ( auto &edge: edges_arr )
	{
		if ( edge.visible )
		{
			double first_point_distance = vertex_arr[edge[0]].distance;
			double second_point_distance = vertex_arr[edge[1]].distance;
			
			if ( first_point_distance <= 0 && second_point_distance <= 0 )
			{
				edge.faces.clear();
				edge.visible = false;
				continue;
			} 
			
			if ( first_point_distance >= 0 && second_point_distance >= 0 )
				continue;

			double t = first_point_distance/( first_point_distance - second_point_distance );
			Vec3d intersection_point = vertex_arr[edge[0]].point * (1 - t) + vertex_arr[edge[1]].point * t;
			vertex_arr.emplace_back(intersection_point);
			size_t new_vertex_index = vertex_arr.size() - 1;

			if ( first_point_distance > 0 )
				edge[1] = new_vertex_index;

			else
				edge[0] = new_vertex_index;
		}
	}

	
	for (auto &face: faces_arr)
	{
		for (auto iter = face.edges.begin(); iter != face.edges.end();)
		{
			if (!edges_arr[*iter].visible)
				iter = face.edges.erase(iter);
			else
				++iter;
		}
	}
}


void Mesh::updatePointArray(double output_points[][3], int output_faces[][MAX_SIZE_OF_FACE_ARRAY])
{
	for (size_t index = 0; index < vertex_arr.size(); ++index)
	{
		for (int i = 0; i < 3; ++i)
		{
			output_points[index][i] = vertex_arr[index].point[i];
		}
	}

	for ( size_t index = 0; index < faces_arr.size(); ++index )
	{

		int count_of_points = 0;
		

		int i  = 1 ;
		size_t temp_index = edges_arr[*(faces_arr[index].edges.begin())][0];
		for (auto &edge_index: faces_arr[index].edges)
		{
			if ( edges_arr[edge_index][0] == temp_index )
			{
				output_faces[index][i] = temp_index;
				++count_of_points;
			}
			else
			{
				output_faces[index][i++] = temp_index;
				++count_of_points;
				output_faces[index][i] = edges_arr[edge_index][0];
				++count_of_points;
			}
			temp_index = edges_arr[edge_index][1];
			++i;
		}


		if(faces_arr[index].isAHole)
			count_of_points *= -1;

		output_faces[index][0] = count_of_points;
	}
}
