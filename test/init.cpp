
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
	REQUIRE(A.count() == 1);
	REQUIRE(A.top() == 4);
	A.pop();
	REQUIRE(A.count() == 0);
}
SCENARIO("operator =","[=]"){
	stack<int> A;
	A.push(221);
	stack<int> B = A; 
	REQUIRE(B.count() == 1);
	REQUIRE(B.top() == 221);
}
SCENARIO("Stack push, top and pop #2", "[PTP#2]") {
	stack<int> A;
	int m=404;
	for (int i=0; i<10; ++i){
		A.push(m+i);
	}
	bool A=True;
	for (int i=0; i<10; ++i){
		if (A.count()!=10-i) { A=False;}
		if (A.top!=m-i) { A=False; }
		A.pop;
	}
	REQUIRE(A==True);
}
