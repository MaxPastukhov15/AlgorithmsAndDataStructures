#include "Gauss.hpp"
#include <iostream>

using namespace linearsolver;

int main() {
    
    std::string file1,file2,ans;
    
    std::cout << "Write name of file with matrix\n";
    std::cin >> file1;
    std::cout << "\nWrite name of file with vector\n";
    std::cin >> file2;
    
    MatrixXd A = readMatrixVectorFromCSV(file1);
    MatrixXd b = readMatrixVectorFromCSV(file2);
    
    MatrixXd res = solveGauss(A,b);
    
    std::cout << "Write name of result file\n";
    std::cin >> ans;
    
    writeVectorMatrixToCSV(res,ans);    
    return 0;
}
