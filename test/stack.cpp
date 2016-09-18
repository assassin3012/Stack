#ifndef STACK_CPP
#define STACK_CPP
#include "stack.h"

using namespace std;

template <typename T>
stack<T>::stack() : array_(nullptr), array_size_(0), count_(0) {}
template <typename T>
stack<T>::~stack() { delete[] array_; }	
template <typename T>
stack<T>::stack(const stack<T> & st) : array_size_(st.array_size_), count_(st.count_) {		
   array_ = new T[array_size_];		
   for (size_t i = 0; i < count_; i++) {			
   array_[i] = st.array_[i];		
   }
}	
template <typename T>
size_t stack<T>::count() const noexcept { return count_; }
template <typename T>
void stack<T>::push(T const & el) {		
   if (array_size_ == count_) {
      if (array_size_==0) { array_size_=1; }			
      array_size_ = array_size_*2;			
      T * ar = new T[array_size_];			
      for (size_t i = 0; i < count_; i++) { ar[i] = array_[i]; }			
      delete[] array_;			
      array_ = ar;
   }		
   array_[count_++] = el;	
}
template <typename T>
stack<T> & stack<T>::operator = (stack<T> & st) {
	if (this != &st) {			
      delete[] array_;			
      array_size_ = st.array_size_;			
      count_ = st.count_;			
      array_ = new T[array_size_];			
      for (int i = 0; i < count_; i++) {				
         array_[i] = st.array_[i];
       }
   }		
   return *this;	
}
template <typename T>
T stack<T>::pop() { 
   if (count_ == 0) {
      throw std::logic_error("Empty");
      }		
   return array_[--count_];
}
template <typename T>
void stack<T>::swap(stack & st) noexcept {
	swap(array_size_, st.array_size_);
	swap(array_, st.array_);
	swap(count_, st.count_);
}
#endif
 
