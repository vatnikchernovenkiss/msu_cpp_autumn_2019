#include <mutex>
#include <fstream>
#include <limits>
#include <thread>
#include <condition_variable>
#include <vector>

constexpr size_t type_size = sizeof(uint64_t);

void multi_sort(const char *in_file, const char *out_file) {
    int finished = 0;
    uint64_t total_min = 0;
    uint64_t cur_min_local[2] = {std::numeric_limits<uint64_t>::max(), std::numeric_limits<uint64_t>::max()};
    uint64_t checked = 0;
    uint64_t cur_min = 0;
    uint64_t count_min = 0;
    std::condition_variable_any cond;
    bool fl = false, null_check = false;
    uint64_t count_min_local[2] = {0, 0};
    std::recursive_mutex m;
    std::ofstream out(out_file, std::ios::binary);
    std::vector<std::thread> pair;
    for (int i = 0; i < 2; ++i) {
        pair.emplace_back([&](int num) {
            std::ifstream in(in_file,std::ios::binary);
            in.seekg(0, std::ios_base::end);
            uint64_t size = in.tellg();
            size = size / type_size;
            uint64_t offset = size / 2 + size % 2;
            while (checked < size ){
                in.seekg(num * offset * type_size, std::ios_base::beg );
                uint64_t end = num == 0 ? offset : size;
                for (uint64_t i = offset * num;  i < end; ++i) {
                    uint64_t cur = 0;
                    in.read(reinterpret_cast<char*>(&cur), type_size);
                    if (cur == cur_min_local[num]) {
                        count_min_local[num]++;
                    } else if ((!cur && !null_check) || (cur < cur_min_local[num] && cur > total_min)) {		
                        count_min_local[num] = 1;
                        cur_min_local[num] = cur;
                    } 
                }
                m.lock();
                finished++;		
                if (finished != 2) {		  
                    std::unique_lock<std::recursive_mutex> lock(m);
                    m.unlock();
                    cond.wait(lock);
                } else {
                    if (cur_min_local[0] < cur_min_local[1]) {
                        cur_min = cur_min_local[0];
                        count_min = count_min_local[0];
                    } else if (cur_min_local[0] > cur_min_local[1]) {
                        cur_min = cur_min_local[1];
                        count_min = count_min_local[1];
                    } else {
                        cur_min = cur_min_local[0];
                        count_min = count_min_local[0] + count_min_local[1];
                    }		
                    if (! cur_min) {
                        null_check = true;
					}
                    m.unlock();
                    cond.notify_one();
                    finished = 0;
                }
                for (uint64_t i = 0; i < count_min / 2 + num * (count_min % 2); ++i) {
                    m.lock();
                    out.write(reinterpret_cast<char*>(&cur_min), type_size);
                    m.unlock();
                }
                m.lock();
                if (!fl) {
                    total_min = cur_min; 
                    checked += count_min;   
                    fl = true;
                    count_min_local[0] = 0;
                    count_min_local[1] = 0;
                    cur_min_local[1] = std::numeric_limits<uint64_t >::max();
                    cur_min_local[0] = std::numeric_limits<uint64_t>::max();
                } else {
                    fl = false;
                }
                m.unlock();
            }
            in.close();
        }, i);
    }
    pair[0].join();
    pair[1].join();
	out.close();
}
