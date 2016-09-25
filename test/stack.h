#ifndef STACK_H
#define STACK_H
#include <iostream>

template <typename T>
class stack
{
public:
	stack(); /*noexcept*/
	stack(const stack<T> & st); /*strong*/
	~stack(); /*noexcept*/
	size_t count() const noexcept; /*noexcept*/
	void push(T const & el); /*strong*/
	stack<T> & operator = (stack<T> & st); /*noexcept*/
	void pop(); /*strong*/
	T& top() const; /*strong*/
private:
	T * array_;
	size_t array_size_;
	size_t count_;
	void swap(stack & st); /*noexcept*/
};
#include "stack.cpp"
#endif
