#include <iostream>
#include <functional>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <initializer_list>

using namespace std;

template<typename T> class Matrix
{
	private:
		size_t width { 0 }, height { 0 };
		vector<T> contents;

		int toIdx(int j, int i)
		{
			return i + j * width;
		}

		int toJ(int index)
		{
			return index % width;
		}

		int toI(int index)
		{
			return index / width;
		}

		void set_size(size_t width, size_t height)
		{
			this->width = width;
			this->height = height;
			contents.resize(width * height);
		}

	public:
		Matrix() : contents()
		{
		}

		Matrix(int width, int height) : Matrix()
		{
			set_size(width, height);
		}

		Matrix(initializer_list<initializer_list<T>> rows)
		{
			for (auto row : rows)
				add_row(row);
		}

		Matrix(Matrix &rhs) :
				width(rhs.width),
				height(rhs.height),
				contents(rhs.contents)
		{
		}

		~Matrix() = default;

		size_t get_width()
		{
			return width;
		}
		size_t get_height()
		{
			return height;
		}
		size_t size()
		{
			return contents.size();
		}

		void set(T val, int i, int j)
		{
			contents[toIdx(i, j)] = val;
		}

		T& at(int i, int j)
		{
			return contents[toIdx(i, j)];
		}

		T* operator [](int i)
		{
			return contents.data() + i * width;
		}

		const T& operator ()(int i, int j)
		{
			return at(i, j);
		}

		friend ifstream& operator >>(ifstream &in, Matrix &m)
		{
			string row_str { };
			T elem;
			vector<T> row;

			while (getline(in, row_str)) {
				stringstream rstream(row_str);
				while (rstream >> elem)
					row.push_back(elem);
				m.add_row(row);
				row.clear();
			}
			return in;
		}

		void add_row(const vector<T> &elements)
		{
			size_t i { height }, j { 0 };
			set_size(width = max(width, elements.size()), ++height);
			for (T elem : elements) {
				set(elem, i, j);
				++j;
			}
		}

		void move_row(size_t i_src, size_t i_dst)
		{
			if (i_src == i_dst)
				return;

			T tmp;
			for (size_t j = 0; j < width; ++j) {
				tmp = at(i_dst, j);
				at(i_dst, j) = at(i_src, j);
				at(i_src, j) = tmp;
			}
		}

		void for_each_row(function<void(T*)> action)
		{
			for (size_t i = 0; i < height; ++i)
				action((*this)[i]);
		}

		void dump()
		{
			for_each_row([this](T *row) {
				for (size_t j = 0; j < width; ++j)
				cout << row[j] << " ";
				cout << endl;
			});
			cout << endl;
		}
};

template<typename T> class LinearSolver
{
	private:
		vector<T> solution;
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
				cout << "Moving " << max << " from " << max_i << " to "
						<< move_to << endl;
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

			for (size_t i = 0; i < height; i++)
				cout << solution[i] << endl;
		}

	public:
		LinearSolver(Matrix<T> &matrix) : solution(), matrix { matrix } {}

		~LinearSolver() = default;

		void set_matrix(Matrix<T> &matrix)
		{
			this->matrix = matrix;
		}

		void solve()
		{
			move_max_rows();
			matrix.dump();
			make_diagonal();
			matrix.dump();
			calc_solution();
		}
};

int main(int argc, char **argv)
{
	ifstream data_file("matrix.txt", ios::in);
	if (!data_file.is_open()) {
		cerr << "Unable to open file";
		return -1;
	}

	Matrix<int> matrix;
	data_file >> matrix;
	data_file.close();
	matrix.dump();
	cout << endl;

	Matrix<double> shit {
			{ 1, 2, 3, 1 },
			{ 4, 5, 6, 0 },
			{ 7, 8, 9, 5 }
	};
	shit.dump();

	LinearSolver<double> solver(shit);
	solver.solve();
}
