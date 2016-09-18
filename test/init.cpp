
#include "stack.h"
#include <iostream>
#include <catch.hpp>

SCENARIO("Stack", "[all]") 
{
	stack <bool> b;
	REQUIRE( b.count() == 0);
	stack <int> a;
	a.push(1);
	a.push(2);
	a.push(3);
	REQUIRE( a.pop() == 3);
	REQUIRE( a.count() == 2);
	REQUIRE( a.pop() == 2);
}
