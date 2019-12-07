#include <iostream>
#include <mutex>
#include <fstream>
#include <limits>
#include <thread>
#include <condition_variable>
#include <vector>
#include <algorithm>
#include "Multithread_sort.h"
#include <assert.h>

using namespace std;

int main(int argc, char **argv) {
	remove("b.bin");
	ofstream ou("b.bin",std::ios::app);
	vector <uint64_t> r;
		for (unsigned long long i = 500; i < 2000; ++i) {
		for (int j = 0; j < 3; ++j) {
			ou.write((char*)&i,8);
			r.push_back(i);
		}
	}
	auto k = numeric_limits<unsigned long long >::max();
	ou.write((char*)&k, 8);
	r.push_back(k);
    ou.write((char*)&k, 8);
	r.push_back(k);
	ou.write((char*)&k, 8);
	r.push_back(k);
	for (long long i = 1000; i >=0; --i) {
		for (int j = 0; j < 3; ++j) {
			ou.write((char*)&i,8);
			r.push_back(i);
		}
	}
	ou.close();
	multi_sort("b.bin","res.bin");
	uint64_t size = r.size();
	sort(r.begin(), r.end());
	ifstream id("res.bin", ios::binary);
	for (uint64_t i = 0; i < size; ++i) {
		uint64_t cur = 0;
		id.read((char*)&cur, 8);
		assert(cur == r[i]);
	}
	id.close();
	cout << "OK!\n";
	return 0;
}

