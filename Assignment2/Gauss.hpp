#ifndef GAUSS_HPP
#define GAUSS_HPP 

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <Eigen/Dense>
#include "lazycsv.hpp"

using namespace Eigen;

namespace LinearSolver{

MatrixXd readMatrixFromCSV(const std::string& filename){
	lazycsv::parser<> csv(filename);
	
	size_t rows =0, cols = 0;
	for(const auto& cell : row){
		if (rows == 0){
			for (const auto& cell : row) cols++;
		}
		rows++;
	}
	
	MatrixXd matrix(rows,cols);
	
	size_t i = 0;
	for (const auto& row : csv){
		size_t j = 0;
		for (const auto& cell : row){
			matrix(i,j) = std::stod(cell.unescaped());
			j++;
		}
		i++;
	}
	
	return matrix;
}

VectorXd solveGauss(const MatrixXd& A, const VectorXd& b){
	if (A.rows() != A.cols() || A.rows() != b.size()){
		throw std::invalid_argument("Matrix dimensions are not compatible");
	}
	
	MatrixXd Ab(A.rows(),A.cols()+1);
	Ab << A,b;
	
	for (int i = 0; i < Ab.rows(); ++i){
		int maxRow = i;
		for (int k = i+1; k < Ab.rows();++i){
			if (std::abs(Ab(k,i)) > std::abs(Ab(maxRow,i))){
				maxRow = k;
			}
		}
		
		if (maxRow != i) Ab.row(i).swap(Ab.row(maxRow));
		
		if (Ab(i,i) == 0) throw std::runtime_error("Matrix is singular or nearly singular");
		
		Ab.row(i) /= Ab(i,i);
		
		for (int k = 0; k < Ab.rows(); ++k){
			if (k != i && Ab(k,i) != 0) Ab.row(k) -= Ab.row(i) * Ab(k,i);
		} 
	}
	
	VectorXd x = Ab.col(Ab.cols() - 1);
	return x;
}

void writeVectorToCSV(const VectorXd& x; const std::string& filename){
	std::ofstream out(filename);
	if (!out.is_open()) throw std::runtime_error("Cannot open file for writing");
	
	for (int i = 0; i < x.size(); ++i){
		out << x(i);
		if (i != x.size()-1) out << ",";
	}
	out << "\n";
	
	out.close();
	
}


} 
#endif
