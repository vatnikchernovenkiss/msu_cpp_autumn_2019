#include "serializer.h"
#include <string.h>
#include <cstdlib>
#include <climits>
#include <assert.h>
#include <sstream>


struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;
	bool d;
    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c, d);
    }
};

int main() {
	Data x { 1, true, 2,false}, y {100000, false,30000000000,false};
	std::stringstream k;
	Serializer serializer(k);
	serializer.save(x);
	serializer.save(y);
	k << "1 true -5 false";
	Data m { 0, false, 0,true};
	Deserializer deserializer(k);
	Error err = deserializer.load(m);
	assert(err == Error::NoError);
	assert(x.a == m.a);
	assert(x.b == m.b);
	assert(x.c == m.c);
	assert(x.d == m.d);
	err = deserializer.load(m);
	assert(err == Error::NoError);
	assert(y.a == m.a);
	assert(y.b == m.b);
	assert(y.c == m.c);
	assert(x.d == m.d);
	err = deserializer.load(m);
	assert(err == Error::CorruptedArchive);
	std::cout<<"Passed"<<std::endl;
	return 0;
}
