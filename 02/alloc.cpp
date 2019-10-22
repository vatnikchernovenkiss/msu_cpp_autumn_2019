#include <iostream>
#include <new>
#include <string.h>

using namespace std;

class LinearAllocator {
	size_t maxSize;
	char *buffer;
	char *current;
public:
	LinearAllocator(size_t maxSize = 0): maxSize(maxSize)
	{
		buffer = reinterpret_cast<char*> (malloc(maxSize));
		if (buffer == nullptr) {
			throw bad_alloc();
		}
		current = buffer;
	}
	char *alloc(size_t size) {
		if (current + size > buffer + maxSize) {
			return nullptr;
		}
		char *ret = current;
		current += size;
		return ret;
	}
	void reset() {
		current = buffer;
	}
	~LinearAllocator() {
		free(buffer);
	}
};

int main(int argc, char **argv) {
	try {
		LinearAllocator my_alloc(atoi(argv[1]));
		for (int i = 2 ; i < argc; ++i) {
			if (!strcmp(argv[i], "reset")) {
				cout<<"Reset!"<<endl;
				my_alloc.reset();
				continue;
			}
			char *d  = my_alloc.alloc(atoi(argv[i]));
			if (d == nullptr) {
				throw atoi(argv[i]);
			}
			cout<<"Allocated "<<atoi(argv[i])<<" bytes"<<endl;
		}
	} 
	catch(int &k) {
		cout<<"Error! Unable to allocate memory of size "<<k;
		return 1;
	}
	catch(bad_alloc) {
		cout<<"Error! Incorrect initial size";
		return 1;
	}
	cout<<"success!";
	return 0;
}
