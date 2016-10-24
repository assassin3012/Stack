#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

class bitset
{
public:
	explicit bitset(size_t size = 0) noexcept;
	~bitset();
	auto test(size_t index) const -> bool;
	auto set(size_t index) -> void;
	auto reset(size_t index) -> void;
	auto resize() noexcept -> void;
private:
	std::vector<bool> bit_;
}

//--------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class allocator
{
protected:
    	explicit allocator(size_t size = 0);
	allocator(allocator const & other);
    	~allocator();
	auto operator =(allocator const &) -> allocator & = delete;
    	auto swap(allocator & other) -> void;
	auto resize() -> void;
    	auto construct(T * ptr, T const & value ) -> void;
    	auto destroy( T * ptr ) -> void;
    	auto get() -> T *;
    	auto get() const -> T const *;
    	auto count() const noexcept -> size_t;
    	auto full() const noexcept -> bool;
    	auto empty() const noexcept -> bool;
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
	stack(const stack<T> & st); /*strong*/
	~stack(); /*noexcept*/
	size_t count() const noexcept; /*noexcept*/
	bool empty() const noexcept; /*noexcept*/
	void push(T const & el); /*strong*/
	stack<T> & operator = (stack<T> & st); /*strong*/
	void pop(); /*strong*/
	const T& top() const; /*strong*/
private:	
	allocator<T> al_;
};

//--------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void construct(T * ptr, T const & value) {
	new(ptr) T (value);
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

//--------------------------------------------------------------------------------------------------------------------------------------

bitset::bitset(size_t s) noexcept { bit_(s, False); }

bitset::~bitset() { bit_.clear(); }

auto bitset::test(size_t index) const -> bool { 
	if (index >= bit_.size()) throw;
	else return bit_[index];
}

auto bitset::set(size_t index) const -> void { 
	if (index >= bit_.size()) throw;
	else bit_[index] = True;
}

auto bitset::reset(size_t index) const -> void { 
	if (index >= bit_.size()) throw;
	else bit_[index] = False;
}

auto bitset::resize() noexcept -> void { 
	size_t size = bit_.size() * 2 + (bit_.size() == 0);
	bs_.resize(size, False);
}

//--------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
allocator<T>::allocator(size_t s) : ptr_(static_cast<T *>(s != 0 ? operator new(s * sizeof(T)) : nullptr)),
	size_(s), count_(0), bs_(s) {}

template <typename T>
allocator<T>::allocator(allocator const & other) : ptr_(static_cast<T *>(other.size != 0 ? operator new(other.size * sizeof(T)) : nullptr)),
	size_(other.size), count_(0), bs_(other.size) {
	for (size_t t = 0; t < other.count_; ++t) {
		this->construct(ptr_ + t, other.ptr_[t]);
	}
}

template<typename T>
allocator<T>::~allocator() { operator delete(ptr_); }

template <typename T>
auto allocator<T>::resize() -> void {
	size_t size = size_ * 2 + (size_ == 0);
	for (size_t t = 0; t < count_; ++t) {
		this->construct(ptr_ + t, other.ptr_[t]);
	}
	bs_.resize();
	size_ = size;	
}

template<typename T>
auto allocator<T>::destroy(T * ptr) -> void {
	if (ptr >= ptr_ + size_ || ptr <= ptr_) throw;
	if (bs_.test(ptr)) {
		ptr->~T();
		bs_.reset(ptr);
		--count_;
	}
}

template <typename T>
auto allocator<T>::construct(T * ptr, T const & value ) -> void {
	if (ptr >= ptr_ + size_ || ptr <= ptr_) throw;
	if (bs_.test(ptr)) {
		this->destroy(ptr);
	}
	new(ptr) T (value);
	bs_.set(ptr);
	++count_
}

template<typename T>
auto allocator<T>::get() -> T * {return ptr_; }

template<typename T>
auto allocator<T>::get() const -> T const * { return ptr_; }

template<typename T>
auto allocator<T>::count() const noexcept -> size_t { return count_; }

template<typename T>
auto allocator<T>::full() const noexcept -> bool { return (count_ == size_); }

template<typename T>
auto allocator<T>::empty() const noexcept -> bool { return (count_ == 0); }

template <typename T>
auto allocator<T>::swap(allocator& s) -> void {
	std::swap(s.ptr_, ptr_);
	std::swap(s.count_, count_);
	std::swap(s.size_, size_);
	std::swap(s.bs_, bs_);
}

//--------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
stack<T>::stack(size_t s) : al_(s) {}

template<typename T> 
stack<T>::~stack() { destroy(al_.get(), al_.get() + al_.count_(); }

template <typename T>
bool stack<T>::empty() const noexcept { return (al_.count_() == 0); }

template <typename T>
stack<T>::stack(const stack& st) : al_(st.al_) {}

template <typename T> 
size_t stack<T>::count() const noexcept { return al_.count_(); }

template <typename T> 
void stack<T>::push(T const & el) {
	if (al_.full() || al_.empty()) {
		al_.resize();
	}
	al_.construct(al_.get() + al_.count(), el);
}

template <typename T>
stack<T> & stack<T>::operator = (stack<T> & st) {
	if (this != &st) {
		(st.al_).swap(*this->al_);
	}
	return *this;
}

template <typename T>
void stack<T>::pop() {
	if (al_.empty()) throw;
	else al_.destroy(al_.get() + al_.count() - 1);
}

template <typename T>
const T& stack<T>::top() const {
	if (al_.empty()) throw;
	else return al_.get()[al_.count() - 1];
}
}
