#include <catch2/catch_test_macros.hpp>
// This file just exists to see if the automated tests as specified in ci.yml  is working

unsigned factorial(unsigned num)
{
	return num <= 1 ? 1 : factorial(num - 1) * num;
}

TEST_CASE("Testing Factorial", "[factorial]")
{
	REQUIRE(factorial(0) == 1);
	REQUIRE(factorial(1) == 1);
	REQUIRE(factorial(2) == 2);
	REQUIRE(factorial(3) == 6);

}