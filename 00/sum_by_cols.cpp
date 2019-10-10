#include <chrono>
#include <iostream>

class Timer
{
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;
public:
    Timer()
        : start_(clock_t::now())
    {
    }

    ~Timer()
    {
        const auto finish = clock_t::now();
        const auto us = 
            std::chrono::duration_cast<microseconds>
                (finish - start_).count();
        std::cout << us << " us" << std::endl;
    }

private:
    const clock_t::time_point start_;
};
constexpr int SIZE = 10000;
using namespace std;
void sum_by_cols(int* mat[SIZE]) {
	Timer t;
	unsigned long long res = 0;
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			res += mat[j][i];
		}
	}
	cout<<res<<'\n';
}

int main() {
	int **mat = new int*[SIZE];
	for (int i = 0; i < SIZE; ++i) {
		mat[i] = new int[SIZE];
	}
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			mat[i][j] = i + j;
		}
	}
	sum_by_cols(mat);
	for (int i = 0; i < SIZE; ++i) {
		delete [] mat[i];
	}
	delete[] mat;
}
