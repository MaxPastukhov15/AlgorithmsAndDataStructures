#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "Gauss.hpp"

namespace LinearSolverTest {

class GaussSolverTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Простая 2x2 система
        A2 << 2, -1,
             -1, 2;
        b2 << 0, 3;
        expected2 << 1, 2;

        // Простая 3x3 система
        A3 << 1, 2, -1,
              2, 1, -2,
              -3, 1, 1;
        b3 << 3, 3, -6;
        expected3 << 1, 2, 3;

        // Вырожденная матрица
        singularA << 1, 2, 3,
                     4, 5, 6,
                     7, 8, 9;
        singularB << 1, 2, 3;
    }

    Matrix2d A2;
    Vector2d b2;
    Vector2d expected2;

    Matrix3d A3;
    Vector3d b3;
    Vector3d expected3;

    Matrix3d singularA;
    Vector3d singularB;
};

TEST_F(GaussSolverTest, SolvesSimple2x2System) {
    Vector2d result = LinearSolver::solveGauss(A2, b2);
    ASSERT_TRUE(result.isApprox(expected2, 1e-6));
}

TEST_F(GaussSolverTest, SolvesSimple3x3System) {
    Vector3d result = LinearSolver::solveGauss(A3, b3);
    ASSERT_TRUE(result.isApprox(expected3, 1e-6));
}

TEST_F(GaussSolverTest, ThrowsForSingularMatrix) {
    EXPECT_THROW(LinearSolver::solveGauss(singularA, singularB), std::runtime_error);
}

} 
