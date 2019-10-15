#include <iostream>
#include <stack>
#include <string>

using namespace std;

class Error {
	const char *mes;
public:
	Error(const char* mes): mes(mes){}
	void get() const {
		cout<<mes;
	}
};
/*S->T{+T | -T}
T->F{*F | /F}
F-> number
*/

void space_skip(string::const_iterator &current) {
	while (isspace(*current)) {
		current++;
	}
}

void prep(int &second_op, int &first_op, stack<int> &st) {
	second_op = st.top();
	st.pop();
	first_op = st.top();
	st.pop();
}

void get_digit(stack<int> &st, string::const_iterator &current)
{
	int res = 0, fl = 1;
	if (!isdigit(*current)) {
		while (*current == '-') {
			fl *= -1;
			space_skip(++current);
		}
		if (!isdigit(*current)) {
			throw Error("error");
		}
	}		
	while (isdigit(*current)) {
		res = res * 10 + *current++ - '0';
	}
	st.push(fl * res);
	space_skip(current);
}

void mul_div_parse(stack<int> &st, string::const_iterator &current)
{
	get_digit(st, current);
	char buf;
	while ((buf = *current) == '*' || buf == '/') {
		space_skip(++current);
		get_digit(st, current);
		int second_op = 0,  first_op = 0;
		prep(second_op,first_op, st);
		if (buf == '*') {
			st.push(second_op * first_op);
		} else {
			st.push(first_op / second_op);
		}
	}
}

void add_sub_parse(stack<int> &st, string::const_iterator &current)
{
	space_skip(current);
	mul_div_parse(st, current);
	char buf;
	while ((buf = *current) == '+' || buf == '-') {
		space_skip(++current);
		mul_div_parse(st, current);
		int second_op = 0,  first_op= 0;
		prep(second_op, first_op, st);
		if (buf == '+') {
			st.push(second_op + first_op);
		} else {
			st.push(first_op - second_op);
		}
	}
	if (*current != '\0') {
		throw Error("error");
	}
}

int main(int argc, char **argv) 
{
	stack<int> st;
	string input(argv[1]);
	string::const_iterator current = input.begin();
	try {
		add_sub_parse(st,current);
	} 
	catch(Error res) {
		res.get();
		return 1;
	}
	cout<<st.top();
	return 0;
}
	

