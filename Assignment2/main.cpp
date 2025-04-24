#include "Gauss.hpp"

int main(int argc, char* argv[]){
	if (argc != 3){
		std::cerr "Usage:" << argv[0] << "<input_csv> <output_csv>\n";
		return 1;
	}
	
	try{
		MatrixXd A = LinearSolver::readMatrixFromCSV(argv[1]);
		
		if (A.rows() != A.cols() - 1){
			std::cerr << "Error: Matrix should be N x (N+1) for N equations\n";
			return 1;
		}
		
		MatrixXd coefficients = A.leftCols(A.cols() - 1);
		VectorXd rhs = A.rightCols(1);
		
		VectorXd solution = LinearSolver::solveGuass(coefficients,rhs);
		
		LinearSolver::writeVectorToCSV(solution, argv[2]);
		
		std::cout << "Solution successfully written to " << argv[2] << "\n";
	}
	catch (const std::exception& e){
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}
	return 0;
}
