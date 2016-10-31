#pragma once
#include <iostream>
#include <stdexcept>
#include <memory>

class bitset
{
public:
	explicit bitset(size_t size = 0);
	bitset(bitset const & other) = delete;
	auto operator =(bitset const & other) -> bitset & = delete;
	bitset(bitset && other) = delete;
	auto operator =(bitset && other) -> bitset & = delete;
	auto test(size_t index) const -> bool;
	auto set(size_t index) -> void;
	auto reset(size_t index) -> void;
	auto size() const noexcept -> size_t;
	auto counter() const noexcept -> size_t;
private:
	std::unique_ptr<bool[]>  ptr_;
	size_t size_;
	size_t counter_;
};

//--------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class allocator
{
public:
	explicit allocator(size_t size = 0);
	allocator(allocator const & other);
	~allocator();
	auto operator =(allocator const &) -> allocator & = delete;
	auto resize() -> void;
	auto construct(T * ptr, T const & value) -> void;
	auto destroy(T * ptr) -> void;
	auto get() noexcept -> T *;
	auto get() const noexcept -> T const *;
	auto count() const noexcept -> size_t;
	auto size() const noexcept -> size_t;
	auto full() const noexcept -> bool;
	auto empty() const noexcept -> bool;
	auto test(size_t index) const -> bool;
	auto swap(allocator & other) -> void;	
private:
	auto destroy(T * first, T * last) -> void;
	T * ptr_;
	size_t size_;
	std::unique_ptr<bitset> map_;
	
};

//-----------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class stack
{
public:
	explicit stack(size_t size = 0); /*noexcept*/
	auto count() const noexcept -> size_t; /*noexcept*/
	auto empty() const noexcept -> bool; /*noexcept*/
	auto push(T const & el) -> void; /*strong*/
	auto operator = (stack<T> & st) -> stack<T> &; /*strong*/
	auto pop() -> void; /*strong*/
	auto top() const -> T const &; /*strong*/
	auto top() -> T &; /*strong*/
private:
	allocator<T> al_;
	auto throw_is_empty() const -> void;
	
};

//--------------------------------------------------------------------------------------------------------------------------------------

bitset::bitset(size_t s) : ptr_(std::make_unique<bool[]>(s)), 
	size_(s), counter_(0) {
	for (size_t t = 0; t < s; ++t) {
		ptr_[t] = 0;
	}
}

auto bitset::test(size_t index) const -> bool {
	if (index >= size_) std::out_of_range("In test");
	else {
		return ptr_[index];
	}
}

auto bitset::set(size_t index) -> void {
	if (index >= size_) std::out_of_range("In set");
	else {
		ptr_[index] = true;
		++counter_;
	}
}

auto bitset::reset(size_t index) -> void {
	if (index >= size_) std::out_of_range("In reset");
	else {
		ptr_[index] = false;
		--counter_;
	}
}

auto bitset::size() const noexcept -> size_t {return size_; }

auto bitset::counter() const noexcept -> size_t {return counter_; }

//--------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
allocator<T>::allocator(size_t s) : ptr_(static_cast<T *>(s != 0 ? operator new(s * sizeof(T)) : nullptr)),
size_(s), map_(std::make_unique<bitset>(s)) {}

template <typename T>
allocator<T>::allocator(allocator const & other) : ptr_(static_cast<T *>(other.size() != 0 ? operator new(other.size() * sizeof(T)) : nullptr)),
size_(other.size()), map_(std::make_unique<bitset>(other.size())) {
	for (size_t t = 0; t < other.size(); ++t) {
		if (other.test(t)) {
			this->construct(ptr_ + t, other.get()[t]);
		}
	}
}

template<typename T>
allocator<T>::~allocator() { 
	if (map_->counter_ > 0) {
		this->destroy(ptr_, ptr_ + size_);
	}
	operator delete(ptr_);
}


template <typename T>
auto allocator<T>::resize() -> void {
	size_t size = size_ * 2 + (size_ == 0);
	allocator<T> other(size);
	for (size_t t = 0; t < size_; ++t) {
		if (this->test(t)) {
			other.construct(other.get() + t, ptr_[t]);
		}
	}
	other.swap(*this);
}

template<typename T>
auto allocator<T>::destroy(T * ptr) -> void {
	if (ptr >= ptr_ + size_ || ptr < ptr_) std::out_of_range("In destroy(T * ptr)");
	size_t t = ptr - ptr_;
	if (this->test(t)) {
		ptr->~T();
		map_->reset(t);
	}
}

template <typename T>
auto allocator<T>::destroy(T * first, T * last)  -> void {
	for (; first != last; ++first) {
		this->destroy(first);
	}
}

template <typename T>
auto allocator<T>::construct(T * ptr, T const & value) -> void {
	if (ptr >= ptr_ + size_ || ptr < ptr_) std::out_of_range("In construct");
	size_t t = ptr - ptr_;
	if (this->test(t)) {
		this->destroy(ptr);
	}
	new(ptr) T(value);
	map_->set(t);
}

template<typename T>
auto allocator<T>::get() noexcept-> T * { return ptr_; }

template<typename T>
auto allocator<T>::get() const noexcept-> T const * { return ptr_; }

template<typename T>
auto allocator<T>::count() const noexcept -> size_t { return map_->counter(); }

template<typename T>
auto allocator<T>::size() const noexcept -> size_t { return size_; }

template<typename T>
auto allocator<T>::full() const noexcept -> bool { return (map_->counter() == size_); }

template<typename T>
auto allocator<T>::empty() const noexcept -> bool { return (map_->counter() == 0); }

template<typename T>
auto allocator<T>::test(size_t index) const -> bool { return map_->test(index); }

template <typename T>
auto allocator<T>::swap(allocator& s) -> void {
	std::swap(s.ptr_, ptr_);
	std::swap(s.map_, map_);
	std::swap(s.size_, size_);
}

//--------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
stack<T>::stack(size_t s) : al_(s) {}

template <typename T>
auto stack<T>::empty() const noexcept -> bool { return (al_.count() == 0); }

template <typename T>
auto stack<T>::count() const noexcept -> size_t { return al_.count(); }

template <typename T>
auto stack<T>::push(T const & el) ->void {
	if (al_.full()) {
		al_.resize();
	}
	al_.construct(al_.get() + al_.count(), el);
}

template <typename T>
auto stack<T>::operator = (stack<T> & st) -> stack<T> & {
	if (this != &st) {
		(this->al_).swap(allocator<T>(st.al_));
	}
	return *this;
}

template <typename T>
auto stack<T>::pop() -> void {
	if (al_.empty()) throw_is_empty();
	else {
		al_.destroy(al_.get() + al_.count() - 1);
	}
}

template <typename T>
auto stack<T>::top() -> T & {
	if (al_.empty()) throw_is_empty();
	else {
		return al_.get()[al_.count() - 1];
	}
}

template <typename T>
auto stack<T>::top() const -> T const & {
	if (al_.empty()) throw_is_empty();
	else {
		return al_.get()[al_.count() - 1];
	}
}

template<typename T>
auto stack<T>::throw_is_empty() const -> void {
	throw (std::logic_error("Empty stack"));
}
