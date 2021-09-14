#ifndef GAUSSSOLVER_H_
#define GAUSSSOLVER_H_

#include <iostream>
#include <vector>
#include <stdexcept>

#include "Matrix.h"

template<typename T> class GaussSolver
{
	private:
		std::vector<T> solution;
		Matrix<T> &matrix;

		void move_max_rows()
		{
			T max = 0;
			size_t max_i = 0, move_to = 0;

			for (size_t j = 0; j < matrix.get_width(); ++j) {
				for (size_t i = 0; i < matrix.get_height(); ++i) {
					if (matrix[i][j] > max) {
						max = matrix[i][j];
						max_i = i;
					}
				}

				if (move_to > matrix.get_height() - 1)
					return;

				matrix.move_row(max_i, move_to);
				max = 0;
				++move_to;
			}
		}

		void make_diagonal()
		{
			size_t width = matrix.get_width(), height = matrix.get_height();
			for (size_t k = 0; k < height; ++k) {
				for (size_t i = k + 1; i < height; ++i) {
					double f = matrix[i][k] / matrix[k][k];

					for (size_t j = k + 1; j < width; ++j)
						matrix[i][j] -= matrix[k][j] * f;

					matrix[i][k] = 0;
				}
			}
		}

		void calc_solution()
		{
			solution.resize(matrix.get_height());
			size_t height = matrix.get_height();
			for (int i = height - 1; i >= 0; --i) {
				solution[i] = matrix[i][height];

				for (size_t j = i + 1; j < height; j++)
					solution[i] -= matrix[i][j] * solution[j];

				solution[i] /= matrix[i][i];
			}
		}

	public:
		GaussSolver(Matrix<T> &matrix) : solution(), matrix { matrix } {}

		~GaussSolver() = default;

		void set_matrix(Matrix<T> &matrix)
		{
			this->matrix = matrix;
		}

		std::vector<T> solve(bool verbose = false)
		{
			if (matrix.get_width() != matrix.get_height() + 1)
				throw std::runtime_error("Invalid system matrix size: must be [N x N+1]!");

			if (verbose)
				std::cout << matrix
						<< "Moving rows with max elements to the main diagonal:"
						<< std::endl;

			move_max_rows();

			if (verbose)
				std::cout << matrix
						<< "Transforming system matrix into row echelon form:"
						<< std::endl;

			make_diagonal();

			if (verbose)
				std::cout << matrix << "System solution:" << std::endl;

			calc_solution();

			if (verbose) {
				std::cout << "{ ";
				size_t xn = 1;
				for (T val : solution) {
					std::cout << "x" << (xn++) << " = " << val;
					std::cout << ((xn <= matrix.get_height()) ? "; " : " }");
				}
				std::cout << std::endl;
			}

			return solution;
		}

		std::vector<T> get_solution()
		{
			return solution;
		}
};

#endif
