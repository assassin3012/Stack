#include "stack.hpp"
#include <iostream>
#include <catch.hpp>

SCENARIO("Stack init, count", "[init_count]"){
	stack<bool> A;
	REQUIRE(A.count() == 0);
	REQUIRE(A.empty());
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
	REQUIRE(A.empty());
}
SCENARIO("Stack operator =","[oper]"){
	stack<int> A;
	A.push(221);
	stack<int> B = A; 
	REQUIRE(B.count() == 1);
	REQUIRE(B.top() == 221);
stack<int> A1;
stack<int> B1;
	A1.push(221);
	B1 = A1; 
	REQUIRE(B.count() == 1);
	REQUIRE(B.top() == 221);
}
