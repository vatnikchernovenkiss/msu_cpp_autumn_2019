#include <iostream>
#include <string.h>
#include <cstdlib>
#include <climits>

using namespace std; 

class BigInt {
	short *value;
	size_t current_size;
	static constexpr size_t start_size = 20;
	bool is_negative;
	size_t max_size;
	friend BigInt operator + (const BigInt&, const BigInt&);
	friend BigInt operator - (const BigInt&, const BigInt&);
	friend bool operator > (const BigInt&, const BigInt&);
	friend bool operator < (const BigInt&, const BigInt&);
	friend bool operator == (const BigInt&, const BigInt&);
	friend bool operator != (const BigInt&, const BigInt&);
	friend bool operator >= (const BigInt&, const BigInt&);
	friend bool operator <= (const BigInt&, const BigInt&);
	friend ostream& operator << (ostream &a, const BigInt &val);
public:
	BigInt(int k = 0): value(new short[start_size]()),
	current_size(!k), is_negative(k < 0), max_size(start_size)
	{
		bool min = false;
		if (k == INT_MIN) {
			min = true;
			k--;
		} 
		k = abs(k);
		for (; k > 0; k = k / 10) {
			value[current_size++] = k % 10;
		}
		value[0] += min;
	}
	/*При копировании увеличиваем буфер на единицу. Этого достаточно, так как
	 в результате сложения или вычитания размер числа увеличивается максимум на один*/
	BigInt(const BigInt &he): value(new short[he.max_size + 1]()),
	current_size(he.current_size), is_negative(he.is_negative), max_size(he.max_size + 1)
	{
		for (size_t i = 0; i < he.current_size; ++i) {
			value[i] = he.value[i];
		}
	}
	
	BigInt(BigInt &&he): value(he.value),current_size(he.current_size),
	is_negative(he.is_negative), max_size(he.max_size)
	{
		he.value = nullptr;
		he.current_size = 0;
		he.is_negative = 0;
		he.max_size = 0;
	}
	
	BigInt& operator = (const BigInt &he) {
		if (this != &he) {
			current_size = he.current_size;
			is_negative = he.is_negative;
			max_size = he.max_size;
			delete [] value;
			value = new  short[he.max_size]();
			for (size_t i = 0; i < he.current_size; ++i) {
				value[i] = he.value[i];
			}
		}
		return *this;
	}
	
	BigInt& operator = (BigInt &&he) {
		if (this == &he) {
			return *this;
		}
		current_size = he.current_size;
		is_negative = he.is_negative;
		max_size = he.max_size;
		delete [] value;
		value = he.value;
		he.current_size = 0;
		he.is_negative = 0;
		he.value = nullptr;
		he.max_size = 0;
		return *this;
	}
	
	BigInt& operator += (const BigInt &he) {
		*this = *this + he;
		return *this;
	}
	
	BigInt& operator -= (const BigInt &he) {
		*this = *this - he;
		return *this;
	}
	
	BigInt operator-() const {
		BigInt res(*this);
		res.is_negative = !res.is_negative;
		return res;
	}
	~BigInt() {
		delete [] value;
	}
};

BigInt operator + (const BigInt &a, const BigInt &b) {
	//Ищем максимальное по модулю число
	BigInt res(a.is_negative > b.is_negative ? -a > b ? a : b : a.is_negative <
	b.is_negative ? a > -b ? a : b : a.is_negative ? -a > -b ? a : b : a > b ? a : b);
	const BigInt &other = a == res ? b : a;
	if ((a.is_negative && b.is_negative) || !(a.is_negative || b.is_negative)) {
		for (size_t i = 0; i < other.current_size; ++i) {
			res.value[i] = res.value[i] + other.value[i];
			size_t k = i;
			while (k < res.current_size && res.value[k] >= 10) {
				res.value[k + 1]++;
				res.value[k] = res.value[k] % 10;
				k++;
			}
			res.current_size += k == res.current_size;
		}
	} else {
		res.is_negative = -res <= other ? false : true;
		for (size_t i = 0; i < other.current_size; ++i) {
			res.value[i] = res.value[i] - other.value[i];
			size_t k = i;
			while (k < res.current_size && res.value[k] < 0) {
				res.value[k + 1] -= 1;
				if (k < res.current_size - 1) {
					res.value[k] = res.value[k] + 10;
				} else {
					res.value[k] = abs(res.value[k] );
				}
				k++;
			}
		}
		for (; res.current_size > 1 && !res.value[res.current_size - 1];res.current_size--) {}			
	}
	return res;
}	
			
BigInt operator - (const BigInt &a, const BigInt &b) {
	return a + (-b);
}

bool operator > (const BigInt &a, const BigInt &b) {
	if (a.is_negative == b.is_negative) {
		if (a.current_size == b.current_size) {
			bool fl = true;
			int  i = a.current_size - 1;
			//Так как число хранится в обратном порядке
			for (; i >= 0; --i) {
				if (b.value[i] > a.value[i]) {
					fl = false;
					break;
				} else if (a.value[i] > b.value[i]) {
					fl = true;
					break;
				}
			}
			if (i == -1) {
				return false;
			}	
			return (!a.is_negative && fl) || (a.is_negative && !fl);
		}
		return (a.is_negative && a.current_size < b.current_size) ||
				(!a.is_negative && a.current_size > b.current_size);
	}
	return a.is_negative < b.is_negative;
}

bool operator == (const BigInt &a, const BigInt &b) {
	if (a.current_size == b.current_size) {
		for (size_t i = 0; i < a.current_size; ++i) {
			if (b.value[i] != a.value[i]) {
				return false;
			}
		}
		return a.is_negative == b.is_negative;
	}
	return false;
}
			
bool operator < (const BigInt &a, const BigInt &b) {
	return !(a > b) && !(a == b);
}

bool operator >= (const BigInt &a, const BigInt &b) {
	return !(a < b);
}

bool operator <= (const BigInt &a, const BigInt &b) {
	return !(a > b);
}

ostream& operator << (ostream &a, const BigInt &val) {
	if (val.is_negative) {
		a<<'-';
	}
	for (int i = val.current_size - 1; i > 0; --i) {
		a << val.value[i];
	}
	a << val.value[0];
	return a;
}

int main(int argc, char **argv) {
	BigInt a(atoi(argv[1])), b(atoi(argv[2]));
	for (int i = 3; i < argc; ++i) {
		switch (argv[i][0]) {
			case '+':
			a = a + b;
			break;
			case '@':
			a = a - b;
			break;
			case '>':
			cout << (a > b) << endl;
			break;
			case '=':
			cout << (a == b) << endl;;
			break;
			case '!':
			b = -b;
			break;
			default:
			b = atoi(argv[i]);
			break;
		}
	}
	cout << a << endl;
	return 0;
}
