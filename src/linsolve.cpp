#include <iostream>
#include <fstream>

#include "Matrix.h"
#include "GaussSolver.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc < 2) {
		cerr << "Usage: linsolve path/to/matrix_file.ext" << '\n';
		return -1;
	}

	string fname(argv[1]);
	ifstream data_file(fname, ios::in);
	if (!data_file.is_open()) {
		cerr << "Unable to open file \"" << fname << "\"\n";
		return -1;
	}

	Matrix<double> matrix;
	data_file >> matrix;
	data_file.close();
	GaussSolver<double> solver(matrix);

	cout << "Solving System of Linear Equations [" << fname << "]:" << endl;
	try {
		solver.solve(true);
	} catch (const std::exception &e) {
		cerr << e.what() << '\n';
	}
}
