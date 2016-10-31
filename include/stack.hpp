#pragma once
#include <iostream>
#include <stdexcept>

class bitset
{
public:
	explicit bitset(size_t size = 0);
	bitset(bitset const & other);
	auto operator =(bitset const &) -> bitset & = delete;
	~bitset();
	auto test(size_t index) const -> bool;
	auto set(size_t index) -> void;
	auto reset(size_t index) -> void;
	auto size() const noexcept -> size_t;
	auto swap(bitset& other) -> void;
private:
	size_t size_;
	bool* bit_;
	auto throw_about_range() -> void
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
	auto swap(allocator & other) -> void;
	auto resize() -> void;
	auto construct(T * ptr, T const & value) -> void;
	auto destroy(T * ptr) -> void;
	auto destroy(T * first, T * last) -> void;
	auto get() noexcept -> T *;
	auto get() const noexcept -> T const *;
	auto count() const noexcept -> size_t;
	auto size() const noexcept -> size_t;
	auto full() const noexcept -> bool;
	auto empty() const noexcept -> bool;
	auto test(size_t index) const -> bool;

private:
	T * ptr_;
	size_t size_;
	size_t count_;
	bitset bs_;
};

//-----------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class stack
{
public:
	explicit stack(size_t size = 0); /*noexcept*/
	~stack(); /*noexcept*/
	auto count() const noexcept -> size_t; /*noexcept*/
	auto empty() const noexcept -> bool; /*noexcept*/
	auto push(T const & el) -> void; /*strong*/
	auto operator = (stack<T> & st) -> stack<T> &; /*strong*/
	auto pop() -> void; /*strong*/
	auto top() const -> T const &; /*strong*/
private:
	allocator<T> al_;
};

//--------------------------------------------------------------------------------------------------------------------------------------

bitset::bitset(size_t s) : size_(s), 
bit_(s != 0 ? new bool[s] : nullptr) {
	for (size_t t = 0; t < s; ++t) {
		bit_[t] = 0;
	}
}

bitset::bitset(bitset const & other) : bitset(other.size()) {
	for (size_t t = 0; t < size_; ++t) {
		bit_[t] = other.bit_[t];
	}
}

bitset::~bitset() { delete[] bit_; }

auto bitset::test(size_t index) const -> bool {
	if (index >= size_) throw_about_range();
	else {
		return bit_[index];
	}
}

auto bitset::set(size_t index) -> void {
	if (index >= size_) throw_about_range();
	else {
		bit_[index] = true;
	}
}

auto bitset::reset(size_t index) -> void {
	if (index >= size_) throw_about_range();
	else {
		bit_[index] = false;
	}
}

auto bitset::size() const noexcept -> size_t {return size_; }

auto bitset::swap(bitset& other) -> void {
	std::swap(size_, other.size_);
	std::swap(bit_, other.bit_);
}

autobitset::throw_about_range() const -> void {
	throw (std::out_of_range("Out of range"));
}

//--------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
allocator<T>::allocator(size_t s) : ptr_(static_cast<T *>(s != 0 ? operator new(s * sizeof(T)) : nullptr)),
size_(s), count_(0), bs_(s) {}

template <typename T>
allocator<T>::allocator(allocator const & other) : allocator<T>(other.size()) {
	for (size_t t = 0; t < other.size(); ++t) {
		if (other.test(t)) {
			this->construct(ptr_ + t, other.get()[t]);
		}
	}
}

template<typename T>
allocator<T>::~allocator() { 
	if (count_ > 0) {
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
	if (bs_.test(t)) {
		ptr->~T();
		bs_.reset(t);
		--count_;
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
	if (bs_.test(t)) {
		this->destroy(ptr);
	}
	new(ptr) T(value);
	bs_.set(t);
	++count_;
}

template<typename T>
auto allocator<T>::get() noexcept-> T * { return ptr_; }

template<typename T>
auto allocator<T>::get() const noexcept-> T const * { return ptr_; }

template<typename T>
auto allocator<T>::count() const noexcept -> size_t { return count_; }

template<typename T>
auto allocator<T>::size() const noexcept -> size_t { return size_; }

template<typename T>
auto allocator<T>::full() const noexcept -> bool { return (count_ == size_); }

template<typename T>
auto allocator<T>::empty() const noexcept -> bool { return (count_ == 0); }

template<typename T>
auto allocator<T>::test(size_t index) const -> bool { return bs_.test(index); }

template <typename T>
auto allocator<T>::swap(allocator& s) -> void {
	std::swap(s.ptr_, ptr_);
	std::swap(s.count_, count_);
	std::swap(s.size_, size_);
	bs_.swap(s.bs_);
}

//--------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
stack<T>::stack(size_t s) : al_(s) {}

template<typename T>
stack<T>::~stack() { al_.destroy(al_.get(), al_.get() + al_.count()); }

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
		(allocator<T>(st.al_)).swap(al_);
	}
	return *this;
}

template <typename T>
auto stack<T>::pop() -> void {
	if (al_.empty()) std::logic_error("In pop");
	else {
		al_.destroy(al_.get() + al_.count() - 1);
	}
}

template <typename T>
auto stack<T>::top() const -> T const & {
	if (al_.empty()) std::logic_error("In top");
	else {
		return al_.get()[al_.count() - 1];
	}
}
