#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <array>
#include <queue>
#include <future>
#include <unistd.h>
#include <functional>
#include <condition_variable>

using namespace std;

class ThreadPool
{
	vector<thread> threads;
	mutex m;
	queue<function<void()>> tasks;
	bool fin;
	condition_variable cond;
public:
    explicit ThreadPool(size_t poolSize):fin(false) {
        for (size_t i = 0; i < poolSize; ++i) {
            threads.emplace_back([this]{
                while (true) {
                    unique_lock<mutex> lock(m);
                    cond.wait(lock, 
                    [this](){return fin || !tasks.empty();});
                    if (fin) {
						while (true) {
							if (tasks.empty()) {
								lock.unlock();
								return;
							}
							auto fun = tasks.front();
							tasks.pop();
							lock.unlock();
							fun();
							lock.lock();
						}	
                    }
                    auto fun = tasks.front();
                    tasks.pop();
                    lock.unlock();
                    fun();	
                }
            });
        }
    }	
    
    template <class Func, class... Args>
    auto exec(Func func, Args...args) -> future<decltype(func(args...))> {
        auto cur = make_shared<packaged_task<typename result_of<Func(Args...)>::type()>>(
        bind(func, args...));
        auto res = cur->get_future();
        m.lock();
        tasks.emplace([cur](){ (*cur)(); });
        m.unlock();
        cond.notify_all();
        return res;
    }
	
    ~ThreadPool() {
        fin = true;
        cond.notify_all();
        for (thread &thr: threads) {
            thr.join();
        }
    }
};

struct A {};

int foo(const A&) {return -6;}

int main() {
	ThreadPool pool(8);

	auto task1 = pool.exec(foo, A());

	auto task2 = pool.exec([]() { return 1; });
	cout<<task2.get();
	cout<<task1.get();
	for (int i = 0; i < 10000; ++i) {
		 cout<<pool.exec([i]() { return i; }).get() << endl;
	}
}
