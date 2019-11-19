#include <string.h>
#include <assert.h>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <limits.h>
#include <vector>

using namespace std;

void form(vector<string> &param) {}

template<class Cur, class... Tail>
void form(vector<string> &param, Cur &&cur, Tail&&... rest ) {
	stringstream buf;
	buf << forward<Cur>(cur);
	param.push_back(buf.str());
	form(param, forward<Tail>(rest)...);
}

template<class Str_type, class... Tail>
string format(Str_type &&a, Tail&&...rest) {
	string ans(a);
	vector<string> param;
	form(param, forward<Tail>(rest)...);
	size_t idx = 0, size = param.size();
	while ((idx = ans.find('{')) != ans.npos) {
		size_t next = 0, pos = 0;
		try {
			pos = stoi(ans.substr(idx + 1), &next, 10);
		} catch(invalid_argument &k) {
			throw runtime_error("Invalid argument\n");
		}
		if (ans[next + idx + 1] != '}' || pos >= size) {
			throw runtime_error("Invalid argument\n");
		}
		//из-за скобок прибавляем к next двойку
		ans.replace(idx, next + 2, param[pos]);
	}
	if (ans.find('}') != ans.npos) {
		throw runtime_error("Invalid argument\n");
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

