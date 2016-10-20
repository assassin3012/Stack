#ifndef STACK_CPP
#define STACK_CPP
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
allocator<T>::~allocator() { operator delete(ptr_); }

template <typename T>
auto allocator<T>::swap(allocator& s) -> bool {
	std::swap(s.ptr_, ptr_);
	std::swap(s.count_, count_);
	std::swap(s.size_, size_);
};

template<typename T>
stack<T>::stack(size_t s) : allocator<T>(s) {}

template<typename T> 
stack<T>::~stack() { destroy(allocator<T>::ptr_, allocator<T>::ptr_ + allocator<T>::count_); }

template <typename T>
bool stack<T>::empty() const noexcept { return (allocator<T>::count_ == 0); }

template <typename T>
stack<T>::stack(const stack& st) : allocator<T>(st.size_) {
	for (size_t t = 0; t < st.count_; ++t) construct(allocator<T>::ptr_ + t, st.ptr_[t]);
	allocator<T>::count_ = st.count_;
};

template <typename T> /*noexcept*/
size_t stack<T>::count() const noexcept { return allocator<T>::count_; }

template <typename T> /*strong*/
void stack<T>::push(T const & el) {
	if (allocator<T>::size_ == allocator<T>::count_) {
		if (allocator<T>::size_ == 0) { allocator<T>::size_ = 1; }
		allocator<T>::size_ = allocator<T>::size_ * 2;
		T * ar = static_cast<T *>(operator new (sizeof(T)*size_));
		for (size_t t = 0; t < allocator<T>::count_; ++t) {
			construct(ar + t, allocator<T>::ptr_[t]);
		}
		operator delete(allocator<T>::ptr_);
		allocator<T>::ptr_ = ar;
	}
	construct(allocator<T>::ptr_ + allocator<T>::count_, el);
        ++allocator<T>::count_;
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
	if (allocator<T>::count_ != 0) {
		--allocator<T>::count_;
	}
	else throw std::logic_error("Empty");
}

template <typename T> /*strong*/
T& stack<T>::top() const {
	if (allocator<T>::count_ != 0) {
		return allocator<T>::ptr_[allocator<T>::count_ - 1];
	}
	else throw std::logic_error("Empty");
}
#endif
