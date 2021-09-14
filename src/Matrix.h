#ifndef MATRIX_H_
#define MATRIX_H_

#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>
#include <algorithm>

template<typename T> class Matrix
{
	private:
		static constexpr size_t OUT_WIDTH = 10;
		size_t width { 0 }, height { 0 };
		std::vector<T> contents;

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
		Matrix() : contents() {}

		Matrix(int width, int height) : Matrix()
		{
			set_size(width, height);
		}

		Matrix(std::initializer_list<std::initializer_list<T>> rows)
		{
			for (auto row : rows)
				add_row(row);
		}

		Matrix(Matrix &rhs) :
				width(rhs.width),
				height(rhs.height),
				contents(rhs.contents) {}

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

		friend std::ifstream& operator >>(std::ifstream &in, Matrix &m)
		{
			std::string row_str { };
			T elem;
			std::vector<T> row;

			while (getline(in, row_str)) {
				std::stringstream rstream(row_str);
				while (rstream >> elem)
					row.push_back(elem);
				m.add_row(row);
				row.clear();
			}
			return in;
		}

		friend std::ostream& operator <<(std::ostream &out, Matrix &m)
		{
			out << m.to_string() << std::endl;
			return out;
		}

		void add_row(const std::vector<T> &elements)
		{
			size_t i { height }, j { 0 };
			set_size(width = std::max(width, elements.size()), ++height);
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

		void for_each_row(std::function<void(T*)> action)
		{
			for (size_t i = 0; i < height; ++i)
				action((*this)[i]);
		}

		std::string to_string()
		{
			std::stringstream out;
			for_each_row([this, &out](T *row) {
				for (size_t j = 0; j < width; ++j)
				out << std::setw(OUT_WIDTH) << row[j] << " ";
				out << '\n';
			});
			return out.str();
		}
};

#endif /* MATRIX_H_ */
