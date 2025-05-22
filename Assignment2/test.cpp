#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "Gauss.hpp"

using namespace Eigen;
using namespace linearsolver;

class GaussSolverTest : public ::testing::Test {
protected:
    void SetUp() override {
        
        A_good << 2, -1, 0,
                 -1, 2, -1,
                 0, -1, 2;
        b_good << 1, 0, 1;
        
        A_singular << 1, 2, 3,
                      1, 2, 3,
                      1, 2, 3;
        b_singular << 1, 1, 1;
        
        A_ill << 1, 1.0001,
                 1, 1.0000;
        b_ill << 2, 2;
    }

    Matrix3d A_good, A_singular;
    Vector3d b_good, b_singular;
    Matrix2d A_ill;
    Vector2d b_ill;
};

// Full cycle test (read-solve-write)
TEST_F(GaussSolverTest, CompleteWorkflow) {
    // Setup test files
    const std::string matrix_file = "test_A.csv";
    const std::string vector_file = "test_b.csv";
    const std::string output_file = "test_x.csv";
    
    // Write input files
    writeVectorMatrixToCSV(A_good, matrix_file);
    writeVectorMatrixToCSV(b_good, vector_file);
    
    // Read and solve system
    MatrixXd A = readMatrixVectorFromCSV(matrix_file);
    MatrixXd b = readMatrixVectorFromCSV(vector_file);
    MatrixXd x = solveGauss(A, b);
    
    // Verify solution in memory
    Vector3d expected(1, 1, 1); 
    ASSERT_TRUE(x.isApprox(expected, 1e-6));
    
    // Write solution to file
    writeVectorMatrixToCSV(x, output_file);
    
    // Verify file content matches expected "1\n1\n1\n" format
    std::ifstream out_file(output_file);
    std::string content(
        (std::istreambuf_iterator<char>(out_file)),
        std::istreambuf_iterator<char>()
    );
    
    // Trim trailing newline for comparison
    if (!content.empty() && content.back() == '\n') {
        content.pop_back();
    }
    
    ASSERT_EQ(content, "1\n1\n1") 
        << "File content mismatch. Got:\n" << content << "\nExpected:\n1\n1\n1";
    
    // Cleanup
    std::remove(matrix_file.c_str());
    std::remove(vector_file.c_str());
    std::remove(output_file.c_str());
}

// Solves well-conditioned system
TEST_F(GaussSolverTest, SolvesWellConditionedSystem) {
    MatrixXd x = solveGauss(A_good, b_good);
    Vector3d expected(1, 1, 1);
    ASSERT_TRUE(x.isApprox(expected, 1e-6));
}

// Solves ill-conditioned system (near singularity)
TEST_F(GaussSolverTest, SolvesIllConditionedSystem) {
    MatrixXd x = solveGauss(A_ill, b_ill);
    Vector2d expected(2, 0); // Approximate solution
    ASSERT_TRUE(x.isApprox(expected, 1e-4)); // Looser tolerance
}

// Throws for singular matrix
TEST_F(GaussSolverTest, ThrowsForSingularMatrix) {
    EXPECT_THROW({
        solveGauss(A_singular, b_singular);
    }, std::runtime_error);
}

//Throws for dimension mismatch
TEST_F(GaussSolverTest, ThrowsForDimensionMismatch) {
    Vector4d b_wrong_size;
    b_wrong_size << 1, 2, 3, 4;
    EXPECT_THROW({
        solveGauss(A_good, b_wrong_size);
    }, std::invalid_argument);
}
