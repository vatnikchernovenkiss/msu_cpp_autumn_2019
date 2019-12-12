#include <iostream>
#include <stack>
#include <string>

using namespace std;

class Error {
	string mes;
public:
	Error(const char* mes): mes(mes){}
	const string& get() const {
		return mes;
	}
};
/*S->T{+T | -T}
T->F{*F | /F}
F-> number
*/

void space_skip(const string& input, int &pos) {
	while (isspace(input[pos])) {
		++pos;
	}

}

void prep(int &second_op, int &first_op, stack<int> &st) {
	second_op = st.top();
	st.pop();
	first_op = st.top();
	st.pop();
}

void get_digit(stack<int> &st, const string& input, int &pos)
{
	int res = 0, fl = 1;
	if (!isdigit(input[pos])) {
		while (input[pos] == '-') {
			fl *= -1;
			space_skip(input, ++pos);
		}
		if (!isdigit(input[pos])) {
			if (input[pos] == '+' || input[pos] == '-' || input[pos] == '*' ||
			input[pos] == '/' || input[pos] == '\0') {
				throw Error("Incorrect using of operation");
			} else {
				throw Error("Incorrect symbol found");
			}
		}
	}		
	while (isdigit(input[pos])) {
		res = res * 10 + input[pos++] - '0';
	}
	st.push(fl * res);

	space_skip(input, pos);

}

void mul_div_parse(stack<int> &st, const string& input, int &pos)
{
	get_digit(st, input, pos);
	char buf;
	while ((buf = input[pos]) == '*' || buf == '/') {
		space_skip(input, ++pos);
		get_digit(st, input, pos);
		int second_op = 0,  first_op = 0;
		prep(second_op,first_op, st);
		if (buf == '*') {
			st.push(second_op * first_op);
		} else {
			if (!second_op) {
				throw Error("Division by zero!");
			}
			st.push(first_op / second_op);
		}
	}
}

void add_sub_parse(stack<int> &st, const string& input, int &pos)
{
	space_skip(input, pos);
	mul_div_parse(st, input, pos);
	char buf;
	while ((buf = input[pos]) == '+' || buf == '-') {
		space_skip(input, ++pos);
		mul_div_parse(st, input, pos);
		int second_op = 0,  first_op= 0;
		prep(second_op, first_op, st);
		if (buf == '+') {
			st.push(second_op + first_op);
		} else {
			st.push(first_op - second_op);
		}
	}
	if (input[pos] != '\0') {
		throw Error("Incorrect symbol found");
	}
}

int main(int argc, char **argv) 
{
	stack<int> st;
	int pos = 0;
	string input(argv[1]);
	try {
		add_sub_parse(st,input, pos);
	} 
	catch(Error &res) {
		cout<<res.get()<<endl;
		return 1;
	}
	cout<<st.top()<<endl;
	return 0;
}
	
