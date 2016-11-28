#include "stack.hpp"
#include <iostream>
#include <catch.hpp>

SCENARIO("Stack threads", "threads") {
	stack<int> A;
	A.push(13);
	A.push(666);
	std::thread t1(&stack<int>::push, &A, 404);
	t1.join();
	REQUIRE(A.count() == 3);
	std::thread t2(&stack<int>::pop, &A);
	t2.join();
	REQUIRE(*A.top() == 666);
	REQUIRE(A.count() == 2);
}
SCENARIO("Stack init, count and empty", "[init_count]"){
	stack<bool> A;
	REQUIRE(A.count() == 0);
	REQUIRE(A.empty());
}
SCENARIO("Stack push, top and pop", "[PTP]") {
	stack<int> A;
	A.push(4);
	A.push(404);
	REQUIRE(A.count() == 2);
	REQUIRE(*A.pop() == 404);
	REQUIRE(A.count() == 1);
	REQUIRE(*A.pop() == 4);
	REQUIRE(A.empty());
}
SCENARIO("Stack operator =","[operator =]"){
	stack<int> A;
	A.push(221);
	stack<int> B = A; 
	REQUIRE(B.count() == 1);
	REQUIRE(*B.pop() == 221);
	stack<int> A1;
	stack<int> B1;
	A1.push(221);
	B1 = A1; 
	REQUIRE(B.count() == 1);
	REQUIRE(*B.pop() == 221);
}
