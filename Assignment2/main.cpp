#include "Gauss.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <matrix_csv> <vector_file> <output_csv>\n";
        std::cerr << "  matrix_csv: CSV file with comma-separated values\n";
        std::cerr << "  vector_file: File with one value per line\n";
        return 1;
    }
    
    try {
        // Read matrix (proper CSV format)
        MatrixXd A = LinearSolver::readMatrixFromCSV(argv[1]);
        
        // Read vector (one value per line format)
        VectorXd b = LinearSolver::readVectorFromCSV(argv[2]);
        
        // Verify dimensions
        if (A.rows() != A.cols()) {
            std::cerr << "Error: Matrix must be square (has " << A.rows() 
                      << " rows and " << A.cols() << " columns)\n";
            return 1;
        }
        
        if (A.rows() != b.size()) {
            std::cerr << "Error: Matrix has " << A.rows() << " rows but vector has " 
                      << b.size() << " elements\n";
            return 1;
        }
        
        // Solve system
        VectorXd solution = LinearSolver::solveGauss(A, b);
        
        // Write output
        LinearSolver::writeVectorToCSV(solution, argv[3]);
        
        std::cout << "Solution successfully written to " << argv[3] << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
