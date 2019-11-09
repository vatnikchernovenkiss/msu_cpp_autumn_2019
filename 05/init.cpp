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
	Data x { 1, true, 2,false};
	std::stringstream k;
	Serializer serializer(k);
	serializer.save(x);
	Deserializer deserializer(k);
	Error err = deserializer.load(x);
}

