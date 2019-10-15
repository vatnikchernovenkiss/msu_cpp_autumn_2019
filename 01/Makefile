 
all: calculator run test
calculator: calculator.cpp
	g++ calculator.cpp -o calculator -um-std=c++11 -O2
run:
	./calculator "3 * 6"
test:
	python3 test.py
clean: rm -rf *.o
