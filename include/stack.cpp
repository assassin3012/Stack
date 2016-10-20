#ifndef STACK_CPP
#define STACK_CPP
#include "stack.hpp"
#include <stdexcept>
using namespace std;

template <typename T> /*strong*/
T* newcopy(const T* st, const size_t new_count, const size_t new_size) {
	T* ar = new T[new_size];
	if (new_count > 0) {
		try {
			std::copy(st, st + new_count, ar);
		}
		catch (...) {
			delete[] ar;
			throw std::logic_error("???");
		}
	}
	return ar;
}

template <typename T1, typename T2>
void construct(T1 * ptr, T2 const & value) {
    new(ptr) T1 (value);
}

template <typename T>
void destroy(T * ptr) noexcept
{
    ptr->~T();
}

template <typename FwdIter>
void destroy(FwdIter first, FwdIter last) noexcept
{
    for (; first != last; ++first) {
        destroy(&*first);
    }
}

template <typename T>
allocator<T>::allocator(size_t s) : size_(0), count_(s),
	ptr_(static_cast<T *>(s != 0 ? operator new(s * sizeof(T))) : nullptr) {};

template<typename T>
allocator<T>::~allocator() {
	operator delete(ptr_);
}

template <typename T>//swap allocator
auto allocator<T>::swap(allocator& s)->void {
	std::swap(s.ptr_, ptr_);
	std::swap(s.count_, count_);
	std::swap(s.size_, size_);
};

template <typename T> /*noexcept*/
stack<T>::stack() : array_(nullptr), array_size_(0), count_(0) {}

template <typename T> /*noexcept*/
stack<T>::~stack() { delete[] array_; }

template <typename T> /*noexcept*/
bool stack<T>::empty() const noexcept { return (count_ == 0); }

template <typename T> /*strong*/
stack<T>::stack(const stack<T> & st) : array_size_(st.array_size_), count_(st.count_), array_(newcopy(st.array_, st.count_, st.array_size_)) {}

template <typename T> /*noexcept*/
size_t stack<T>::count() const noexcept { return count_; }

template <typename T> /*strong*/
void stack<T>::push(T const & el) {
	if (array_size_ == count_) {
		if (array_size_ == 0) { array_size_ = 1; }
		array_size_ = array_size_ * 2;
		T * ar = newcopy(array_, count_, array_size_);
		delete[] array_;
		array_ = ar;
	}
	array_[count_] = el;
        ++count_;
}

template <typename T> /*strong*/
stack<T> & stack<T>::operator = (stack<T> & st) {
	if (this != &st) {
		(stack(st)).swap(*this);
	}
	return *this;
}

template <typename T> /*strong*/
void stack<T>::pop() {
	if (count_ != 0) {
		--count_;
	}
	else throw std::logic_error("Empty");
}

template <typename T> /*strong*/
T& stack<T>::top() const {
	if (count_ != 0) {
		return array_[count_ - 1];
	}
	else throw std::logic_error("Empty");
}

template <typename T> /*noexcept*/
void stack<T>::swap(stack & st) {
	std::swap(array_size_, st.array_size_);
	std::swap(count_, st.count_);
	std::swap(array_, st.array_);
}

#endif
