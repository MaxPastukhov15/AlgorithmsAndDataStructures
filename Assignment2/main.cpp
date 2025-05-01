#include "Gauss.hpp"
#include <iostream>

using namespace linearsolver;

int main() {
    
    std::string /*file1,*/file2/*,ans*/;
    
    //std::cout << "Write name of file with matrix\n";
    //std::cin >> file1;
    std::cout << "\nWrite name of file with vector\n";
    std::cin >> file2;
    
    //MatrixXd A = readMatrixVectorFromCSV(file1);
    MatrixXd b = readMatrixVectorFromCSV(file2);
    
    /*for (int i = 0; i < 3; ++i){
    	for (int j = 0; j < 3; ++j) std::cout << A(i,j) << " ";
    	std::cout << '\n';
    }
    */
    for (int i = 0; i < 3; ++i) std::cout << b(i,1) << '\n';
    //MatrixXd res = solveGauss(A,b);
    
    //std::cout << "Write name of result file\n";
   // std::cin >> ans;
    
    //writeVectorMatrixToCSV(res,ans);    
    
    return 0;
}
