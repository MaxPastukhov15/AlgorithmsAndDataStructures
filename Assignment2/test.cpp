#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "Gauss.hpp"

using namespace Eigen;
using namespace LinearSolver;

namespace LinearSolverTest {

class GaussSolverTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Simple 2x2 system
        A2 << 2, -1,
              -1, 2;
        b2 << 0, 3;
        expected2 = A2.colPivHouseholderQr().solve(b2);

        // Simple 3x3 system
        A3 << 1, 2, -1,
              2, 1, -2,
             -3, 1, 1;
        b3 << 3, 3, -6;
        expected3 = A3.colPivHouseholderQr().solve(b3);

        // Another 3x3 system
        A4 << 2, 1, -1,
             -3, -1, 2,
             -2, 1, 2;
        b4 << 8, -11, -3;
        expected4 = A4.colPivHouseholderQr().solve(b4);

        // Singular matrix (dependent rows)
        singularA << 1, 2, 3,
                     2, 4, 6,
                     3, 6, 9;
        singularB << 1, 2, 3;
    }

    Matrix2d A2;
    Vector2d b2, expected2;

    Matrix3d A3, A4, singularA;
    Vector3d b3, b4, singularB;
    Vector3d expected3, expected4;
};

TEST_F(GaussSolverTest, SolvesSimple2x2System) {
    Vector2d result = solveGauss(A2, b2);
    ASSERT_TRUE(result.isApprox(expected2, 1e-6));
}

TEST_F(GaussSolverTest, SolvesSimple3x3System) {
    Vector3d result = solveGauss(A3, b3);
    ASSERT_TRUE(result.isApprox(expected3, 1e-6));
}

TEST_F(GaussSolverTest, ThrowsForSingularMatrix) {
    EXPECT_THROW({
        solveGauss(singularA, singularB);
    }, std::runtime_error);
}

TEST_F(GaussSolverTest, SolvesAnother3x3System) {
    Vector3d result = solveGauss(A4, b4);
    ASSERT_TRUE(result.isApprox(expected4, 1e-6));
}

// test CSV reading
TEST_F(GaussSolverTest, SolvesFromCSV) {
    MatrixXd A = readMatrixFromCSV("dataA.csv");
    VectorXd b = readVectorFromCSV("datab.csv");  
    if (A.rows() != b.size()) {
        FAIL() << "Matrix A has " << A.rows() << " rows but vector b has " << b.size() << " elements";
    }

    VectorXd expected(3);
    expected << 2, 3, -1;
    VectorXd result = solveGauss(A, b);

    ASSERT_TRUE(result.isApprox(expected, 1e-6));
}

} 

