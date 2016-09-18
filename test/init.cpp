
#include "stack.h"
#include <iostream>
#include <catch.hpp>

SCENARIO("Stack init and count", "[init_count]"){
	stack<bool> A;
	REQUIRE(A.count() == 0);
}
SCENARIO("Stack push, top and pop", "[PTP]") {
	stack<int> A;
	A.push(4);
	A.push(404);
	REQUIRE(A.count() == 2);
	REQUIRE(A.top() == 404);
	A.pop();
	REQUIRE(A.count() == 1);
	REQUIRE(A.top() == 4);
	A.pop();
	REQUIRE(A.count() == 0);
}
SCENARIO("Stack operator =","[operator =]"){
	stack<int> A;
	A.push(221);
	stack<int> B = A; 
	REQUIRE(B.count() == 1);
	REQUIRE(B.top() == 221);
}
