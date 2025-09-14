
#if 1
#include <gsl/gsl_linalg.h> 
#include <gtest/gtest.h>
#include <vector>
namespace
{
	std::vector<double> solve_linear_system(
		const std::vector<std::vector<double>>& A, const std::vector<double>& b)
	{
		// Check dimensions: 2x2 matrix A, 2-element vector b
		if (A.size() != 2 || A[0].size() != 2 || A[1].size() != 2 || b.size() != 2)
		{
			throw std::runtime_error(
				"Expected 2x2 matrix A and 2-element vector b");
		}
		// Create GSL matrix and vectors
		gsl_matrix* matrix = gsl_matrix_alloc(2, 2);
		gsl_vector* vec_b = gsl_vector_alloc(2);
		gsl_vector* x = gsl_vector_alloc(2);
		// Fill GSL matrix and vector
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				gsl_matrix_set(matrix, i, j, A[i][j]);
			}
			gsl_vector_set(vec_b, i, b[i]);
		}

		// Solve using LU decomposition
		gsl_permutation* p = gsl_permutation_alloc(2);
		int signum;
		gsl_linalg_LU_decomp(matrix, p, &signum);
		gsl_linalg_LU_solve(matrix, p, vec_b, x);
		// Extract solution
		std::vector<double> result(2);
		for (int i = 0; i < 2; ++i)
		{
			result[i] = gsl_vector_get(x, i);
		}

		// Clean up
		gsl_matrix_free(matrix);
		gsl_vector_free(vec_b);
		gsl_vector_free(x);
		gsl_permutation_free(p);
		return result;
	}
}
// namespace
TEST(GSLTest, Example1)
{
	std::vector<std::vector<double>> A = { { 3.0, 2.0 }, { 1.0, 4.0 } };
	std::vector<double> b = { 5.0, 7.0 };
	std::vector<double> expected_x = { 0.6, 1.6 };
	std::vector<double> x = solve_linear_system(A, b);
	ASSERT_EQ(x.size(), 2);
	EXPECT_NEAR(x[0], expected_x[0], 1e-6);
	EXPECT_NEAR(x[1], expected_x[1], 1e-6);
}
#endif