#pragma once
#include <iostream>

template <typename T>
class allocator
{
protected:
    	allocator(size_t size = 0);
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

    	T * ptr_;
    	size_t size_;
    	size_t count_;
};

//-----------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class stack : private allocator<T>
{
public:
	stack(size_t size = 0); /*noexcept*/
	stack(const stack<T> & st); /*strong*/
	~stack(); /*noexcept*/
	size_t count() const noexcept; /*noexcept*/
	bool empty() const noexcept; /*noexcept*/
	void push(T const & el); /*strong*/
	stack<T> & operator = (stack<T> & st); /*strong*/
	void pop(); /*strong*/
	const T& top() const; /*strong*/
};

//-----------------------------------------------------------------------------------------------------------------------------------------

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

//--------------------------------------------------------------------------------------------------------------------------------------
	
template <typename T>
allocator<T>::allocator(size_t s) : ptr_(static_cast<T *>(s != 0 ? operator new(s * sizeof(T)) : nullptr)),
	size_(s), count_(0) {}

template <typename T>
allocator<T>::allocator(allocator const & other) : ptr_(static_cast<T *>(other.size != 0 ? operator new(other.size * sizeof(T)) : nullptr)),
	size_(other.size), count_(0) {
	for (size_t t = 0; t < other.count_; ++t) construct(ptr_ + t, other.ptr_[t]);
	count_ = other.count_;	
}

template<typename T>
allocator<T>::~allocator() { operator delete(ptr_); }

template <typename T>
auto allocator<T>::resize() -> void {
	if (size_ == 0) { size_ = 1; }
	size_t size = size_ * 2;
	stack<T> ar(size);
	for (size_t t = 0; t < count_; ++t) construct(ar.ptr_ + t, ptr_[t]);
	std::swap(ar.ptr_, ptr_);
	size_ = size;	
}

template<typename T>
auto allocator<T>::destroy(T * ptr) -> void {
	if (ptr >= ptr_ + size_ || ptr <= ptr_) throw;
	if (ptr_ + count_ <= ptr) {
		for (size_t t = 0; ptr_ + count_ + t != ptr; --t) {
			(ptr_ + count_ + t)->~T();
		}
		ptr->~T();
		count_ = ptr - ptr_;
	}
}

template <typename T>
auto allocator<T>::construct(T * ptr, T const & value ) -> void {
	if (ptr >= ptr_ + size_ || ptr <= ptr_) throw;
	if (ptr_ + count_ >= ptr) {
		this->destroy(ptr);
		new(ptr) T (value);
	}
	else {
		for (size_t t = 0; ptr_ + count_ + t != ptr; ++t) {
			new(ptr_ + count_ + t) T (value);
		}
		new(ptr) T (value);
		count_ = ptr - ptr_;
	}
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
}

//--------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
stack<T>::stack(size_t s) : allocator<T>(s) {}

template<typename T> 
stack<T>::~stack() { destroy(this->ptr_, this->ptr_ + this->count_); }

template <typename T>
bool stack<T>::empty() const noexcept { return (this->count_ == 0); }

template <typename T>
stack<T>::stack(const stack& st) : allocator<T>(st.size_) {
	if (st.count_ != 0) {
		stack<T> ar(st.size_);
		for (size_t t = 0; t < st.count_; ++t) {
                       construct(ar.ptr_ + t, st.ptr_[t]);
                       ++ar.count_;
                }
		std::swap(ar.ptr_, this->ptr_);
	}
	this->count_ = st.count_;
}

template <typename T> /*noexcept*/
size_t stack<T>::count() const noexcept { return this->count_; }

template <typename T> /*strong*/
void stack<T>::push(T const & el) {
	if (this->size_ == this->count_) {
		if (this->size_ == 0) { this->size_ = 1; }
		size_t size = this->size_ * 2;
		stack<T> ar(size);
		for (size_t t = 0; t < this->count_; ++t) construct(ar.ptr_ + t, this->ptr_[t]);
		std::swap(ar.ptr_, this->ptr_);
		this->size_ = size;
	}
	construct(this->ptr_ + this->count_, el);
        ++this->count_;
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
	if (this->count_ != 0) {
		--this->count_;
	}
	else throw;
}

template <typename T> /*strong*/
const T& stack<T>::top() const {
	if (this->count_ != 0) {
		return this->ptr_[this->count_ - 1];
	}
	else throw;
}
