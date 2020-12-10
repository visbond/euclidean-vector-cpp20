// tests for friend and utility functions

#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>

TEST_CASE("Boolean operator== tests") {
	SECTION("Single dimensional default vectors") {
		auto ev1 = comp6771::euclidean_vector();
		auto ev2 = comp6771::euclidean_vector();
		auto result = (ev1 == ev2);
		CHECK(result == true);
	}

	SECTION("Zero dimensional vectors") {
		auto ev1 = comp6771::euclidean_vector(0);
		auto ev2 = comp6771::euclidean_vector(0);
		auto result = (ev1 == ev2);
		CHECK(result == true);
	}

	SECTION("Multi-dimensional equal vectors with different decimal notations") {
		auto ev1 = comp6771::euclidean_vector{1.0, 2, 3.0000};
		auto ev2 = comp6771::euclidean_vector{1, 2.0, 3.0};
		auto result = (ev1 == ev2);
		CHECK(result == true);
	}

	SECTION("Multi-dimensional vectors unequal in one element") {
		auto ev1 = comp6771::euclidean_vector{1.1, 2, 3};
		auto ev2 = comp6771::euclidean_vector{1, 2, 3};
		auto result = (ev1 == ev2);
		CHECK(result == false);
	}

	SECTION("Multi-dimensional vectors unequal in all elements") {
		auto ev1 = comp6771::euclidean_vector{11, 12, 13};
		auto ev2 = comp6771::euclidean_vector{1, 2, 3};
		auto result = (ev1 == ev2);
		CHECK(result == false);
	}

	SECTION("Mismatched dimensions") {
		auto ev1 = comp6771::euclidean_vector{};
		auto ev2 = comp6771::euclidean_vector{1, 2, 3};
		auto result = (ev1 == ev2);
		CHECK(result == false);
	}
}

TEST_CASE("Boolean operator!= tests") {
	// these are just complements of the == tests
	SECTION("Single dimensional default vectors") {
		auto ev1 = comp6771::euclidean_vector();
		auto ev2 = comp6771::euclidean_vector();
		auto result = (ev1 != ev2);
		CHECK(result == false);
	}

	SECTION("Zero dimensional vectors") {
		auto ev1 = comp6771::euclidean_vector(0);
		auto ev2 = comp6771::euclidean_vector(0);
		auto result = (ev1 != ev2);
		CHECK(result == false);
	}

	SECTION("Multi-dimensional equal vectors with different decimal notations") {
		auto ev1 = comp6771::euclidean_vector{1.0, 2, 3.0000};
		auto ev2 = comp6771::euclidean_vector{1, 2.0, 3.0};
		auto result = (ev1 != ev2);
		CHECK(result == false);
	}

	SECTION("Multi-dimensional vectors unequal in one element") {
		auto ev1 = comp6771::euclidean_vector{1.1, 2, 3};
		auto ev2 = comp6771::euclidean_vector{1, 2, 3};
		auto result = (ev1 != ev2);
		CHECK(result == true);
	}

	SECTION("Multi-dimensional vectors unequal in all elements") {
		auto ev1 = comp6771::euclidean_vector{11, 12, 13};
		auto ev2 = comp6771::euclidean_vector{1, 2, 3};
		auto result = (ev1 != ev2);
		CHECK(result == true);
	}

	SECTION("Mismatched dimensions") {
		auto ev1 = comp6771::euclidean_vector{};
		auto ev2 = comp6771::euclidean_vector{1, 2, 3};
		auto result = (ev1 != ev2);
		CHECK(result == true);
	}
}

TEST_CASE("Scalar multiplication tests") {
	SECTION("Lvalue scalar to left of single-dimensional vector") {
		auto s = 5.0;
		auto ev = comp6771::euclidean_vector{4.0};
		auto result = s * ev;
		CHECK(fmt::format("{}", result) == "[20]");
	}

	SECTION("Rvalue scalar to right of multi-dimensional vector") {
		auto ev = comp6771::euclidean_vector{4.0, 2.5};
		auto result = ev * 2.5;
		CHECK(fmt::format("{}", result) == "[10 6.25]");
	}

	SECTION("Commutativity check") {
		auto ev = comp6771::euclidean_vector{4.0, 2.5};
		CHECK(2.5 * ev == ev * 2.5);
	}

	SECTION("Multiplication of non-zero scalar with zero-dimensional vector") {
		auto ev = comp6771::euclidean_vector(0);
		auto result = 3.0 * ev;
		CHECK(fmt::format("{}", result) == "[]");
	}

	SECTION("Multiplication of zero scalar with non-zero vector") {
		auto ev = comp6771::euclidean_vector{3, 4, 5};
		auto result = 0.0 * ev;
		CHECK(fmt::format("{}", result) == "[0 0 0]");
	}
}

TEST_CASE("Scalar division tests") {
	SECTION("Lvalue scalar with single-dimensional vector") {
		auto s = 5.0;
		auto ev = comp6771::euclidean_vector{5.5};
		auto result = ev / s;
		CHECK(fmt::format("{}", result) == "[1.1]");
	}

	SECTION("Rvalue scalar with multi-dimensional vector") {
		auto ev = comp6771::euclidean_vector{6.25, 2.5};
		auto result = ev / 2.5;
		CHECK(fmt::format("{}", result) == "[2.5 1]");
	}

	SECTION("Division of zero-dimensional vector with non-zero scalar") {
		auto ev = comp6771::euclidean_vector(0);
		auto result = ev / 3.0;
		CHECK(fmt::format("{}", result) == "[]");
	}

	// // the following throws exception as expected, so has been 'tested'
	// // Catch2 allows skipping exceptions by using --nothrow switch, but that's not in spec
	// SECTION("Division by 0") { // should throw exception and abort test
	// 	auto ev = comp6771::euclidean_vector();
	// 	auto result = ev / 0;
	// 	CHECK(fmt::format("{}", result) == "[]");
	// }
}

TEST_CASE("Ostream overload tests") {
	// TO_DO add fmt comparison check (see "basic test" code below from spec)
	SECTION("3-element vector") {
		auto ev = comp6771::euclidean_vector{3.1, 3.2, 3.3};
		CHECK(fmt::format("{}", ev) == "[3.1 3.2 3.3]"); // this needs ostream overload implemented
	}

	SECTION("Test properly formatted output of single-dimension vector") {
		auto ev = comp6771::euclidean_vector(); // default constructed, 1 dimension, value 0
		CHECK(fmt::format("{}", ev) == "[0]");
	}

	SECTION("Test properly formatted output of zero-dimension vector") {
		auto ev = comp6771::euclidean_vector(0); // explicitly constructed to be 0-dimension
		CHECK(fmt::format("{}", ev) == "[]");
	}
}

TEST_CASE("Binary operator+ tests") {
	SECTION("0-dimension vectors") {
		auto ev1 = comp6771::euclidean_vector(0);
		auto ev2 = comp6771::euclidean_vector(0);
		auto ev3 = ev1 + ev2;
		CHECK(fmt::format("{}", ev3) == "[]");
	}

	SECTION("Single-dimension vectors") {
		auto ev1 = comp6771::euclidean_vector(); // default constructed with 1 dimension
		auto ev2 = comp6771::euclidean_vector();
		auto ev3 = ev1 + ev2;
		CHECK(fmt::format("{}", ev3) == "[0]");
	}

	SECTION("Two 3-dimensional vectors") {
		auto ev1 = comp6771::euclidean_vector{3.0, 4.0, 5.0};
		auto ev2 = comp6771::euclidean_vector{1, 2, 3};
		auto ev3 = ev1 + ev2;
		CHECK(fmt::format("{}", ev3) == "[4 6 8]");
	}

	// SECTION("Mismatched dimension vectors") { // can't be properly tested as exception gets thrown
	// by class itself, but exception was thrown as expected
	//  auto ev1 = comp6771::euclidean_vector(); // default constructed with 1 dimension
	// 	auto ev2 = comp6771::euclidean_vector{1, 2}; // two dimensions
	// 	auto ev3 = ev1 + ev2;
	// }
}

TEST_CASE("Compound test with fmt::format, needs both ostream and binary operator+ implemented") {
	auto const a1 = comp6771::euclidean_vector(3, 3.0);
	auto const a2 = comp6771::euclidean_vector(3, 3.0);
	auto const a3 = comp6771::euclidean_vector(3, 3.0);
	CHECK(fmt::format("{}", a1 + a2 + a3) == "[9 9 9]"); // this requires ostream and + implemented
}

TEST_CASE("Binary operator- tests") {
	SECTION("0-dimension vectors") {
		auto ev1 = comp6771::euclidean_vector(0);
		auto ev2 = comp6771::euclidean_vector(0);
		auto ev3 = ev1 - ev2;
		CHECK(fmt::format("{}", ev3) == "[]");
	}

	SECTION("Single-dimension vectors") {
		auto ev1 = comp6771::euclidean_vector(); // default constructed with 1 dimension
		auto ev2 = comp6771::euclidean_vector();
		auto ev3 = ev1 - ev2;
		CHECK(fmt::format("{}", ev3) == "[0]");
	}

	SECTION("Two 3-dimensional vectors") {
		auto ev1 = comp6771::euclidean_vector{3.0, 4.0, 5.0};
		auto ev2 = comp6771::euclidean_vector{1, 2, 3};
		auto ev3 = ev1 - ev2;
		CHECK(fmt::format("{}", ev3) == "[2 2 2]");
	}

	// SECTION("Mismatched dimension vectors") { // can't be properly tested as get exception thrown
	// in between, but exception was thrown as it should be
	// 	auto ev1 = comp6771::euclidean_vector();
	// 	auto ev2 = comp6771::euclidean_vector{1, 2};
	// 	auto ev3 = ev1 - ev2;
	// }
}

// utility functions

TEST_CASE("Dot product") {
	SECTION("Two different 2-dimensional vectors") {
		auto ev1 = comp6771::euclidean_vector{10, 20};
		auto ev2 = comp6771::euclidean_vector{3, 4};
		double d = comp6771::dot(ev1, ev2);
		CHECK(d == 110);
	}
	SECTION("Two default 1-dimensional vectors") {
		auto ev1 = comp6771::euclidean_vector();
		auto ev2 = comp6771::euclidean_vector();
		double d = comp6771::dot(ev1, ev2);
		CHECK(d == 0);
	}
	SECTION("Two 5-dimensional vectors with one all 1 (dot product should add all values of "
	        "first)") {
		auto ev1 = comp6771::euclidean_vector{10, 20, 30, 40, 50};
		auto ev2 = comp6771::euclidean_vector{1, 1, 1, 1, 1};
		double d = comp6771::dot(ev1, ev2);
		CHECK(d == 150);
	}
	SECTION("Two 3-dimensional vectors with one all 0 (dot product should be 0)") {
		auto ev1 = comp6771::euclidean_vector{10, 20, 30, 40, 50};
		auto ev2 = comp6771::euclidean_vector{0, 0, 0, 0, 0};
		double d = comp6771::dot(ev1, ev2);
		CHECK(d == 0);
	}
}

TEST_CASE("Euclidean norm") {
	SECTION("Default vector") {
		auto ev = comp6771::euclidean_vector();
		auto d = comp6771::euclidean_norm(ev);
		CHECK(d == 0);
	}
	SECTION("Two-dimensional vector (Pythagorean triplet 3,4,5") {
		auto ev = comp6771::euclidean_vector{3, 4};
		auto d = comp6771::euclidean_norm(ev);
		CHECK(d == 5);
	}
	SECTION("Three-dimensional vector") {
		auto ev = comp6771::euclidean_vector{1, 2, 3};
		auto d = comp6771::euclidean_norm(ev);
		CHECK(d > 3.73);
		CHECK(d < 3.75); // to allow for floating point differences (Catch didn't allow && inside a
		                 // single expression)
	}
	SECTION("Four-dimensional zero vector") {
		auto ev = comp6771::euclidean_vector{0, 0, 0, 0};
		auto d = comp6771::euclidean_norm(ev);
		CHECK(d == 0);
	}
	SECTION("Five-dimensional unit vector, in 3rd dimension") {
		auto ev = comp6771::euclidean_vector{0, 0, 1, 0, 0};
		auto d = comp6771::euclidean_norm(ev);
		CHECK(d == 1);
	}
	SECTION("Very large vector") {
		auto ev = comp6771::euclidean_vector(1000000, 100000);
		auto d = comp6771::euclidean_norm(ev);
		// std::cout << d;
		CHECK(d > 1000000);
	}
}

TEST_CASE("Unit vector") {
	SECTION("Dimensionality of vector and its unit vector should be same") {
		auto ev = comp6771::euclidean_vector{2.2, 3.3, 4.4};
		auto uv = comp6771::unit(ev);
		CHECK(uv.dimensions() == ev.dimensions());
	}

	SECTION("Unit vector of unit vector should be same as itself") {
		auto ev = comp6771::euclidean_vector{0, 0, 1, 0, 0}; // five-dimensional unit vector, normal
		                                                     // from the third dimension
		auto u = comp6771::unit(ev);
		CHECK(ev == u);
		CHECK(u[2] == 1);
	}

	SECTION("Two identical vectors") {
		auto a = comp6771::euclidean_vector{5.6, 6.5};
		auto b = comp6771::euclidean_vector{5.6, 6.5};
		auto ua = comp6771::unit(a);
		auto ub = comp6771::unit(b);
		CHECK(ua == ub);
	}

	SECTION("Checking exactness of calculation") {
		auto ev = comp6771::euclidean_vector{3, 4};
		auto uv = comp6771::unit(ev);
		CHECK(uv[0] == 0.6);
		CHECK(uv[1] == 0.8);
	}
}