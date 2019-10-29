#include <iostream>
#include <string.h>

using namespace std; 

class Matrix {
	class Row {
		size_t cols_number;
		int *array;
		Row(const Row &he){};
	public:
	
		Row():cols_number(0), array(nullptr){}
		
		Row(int k):cols_number(k), array(new int[cols_number]){}
		
		int& operator[] (size_t ind) {
			if (ind >= cols_number) {
				throw std::out_of_range("");
			}
			return array[ind];
		}
		
		void operator *= (int k) {
			for (size_t i = 0; i < cols_number; ++i) {
				array[i] *= k;
			}
		}
		
		bool operator == (const Row& he) const {
			if (cols_number != he.cols_number) {
				return false;
			}
			for (size_t i = 0; i < cols_number; ++i) {
				if (array[i] != he.array[i]) {
					return false;
				}
			}
			return true;
		}
		
		size_t Size() const {
			return cols_number;
		}
		
		void operator = (const Row& he) {
			if (array) {
				delete [] array;
			}
			array = new int[he.cols_number];
			for (size_t i = 0; i < he.cols_number; ++i) {
				array[i] = he.array[i];
			}
			cols_number = he.cols_number;
		}
		
		~Row() {
			if (array) {
				delete [] array;
			}
		}
	};
	size_t rows_number;
	Row *rows;
	Matrix(const Matrix &he){};
	void operator = (const Matrix &he){};
public:

	Matrix(size_t rows_number = 0, size_t cols_number = 0):rows_number(rows_number), rows(new Row[rows_number]) {
		for (size_t i = 0; i < rows_number; ++i) {
			rows[i] = Row(cols_number);
		}
	}
	
	Row& operator[] (size_t ind) {
		if (ind >= rows_number) {
			throw std::out_of_range("");
		}
		return rows[ind];
	}
	
	Matrix& operator *= (int k) {
		for (size_t i = 0; i < rows_number; ++i) {
			rows[i] *= k;
		}
		return *this;
	}
	
	bool operator == (const Matrix& he) const {
			if (rows_number != he.rows_number) {
				return false;
			}
			for (size_t i = 0; i < rows_number; ++i) {
				if (!(rows[i] == he.rows[i])) {
					return false;
				}
			}
			return true;
	}
	
	bool operator != (const Matrix& he) const {
		return !(*this == he);
	}
	
	size_t GetColumns() const {
		return rows[0].Size();
	}
	
	size_t GetRows() const {
		return rows_number;
	}
	
	~Matrix() {
		delete [] rows;
	}
};

int main(int argc, char **argv) {
	try {
		int row1 = atoi(argv[1]), col1 = atoi(argv[2]), 
		row2 = atoi(argv[3]), col2 = atoi(argv[4]);
		Matrix a(row1, col1), b(row2, col2);
		for (int i = 0; i < row1; ++i) {
			for (int j = 0; j < col1; ++j) {
				a[i][j] = i;
			}
		}
		for (int i = 0; i < row2; ++i) {
			for (int j = 0; j < col2; ++j) {
				b[i][j] = i * -2;
			}
		}
		for (int k = 5; k < argc; ++k) {
			if (argv[k][0] == 'a') {
				a *= atoi(argv[k] + 1);
			} else if (argv[k][0] == 'b') {
				b *= atoi(argv[k] + 1);
			} else if (!strcmp(argv[k], "cmp")) {
				cout << (a == b) << endl;
			} else if (!strcmp(argv[k], "uncmp")) {
				cout << (a != b) << endl;
			} else if (!strcmp(argv[k], "rows")) {
				cout<<a.GetRows()<<endl;
			} else if (!strcmp(argv[k], "cols")) {
				cout<<a.GetColumns() << endl;
			} else {
				cout << a[argv[k][0] - '0'][argv[k][1] - '0'] << endl;
			}
		}
	}
	catch (out_of_range &k) {
		cout<<"Bad range!"<<endl;
	}
	return 0;
}
