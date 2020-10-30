#include "stdafx.h"
#include "CppUnitTest.h"
#include "base_entities.h"
#include "mesh.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace my_mesh_cutter;

namespace unit_tests
{		
	TEST_CLASS(Vec3d_TESTS)
	{
	public:

		TEST_METHOD(Vec3d_addition_test)
		{
			base_entities::Vec3d first(1, 2, 3), second(1, 1, 1), expect(2, 3, 4), result;

			result = first + second;

			for (int i = 0; i < 3; ++i)
				Assert::AreEqual(result[i], expect[i]);
		}

		TEST_METHOD(Vec3d_subtraction_test)
		{
			base_entities::Vec3d first(1,2,3), second(1,1,1), expect(0,1,2), result;

			result = first - second;

			for (int i = 0; i < 3; ++i)
				Assert::AreEqual(result[i], expect[i]);
		}

		TEST_METHOD(Vec3d_dot_product_test)
		{
			base_entities::Vec3d first(1,2,3), second(4, -5, 6);
			double result, expect = 12;

			result = first.dot(second);

			Assert::AreEqual(result, expect);
		}

		TEST_METHOD(Vec3d_cross_product_test)
		{
			base_entities::Vec3d first(3,-3, 1), second(4, 9, 2), expect(-15, -2, 39), result;

			result = first.cross(second);

			for (int i = 0; i < 3; ++i)
				Assert::AreEqual(result[i], expect[i]);
		}

	};
	TEST_CLASS(Mesh_tests)
	{
	public:
		/*
		TEST_METHOD(Mesh_clipper_triangle_test)
		{
			mesh::Mesh test_mesh = mesh::Mesh();
			test_mesh.vertex_arr.emplace_back(base_entities::Vec3d(0, 2, 0));  
			test_mesh.vertex_arr.emplace_back(base_entities::Vec3d(3, -4, 0));  
			test_mesh.vertex_arr.emplace_back(base_entities::Vec3d(-4, -2, 0));  
			
			test_mesh.edges_arr.emplace_back(0, 1);
			test_mesh.edges_arr.emplace_back(1, 2);
			test_mesh.edges_arr.emplace_back(2, 0);

			std::set<size_t> edge_arr;
			edge_arr.insert(0);
			edge_arr.insert(1);
			edge_arr.insert(2);
			test_mesh.faces_arr.emplace_back(edge_arr);

			std::vector<base_entities::Vec3d> expect_points;
			expect_points.emplace_back(0,2,0);
			expect_points.emplace_back(1,0,0);
			expect_points.emplace_back(-2,0,0);

			test_mesh.clipByPlane(base_entities::Plane(base_entities::Vec3d(0, -1, 0), 0));
			for (auto i = 0; i <  test_mesh.vertex_arr.size(); ++i)
			{
				for (int j = 0; j < 3; ++j)
					Assert::AreEqual(expect_points[i][j], test_mesh.vertex_arr[i].point[j]);


			}
			

		}
		*/
		TEST_METHOD(Mesh_read_from_input_and_write_test)
		{
			double point_array[10][3] = 
			{
				{1,0,0},
				{2,0,0},
				{3,0,0},
				{4,0,0}
			};

			int face_array[5][MAX_SIZE_OF_FACE_ARRAY] =
			{
				{3,0,1,2},
				{-3,1,2,3}
			};

			double point_array_1[10][3] = {};
			int face_array_1[5][MAX_SIZE_OF_FACE_ARRAY] = {};

			mesh::Mesh test_mesh(point_array, 4, face_array, 2);
			test_mesh.updatePointArray(point_array_1, face_array_1);

			for (int i = 0 ; i< 10; ++i)
			{
				for (int j = 0; j < 3; ++j)
					Assert::AreEqual(point_array[i][j], point_array_1[i][j]);
			}

			for (int i = 0; i < 5; ++i)
			{
				for (int j = 0; j < MAX_SIZE_OF_FACE_ARRAY; ++j)
					Assert::AreEqual(face_array[i][j], face_array_1[i][j]);
			}
		}

		TEST_METHOD(Mesh_simple_cut_test)
		{
			double point_array[10][3] =
			{
				{0,2,0},
				{3,-4,0},
				{-4,-2,0}
			};

			int face_array[5][MAX_SIZE_OF_FACE_ARRAY] =
			{
				{3, 0, 1, 2}
			};

			double expect_point_array[10][3] = 
			{
				{0,2,0},
				{3,-4,0},
				{-4,-2,0},
				{1,0,0},
				{-2,0,0}
			};

			int expect_face_array[5][MAX_SIZE_OF_FACE_ARRAY] =
			{
				{3, 0, 3 ,4}
			};

			mesh::Mesh test_mesh(point_array, 3, face_array, 1);
			test_mesh.clipByPlane(base_entities::Plane(base_entities::Vec3d(-1, 0, 0), base_entities::Vec3d(0,0,1), base_entities::Vec3d(0,0,0) ));
			test_mesh.updatePointArray(point_array, face_array);

			for (int i = 0 ; i< 10; ++i)
			{
				for (int j = 0; j < 3; ++j)
					Assert::AreEqual((int)point_array[i][j], (int)expect_point_array[i][j]);
			}
			
			for (int i = 0; i < 5; ++i)
			{
				for (int j = 0; j < MAX_SIZE_OF_FACE_ARRAY; ++j)
					Assert::AreEqual(face_array[i][j], expect_face_array[i][j]);
			}

		}

		TEST_METHOD(Mesh_polygon_with_holes_test)
		{
			double point_array[15][3] =
			{
				{-4, 4, 0},
				{4, 4, 0},
				{4, -4, 0},
				{-4, -4, 0},
				{-2, 2, 0},
				{2, 2, 0},
				{2, -2, 0},
				{-2, -2, 0}
			};

			int face_array[5][MAX_SIZE_OF_FACE_ARRAY] =
			{
				{4, 0, 1, 2, 3},
				{-4, 4, 5, 6, 7}
			};

			double expect_point_array[15][3] =
			{
				{-4, 4, 0},
				{4, 4, 0},
				{4, -4, 0},
				{-4, -4, 0},
				{-2, 2, 0},
				{2, 2, 0},
				{2, -2, 0},
				{-2, -2, 0},
				{4, 0, 0},
				{-4, 0, 0},
				{2, 0, 0},
				{-2, 0, 0}
			};

			int expect_face_array[5][MAX_SIZE_OF_FACE_ARRAY] =
			{
				{4, 0, 1, 8, 9},
				{-4, 4, 5, 10, 11}
			};

			base_entities::Plane plane = base_entities::Plane(base_entities::Vec3d(0,1,0), 0);
			mesh::Mesh test_mesh = mesh::Mesh(point_array ,8, face_array, 2);

			test_mesh.updatePointArray(point_array, face_array);


		}


	
	};

}