#ifndef STACK_HPP
#define STACK_HPP
#include <iostream>

template <typename T>
class allocator
{
protected:
    allocator(size_t size = 0);
    ~allocator();
    auto swap(allocator & other) -> void;
    
    allocator(allocator const &) = delete;
    auto operator =(allocator const &) -> allocator & = delete;
    
    T * ptr_;
    size_t size_;
    size_t count_;
};

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
	T& top() const; /*strong*/
	void swap(stack & st); /*noexcept*/
};
#include "stack.cpp"
#endif
