#include <stdexcept>
#include <limits>
#include <memory>

template<class T>
class Allocator
{
public:
    using arg_type = T;
    using pointer = T*;
    using size_type = size_t;
    
    template<class... ArgsT>
    void construct(pointer ptr, ArgsT&& ... args) const
    {
        new (ptr) arg_type(std::forward<ArgsT>(args)...);
    }
    
    void destroy(pointer ptr) const
    {
        ptr->~T();
    }
    
    pointer allocate(size_type size) const
    {
        return static_cast<pointer>(operator new[](sizeof(arg_type) * size));
    }
    
    void deallocate(pointer ptr, size_type size) const
    {
        operator delete[](ptr, size * sizeof(arg_type));
    }
    
    size_type max_size() const 
    {
		return std::numeric_limits<size_type>::max();
    }
};

template<class T>
class Iterator: public std::iterator<std::random_access_iterator_tag, T>
{
public:
	using arg_type = T;
    using reference = T&;
    using pointer = T*;
    using iterator = Iterator<arg_type>;
    
private:
	pointer ptr;
	
public:
    
    explicit Iterator(pointer ptr): ptr{ptr} {}
    
    reference operator * () 
    {
        return *ptr;
    }
    
    iterator& operator [] (int n)
    {
        return ptr[n];
    }

    bool operator == (const iterator& other) const 
    {
        return ptr == other.ptr;
    }
    
    bool operator != (const iterator& other) const 
    {
        return ptr != other.ptr;
    }
    
    bool operator < (const iterator& other) const 
    {
        return ptr < other.ptr;
    }
    
    bool operator > (const iterator& other) const 
    {
        return ptr > other.ptr;
    }
    
    bool operator <= (const iterator& other) const 
    {
        return ptr <= other.ptr;
    }
    
    bool operator >= (const iterator& other) const 
    {
        return ptr >= other.ptr;
    }
    
    iterator& operator ++ () 
    {
        ++ptr;
        return *this;
    }
    iterator& operator -- () 
    {
        --ptr;
        return *this;
    }
    
    iterator operator ++ (int) 
    {
        auto tmp = this;
        ++ptr;
        return *tmp;
    }
    
    iterator operator -- (int) 
    {
        auto tmp = this;
        --ptr;
        return *tmp;
    }
    
    iterator& operator += (int n) 
    {
        ptr += n;
        return *this;
    }
    
    iterator& operator -= (int n) 
    {
        ptr -= n;
        return *this;
    }
    
    iterator operator + (const iterator& other) 
    {
        auto tmp = this;
        tmp.ptr += other.ptr;
        return *tmp;
    }
    iterator operator - (const iterator& other) 
    {
        auto tmp = this;
        tmp.ptr -= other.ptr;
        return *tmp;
    }
    
    iterator operator + (int n) 
    {
        auto tmp = this;
        ptr += n;
        return *tmp;
    }
    
    iterator operator - (int n) 
    {
        auto tmp = this;
        ptr -= n;
        return *tmp;
    }
};

template<class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using size_type = size_t;
    using arg_type = T;
    using pointer = T*;
    using reference = T&;
    using allocator_type = Alloc;
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
private:
    static constexpr size_type base_size = 64;
    Alloc alloc;
    size_type allocated;
    pointer data;
    size_type cur_size;
public:
	Vector(size_type size = 0):alloc(), allocated(base_size),
		data(alloc.allocate(allocated)), cur_size(size)
    {
        for (pointer ptr = data; ptr != data + size; ++ptr) {
            alloc.construct(ptr);
        }
    }
    
    size_type size() const 
    {
        return cur_size;
    }

    bool empty() const 
    {
        return !cur_size;
    }

    const reference operator[](size_type pos) const
    {
        if (pos < cur_size) {
            return data[pos];
        } else {
            throw std::out_of_range("Index out of range");
        }
    }
    
    reference operator[](size_type pos)
    {
        if (pos < cur_size) {
            return data[pos];
        } else {
            throw std::out_of_range("Index out of range");
        }
    }

    void push_back(arg_type&& value)
    {
        if (cur_size >= allocated) {
            reserve(allocated * 2);
        }
        alloc.construct(data + (cur_size)++, std::move(value));
    }
    
    void pop_back()
    {
        if (cur_size > 0) {
            --cur_size;
            alloc.destroy(data + cur_size);
        } else {
            throw std::out_of_range("Empty vector");
        }
    }
    
    void clear()
    {
        for (auto ptr = data; ptr != data + cur_size; ++ptr) {
            alloc.destroy(ptr);
        }
        cur_size = 0;
    }
    
    void reserve(size_type size)
    {
        if (size > allocated) {
			pointer tmp = alloc.allocate(size);
			for (size_type i = 0; i < size; ++i) {
				alloc.construct(tmp + i, std::move(*(data + i)));
				alloc.destroy(data + i);
			}
			alloc.deallocate(data, allocated);
			data = tmp;
			allocated = size;
		}
    }
    
    void resize(size_type newSize)
    {
		if (newSize < cur_size) {
            for (pointer ptr = data + newSize; ptr != data + cur_size; ++ptr) {
                alloc.destroy(ptr);
            }
            cur_size = newSize;
        } else {
            if (newSize > allocated) {
                reserve(newSize);
            }
            for (pointer ptr = data + cur_size; ptr != data + newSize; ++ptr) {
                alloc.construct(ptr);
            }
            cur_size = newSize;
        }
    }
    
    iterator begin()
    {
        return iterator(data);
    }
    iterator end()
    {
        return iterator(data + cur_size);
    }
    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }
    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }
    
    ~Vector()
    {
        for (pointer ptr = data; ptr != data + cur_size; ++ptr) {
            alloc.destroy(ptr);
        }
        alloc.deallocate(data, allocated);
    }
};
