#include <string.h>
#include <assert.h>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <limits.h>

using namespace std;

void format(string& a, string& what) {}

template<class Cur, class... Tail>
void format(string &a, string &what, Cur &&cur, Tail&&... Rest ) {
	stringstream buf;
	buf << forward<Cur>(cur);
	string val;
	unsigned k;
	char tmp;
	while (buf.get(tmp)) {
		val += tmp;
	}
	while ((k = a.find(what)) != UINT_MAX) {
		a.replace(k, 3, val);
	}
	what = '{' + to_string((atoi(what.c_str() + 1) + 1)) +'}';
	format(a, what, forward<Tail>(Rest)...);
}

template<class Str_type, class... Tail>
string format(Str_type a, Tail&&...Rest) {
	string ans(a), what = "{0}";
	format(ans, what, forward<Tail>(Rest)...);
	unsigned len = ans.length();
	for (unsigned i = 0; i < len; ++i) {
		if (ans[i] == '{' || ans[i] == '}') {
			throw runtime_error("Invalid argument\n");
		}
	}
	return ans;
}

class Test {
	int a;
	bool b;
	char c;
	string d;
	unsigned long long e;
	friend ostream& operator << (ostream&, const Test&);
public:
	Test():a(3), b(true), c('a'), d("test"), e(80000000000){};
};

ostream& operator <<(ostream& out, const Test& he) {
	out << he.a << ' ' << he.b  << ' '<< ' ' << he.c << ' ' << he.d << ' ' << he.e;
	return out;
}

int main() {
	string st("{1} {1} {2} {3} {4} {3} {1} {0}");
	auto text = format(st, 2, "one", 100000000000, true, Test());
	assert(text == "one one 100000000000 1 3 1  a test 80000000000 1 one 2");
	text = format("{2}!{1},{2}-{0}?",  false, "one", 2);
	assert(text == "2!one,2-0?");
	bool check = false;
	try {
		text = format("{1}{2}", 1, 2);
		check = true;
	} catch (const runtime_error& ex) {
		check = false;
	}
	assert(check == false);
	try {
		text = format("{0}{1", 1, 2);
		check = true;
	} catch (const runtime_error& ex) {
		check = false;
	}
	assert(check == false);
	return 0;
}

