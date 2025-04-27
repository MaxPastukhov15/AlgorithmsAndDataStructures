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
    // First pass to count rows and columns
    lazycsv::parser<> csv(filename);
    size_t rows = 0, cols = 0;
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

    // Log the number of rows and columns
    std::cout << "CSV file has " << rows << " rows and " << cols << " columns." << std::endl;

    MatrixXd matrix(rows, cols);

    // Reinitialize the parser for the second pass
    lazycsv::parser<> csv_second_pass(filename);

    // Second pass to fill the matrix
    size_t i = 0;
    for (const auto& row : csv_second_pass) {
        size_t j = 0;
        for (const auto& cell : row) {
            std::string value = cell.unescaped();
            std::cout << "Parsing value: '" << value << "' at row " << i << ", col " << j << std::endl;
            value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
            if (value.empty()) {
                throw std::runtime_error("Empty value encountered");
            }
            matrix(i, j) = std::stod(value);
            j++;
        }
        i++;
    }
    return matrix;
}





VectorXd readVectorFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::vector<double> values;
    std::string line;
    
    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (!line.empty()) {
            try {
                values.push_back(std::stod(line));
            } catch (const std::exception& e) {
                throw std::runtime_error("Error parsing value '" + line + "': " + e.what());
            }
        }
    }

    return Eigen::Map<VectorXd>(values.data(), values.size());
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
