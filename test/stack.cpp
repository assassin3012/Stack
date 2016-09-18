#ifndef STACK_CPP
#define STACK_CPP
#include "stack.h"

using namespace std;

template <typename T>
stack<T>::stack() : array_(nullptr), array_size_(0), count_(0) {}
template <typename T>
stack<T>::~stack() { delete[] array_; }

template <typename T>
stack<T>::stack(const stack<T> & st) : array_size_(st.array_size_), array_(new T[st.array_size_]) {
	if (count > 0) { std::copy(st.array_, st.array_ + st.array_size_, array_); }
}
template <typename T>
size_t stack<T>::count() const noexcept { return count_; }
template <typename T>
void stack<T>::push(T const & el) {
	if (array_size_ == count_) {
		if (array_size_ == 0) { array_size_ = 1; }
		array_size_ = array_size_ * 2;
		T * ar = new T[array_size_];
		std::copy(array_, array_+count_, ar);
		delete[] array_;
		array_ = ar;
	}
	array_[count_++] = el;
}
template <typename T>
stack<T> & stack<T>::operator = (stack<T> & st) {
	if (this != &st) {
		(stack(st)).swap(*this);
	}
	return *this;
}
template <typename T>
void stack<T>::pop() {
	if (count_ != 0) {
		--count_;
	}
	else throw std::logic_error("Empty");
}
template <typename T>
T& stack<T>::top() const {
	if (count_ != 0) {
		return array_[count_ - 1];
	}
	else throw logic_error("Empty");
}
template <typename T>
void stack<T>::swap(stack & st) {
	std::swap(array_size_, st.array_size_);
	std::swap(count_, st.count_);
	std::swap(array_, st.array_);
}
#endif
 
