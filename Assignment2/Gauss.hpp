#ifndef GAUSS_HPP
#define GAUSS_HPP 

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <Eigen/Dense>
#include "lazycsv.hpp"


using namespace Eigen;

namespace LinearSolver {

MatrixXd generateRandomMatrix(int rows, int cols, double min = -10.0, double max = 10.0) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);

    MatrixXd matrix(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix(i, j) = dis(gen);
        }
    }
    return matrix;
}

MatrixXd readMatrixFromCSV(const std::string& filename) {
    lazycsv::parser<> csv(filename);
    
    // First pass to count rows and columns
    size_t rows = 0, cols = 0;
    for (const auto& row : csv) {
        if (rows == 0) {
            for (const auto& cell : row) {
                (void)cell; // unused variable
                cols++;
            }
        }
        rows++;
    }
    
    MatrixXd matrix(rows, cols);
    
    // Second pass to fill the matrix
    size_t i = 0;
    for (const auto& row : csv) {
        size_t j = 0;
        for (const auto& cell : row) {
            matrix(i, j) = std::stod(cell.unescaped());
            j++;
        }
        i++;
    }
    
    return matrix;
}

VectorXd solveGauss(const MatrixXd& A, const VectorXd& b) {
    if (A.rows() != A.cols() || A.rows() != b.size()) {
        throw std::invalid_argument("Matrix dimensions are not compatible");
    }
    
    const int n = A.rows();
    MatrixXd Ab(n, n + 1);
    Ab << A, b;
    
    // Forward elimination with partial pivoting
    for (int i = 0; i < n - 1; ++i) {
        // Partial pivoting
        int maxRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (std::abs(Ab(k, i)) > std::abs(Ab(maxRow, i))) {
                maxRow = k;
            }
        }
        
        // Swap the current row with max row
        if (maxRow != i) {
            Ab.row(i).swap(Ab.row(maxRow));
        }
        
        // Check for singularity
        if (std::abs(Ab(i, i)) < 1e-10) {
            throw std::runtime_error("Matrix is singular or nearly singular");
        }
        
        // Eliminate elements below current pivot
        for (int k = i + 1; k < n; ++k) {
            double factor = Ab(k, i) / Ab(i, i);
            Ab(k, i) = 0; // Explicitly set to zero
            for (int j = i + 1; j < n + 1; ++j) {
                Ab(k, j) -= Ab(i, j) * factor;
            }
        }
    }
    
    // Back substitution
    VectorXd x(n);
    for (int i = n - 1; i >= 0; --i) {
        x(i) = Ab(i, n);
        for (int j = i + 1; j < n; ++j) {
            x(i) -= Ab(i, j) * x(j);
        }
        x(i) /= Ab(i, i);
    }
    
    return x;
}

void writeVectorToCSV(const VectorXd& x, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) throw std::runtime_error("Cannot open file for writing");
    
    for (int i = 0; i < x.size(); ++i) {
        out << x(i);
        if (i != x.size() - 1) out << ",";
    }
    out << "\n";
    
    out.close();
}

} 
#endif
