#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <utility>
#include <Eigen/Dense>
#include "lazycsv.hpp"


using namespace Eigen;

namespace linearsolver {

std::pair<size_t,size_t> Msize(const std::string& filename){
    size_t rows = 0, cols = 0;
   
    lazycsv::parser<lazycsv::mmap_source,lazycsv::has_header<false>> csv(filename);
    for (const auto& row : csv) {
       size_t current_cols = 0;
       for (const auto& cell : row) {
            (void)cell;
            current_cols++;
       }
       if (rows == 0) {
            cols = current_cols;
       }
       rows++;
    }
    return {rows, cols};
}


MatrixXd readMatrixVectorFromCSV(const std::string& filename) {
    
    std::pair<size_t,size_t> matrix_vector_size = Msize(filename);
  
    size_t rows = matrix_vector_size.first;
    size_t cols = matrix_vector_size.second; 

    MatrixXd matrix_vector(rows, cols);
   
    
    lazycsv::parser<lazycsv::mmap_source,lazycsv::has_header<false>> csv(filename);

    size_t i = 0;
    for (const auto& row : csv) {
        size_t j = 0;
        if (i >= rows) break; 
        for (const auto& cell : row) {
            std::string value = cell.unescaped();
            if (value.empty()) throw std::runtime_error("Empty value encountered");
            
            matrix_vector(i, j) = std::stod(value);
            ++j;
        }
        ++i;
    }
    return matrix_vector;
}


MatrixXd solveGauss(const MatrixXd& A, const MatrixXd& b) {
    if (A.rows() != A.cols() || A.rows() != b.rows() || b.cols() != 1 ) {
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
        FullPivLU<MatrixXd> lu(A);
	if (lu.rank() < A.rows()) {
            throw std::runtime_error("Matrix is singular or nearly singular");
        }
        
        // Eliminate elements below current pivot
        for (int k = i + 1; k < n; ++k) {
            double factor = Ab(k, i) / Ab(i, i);
            Ab(k, i) = 0; 
            for (int j = i + 1; j < n + 1; ++j) {
                Ab(k, j) -= Ab(i, j) * factor;
            }
        }
    }
    
    // Back substitution
    
    MatrixXd x(n,1);
    for (int i = n - 1; i >= 0; --i) {
        x(i,0) = Ab(i, n);
        for (int j = i + 1; j < n; ++j) {
            x(i,0) -= Ab(i, j) * x(j,0);
        }
        x(i,0) /= Ab(i, i);
    }
    
    return x;
}


void writeVectorMatrixToCSV(const MatrixXd& x, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) throw std::runtime_error("Cannot open file for writing");
    for (int i = 0; i < x.rows(); ++i) {
    	for (int j = 0; j < x.cols(); ++j){
        	out << x(i,j);
        	if (j != x.cols()-1) out << ",";
        }
        out << "\n";
    }
   
    
    out.close();
}


void generateRandomMatrix(int rows, int cols, double min, double max, const std::string& filename) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);

    MatrixXd matrix(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix(i, j) = dis(gen);
        }
    }
    writeVectorMatrixToCSV(matrix,filename);
}

} //namespace linearsolver 

