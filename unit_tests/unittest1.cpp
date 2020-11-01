#include "stdafx.h"
#include "CppUnitTest.h"
#include "base_entities.h"
#include "mesh.h"
#include "input_arrays.h"


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


	TEST_CLASS(Plane_tests)
	{
	public:
		TEST_METHOD(Plane_init_by_3_points_test)
		{
			base_entities::Plane expect_plane(base_entities::Vec3d(0,1,0),0);
			base_entities::Plane test_plane(base_entities::Vec3d(-1,0,0),
											base_entities::Vec3d(0,0,1),
											base_entities::Vec3d(0,0,0));

			for (int i  =0 ; i < 3; ++i)
			{
				Assert::AreEqual(expect_plane.normal[i], test_plane.normal[i]);
			}

			Assert::AreEqual(expect_plane.scalar, test_plane.scalar);
		}
	};


	TEST_CLASS(Mesh_tests)
	{
	public:

		void assert_mesh_arrays(double expect_point_array[POINT_ARRAY_CAPACITY][3],
							int expect_face_array[FACE_ARRAY_CAPACITY][MAX_SIZE_OF_FACE_ARRAY],
							double actual_point_array[POINT_ARRAY_CAPACITY][3],
							int actual_face_array[FACE_ARRAY_CAPACITY][MAX_SIZE_OF_FACE_ARRAY])
		{
			for (int i = 0 ; i< POINT_ARRAY_CAPACITY; ++i)
			{
				for (int j = 0; j < 3; ++j)
					Assert::AreEqual((int)expect_point_array[i][j], (int)actual_point_array[i][j]);
			}

			for (int i = 0; i < FACE_ARRAY_CAPACITY; ++i)
			{
				for (int j = 0; j < MAX_SIZE_OF_FACE_ARRAY; ++j)
					Assert::AreEqual((int)expect_face_array[i][j], (int)actual_face_array[i][j]);
			}
		}
		
		TEST_METHOD(Mesh_read_from_input_and_write_test)
		{
			double point_array[POINT_ARRAY_CAPACITY][3] = 
			{
				{1,0,0},
				{2,0,0},
				{3,0,0},
				{4,0,0}
			};

			int face_array[FACE_ARRAY_CAPACITY][MAX_SIZE_OF_FACE_ARRAY] =
			{
				{3,0,1,2},
				{-3,1,2,3}
			};

			double point_array_1[POINT_ARRAY_CAPACITY][3] = {};
			int face_array_1[FACE_ARRAY_CAPACITY][MAX_SIZE_OF_FACE_ARRAY] = {};

			mesh::Mesh test_mesh(point_array, 4, face_array, 2);
			test_mesh.updateArray(point_array_1, face_array_1);


			assert_mesh_arrays(point_array_1, face_array_1, point_array, face_array);

		}

		TEST_METHOD(Mesh_simple_cut_test)
		{
			double point_array[POINT_ARRAY_CAPACITY][3] =
			{
				{0,2,0},
				{3,-4,0},
				{-4,-2,0}
			};

			int face_array[FACE_ARRAY_CAPACITY][MAX_SIZE_OF_FACE_ARRAY] =
			{
				{3, 0, 1, 2}
			};

			double expect_point_array[POINT_ARRAY_CAPACITY][3] = 
			{
				{0,2,0},
				{3,-4,0},
				{-4,-2,0},
				{1,0,0},
				{-2,0,0}
			};

			int expect_face_array[FACE_ARRAY_CAPACITY][MAX_SIZE_OF_FACE_ARRAY] =
			{
				{3, 0, 3 ,4}
			};

			mesh::Mesh test_mesh(point_array, 3, face_array, 1);
			test_mesh.clipByPlane(base_entities::Plane(base_entities::Vec3d(-1, 0, 0), base_entities::Vec3d(0,0,1), base_entities::Vec3d(0,0,0) ));
			test_mesh.updateArray(point_array, face_array);

			assert_mesh_arrays(expect_point_array, expect_face_array, point_array, face_array);

		}

		TEST_METHOD(Mesh_polygon_with_holes_test)
		{
			double point_array[POINT_ARRAY_CAPACITY][3] =
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

			int face_array[FACE_ARRAY_CAPACITY][MAX_SIZE_OF_FACE_ARRAY] =
			{
				{4, 0, 1, 2, 3},
				{-4, 4, 5, 6, 7}
			};

			double expect_point_array[POINT_ARRAY_CAPACITY][3] =
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

			int expect_face_array[FACE_ARRAY_CAPACITY][MAX_SIZE_OF_FACE_ARRAY] =
			{
				{4, 0, 1, 8, 9},
				{-4, 4, 5, 10, 11}
			};

			base_entities::Plane plane = base_entities::Plane(base_entities::Vec3d(0,1,0), 0);
			mesh::Mesh test_mesh = mesh::Mesh(point_array ,8, face_array, 2);
			test_mesh.clipByPlane(plane);

			test_mesh.updateArray(point_array, face_array);

			assert_mesh_arrays(expect_point_array, expect_face_array, point_array, face_array);

			
		}

		TEST_METHOD(Mesh_nonconvex_polygon)
		{
			double point_array[POINT_ARRAY_CAPACITY][3] = 
			{
				{-9, 4, 0},
				{11, 4, 0},
				{7, -4, 0},
				{2, 1, 0},
				{-3, -4, 0}
			};

			int face_array[FACE_ARRAY_CAPACITY][MAX_SIZE_OF_FACE_ARRAY] =
			{
				{5, 0, 1, 2, 3, 4}
			};

			double expect_point_array[POINT_ARRAY_CAPACITY][3] = 
			{
				{-9, 4, 0},
				{11, 4, 0},
				{7, -4, 0},
				{2, 1, 0},
				{-3, -4, 0},
				{9, 0, 0},
				{3, 0, 0},
				{1, 0, 0},
				{-6, 0, 0}
			};

			int expect_face_array[FACE_ARRAY_CAPACITY][MAX_SIZE_OF_FACE_ARRAY] = 
			{
				{7, 0, 1, 5, 6, 3, 7, 8}
			};

			base_entities::Plane plane(base_entities::Vec3d(0,1,0), 0);

			mesh::Mesh test_mesh(point_array, 5, face_array, 1);
			test_mesh.clipByPlane(plane);
			test_mesh.updateArray(point_array, face_array);

			assert_mesh_arrays(expect_point_array, expect_face_array, point_array, face_array);
		}
		  
	};

}