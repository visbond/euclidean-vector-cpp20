// tests euclidean vector class methods and operators
#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>

TEST_CASE("operator[] getting tests") {
	SECTION("Single dimensional default vector") {
		auto ev = comp6771::euclidean_vector(); // single-dimension, value 0
		double d = ev[0];
		CHECK(d == 0);
	}

	SECTION("Multi dimensional vector and arithmetic operations") {
		auto ev = comp6771::euclidean_vector{2.3, 2.4, 2.5, 2.6}; // single-dimension, value 0
		double d1 = ev[1] - ev[0];
		double d2 = ev[3] - ev[2];
		CHECK(d1 == d2); // should be 0.1 in both
	}

	// this test is aborted due to the assert failing, but it shows the assert is working
	// SECTION("Error on invalid dimension") {
	// 	auto ev = comp6771::euclidean_vector();
	// 	double d = ev[6];   // try to access beyond range
	// 	(void)d;
	// }
}

TEST_CASE("operator[] setting tests") {
	SECTION("Single dimensional default vector, set with rvalue") {
		auto ev = comp6771::euclidean_vector(); // single-dimension, value 0
		ev[0] = 3.0; // setting directly with integer, rvalue
		CHECK(ev[0] == 3.0); // also checks getter implicitly
		CHECK(fmt::format("{}", ev) == "[3]");
	}

	SECTION("Multi dimensional vector, set with lvalue") {
		auto ev = comp6771::euclidean_vector{2.3, 2.4, 2.5, 2.6}; // single-dimension, value 0
		double d = 2.3;
		ev[1] = d; // setting with named variable, lvalue
		CHECK(ev[0] == ev[1]); // should be 2.3 in both
	}

	// this test is aborted due to the assert failing, but it shows the assert works
	// SECTION("Error on invalid dimension") {
	// 	auto ev = comp6771::euclidean_vector();
	// 	ev[6] = 7;
	// }
}

TEST_CASE("Unary operators") {
	SECTION("operator+ with multidimensional vector") {
		auto ev1 = comp6771::euclidean_vector{1, 2};
		auto ev2 = +ev1;
		CHECK(ev1 == ev2);
	}

	SECTION("operator+ with single dimensional default vector") {
		auto ev1 = comp6771::euclidean_vector();
		auto ev2 = +ev1;
		CHECK(ev1 == ev2);
	}

	SECTION("operator+ with zero-dimensional vector") {
		auto ev1 = comp6771::euclidean_vector(0);
		auto ev2 = +ev1;
		CHECK(ev1 == ev2);
	}

	SECTION("operator- negation equivalence with multidimensional vectors") {
		auto const actual = comp6771::euclidean_vector{-1, 2, -3};
		auto const expected = comp6771::euclidean_vector{1, -2, 3};
		CHECK(expected == -actual);
	}

	SECTION("operator- double negation") {
		auto ev1 = comp6771::euclidean_vector{-1, 2, -3};
		CHECK(ev1 == -(-ev1));
	}

	SECTION("operator- with zero-dimensional vector") {
		auto ev1 = comp6771::euclidean_vector(0);
		auto ev2 = -ev1;
		CHECK(ev1 == ev2); // both should be empty. If they aren't, then one should be negative of
		                   // other and check should fail
	}
}

TEST_CASE("at() test for reading and writing") {
	SECTION("Ensuring at and [] giving same output") {
		auto ev = comp6771::euclidean_vector{1, 2, 3};
		CHECK(ev[1] == ev.at(1));
	}
	SECTION("Writing using at and reading with [] giving same output") {
		auto ev = comp6771::euclidean_vector{1, 2, 3};
		ev.at(1) = 4;
		CHECK(ev[1] == 4);
	}
	SECTION("Reading and writing using at giving same output") {
		auto ev = comp6771::euclidean_vector{1, 2, 3};
		ev.at(1) = 4;
		CHECK(ev.at(1) == 4);
	}
}

TEST_CASE("Compound addition (operator+=) tests") {
	SECTION("Single dimensional 0-vector on each side") {
		auto ev1 = comp6771::euclidean_vector();
		auto ev2 = comp6771::euclidean_vector();
		ev1 += ev2;
		CHECK(ev1[0] == 0);
		CHECK(fmt::format("{}", ev1) == "[0]");
	}
	SECTION("Multi-dimensional vector on each side") {
		auto ev1 = comp6771::euclidean_vector{1, 2, 3};
		auto ev2 = comp6771::euclidean_vector{4, 3, 2};
		ev1 += ev2;
		CHECK(ev1[0] == 5);
		CHECK(fmt::format("{}", ev1) == "[5 5 5]");
	}
	SECTION("Zero-dimensional vector on each side") {
		auto ev1 = comp6771::euclidean_vector(0);
		auto ev2 = comp6771::euclidean_vector(0);
		ev1 += ev2;
		CHECK(fmt::format("{}", ev1) == "[]");
	}
	// not testing vectors of different dimensions as would throw exception and abort test
}

TEST_CASE("Compound subtraction (operator-=) tests") {
	SECTION("Single dimensional 0-vector on each side") {
		auto ev1 = comp6771::euclidean_vector();
		auto ev2 = comp6771::euclidean_vector();
		ev1 -= ev2;
		CHECK(ev1[0] == 0);
		CHECK(fmt::format("{}", ev1) == "[0]");
	}
	SECTION("Multi-dimensional vector on each side") {
		auto ev1 = comp6771::euclidean_vector{1, 2, 3};
		auto ev2 = comp6771::euclidean_vector{4, 3, 2};
		ev1 -= ev2;
		CHECK(ev1[0] == -3);
		CHECK(fmt::format("{}", ev1) == "[-3 -1 1]");
	}
	SECTION("Zero-dimensional vector on each side") {
		auto ev1 = comp6771::euclidean_vector(0);
		auto ev2 = comp6771::euclidean_vector(0);
		ev1 -= ev2;
		CHECK(fmt::format("{}", ev1) == "[]");
	}
	// not testing vectors of different dimensions as would throw exception and abort test
}

TEST_CASE("Compound operator *= (scalar multiplication) tests") {
	SECTION("Lvalue scalar with single-dimensional vector") {
		auto s = 5.0;
		auto result = comp6771::euclidean_vector{4.0};
		result *= s;
		CHECK(fmt::format("{}", result) == "[20]");
	}

	SECTION("Rvalue scalar with multi-dimensional vector") {
		auto result = comp6771::euclidean_vector{4.0, 2.5};
		result *= 2.5;
		CHECK(fmt::format("{}", result) == "[10 6.25]");
	}

	SECTION("Multiplication of non-zero scalar with zero-dimensional vector") {
		auto result = comp6771::euclidean_vector(0);
		result *= 3.0;
		CHECK(fmt::format("{}", result) == "[]");
	}

	SECTION("Multiplication of zero scalar with non-zero vector") {
		auto result = comp6771::euclidean_vector{3, 4, 5};
		result *= 0.0;
		CHECK(fmt::format("{}", result) == "[0 0 0]");
	}
}

TEST_CASE("Compound operator /= (scalar division) tests") {
	SECTION("Lvalue scalar with single-dimensional vector") {
		auto s = 5.0;
		auto result = comp6771::euclidean_vector{5.5};
		result /= s;
		CHECK(fmt::format("{}", result) == "[1.1]");
	}

	SECTION("Rvalue scalar with multi-dimensional vector") {
		auto result = comp6771::euclidean_vector{6.25, 2.5};
		result /= 2.5;
		CHECK(fmt::format("{}", result) == "[2.5 1]");
	}

	SECTION("Division of zero-dimensional vector with non-zero scalar") {
		auto result = comp6771::euclidean_vector(0);
		result /= 3.0;
		CHECK(fmt::format("{}", result) == "[]");
	}

	// // the following throws exception as expected, so has been 'tested'
	// SECTION("Division by 0") { // should throw exception and abort test
	// 	auto ev = comp6771::euclidean_vector();
	// 	ev /= 0;
	// 	CHECK(fmt::format("{}", ev) == "[]");
	// }
}

TEST_CASE("List conversion function") {
	SECTION("Multi dimensional vector, match elements") {
		auto evector = comp6771::euclidean_vector{16.24, 33, 22.5};
		auto list = std::list<double>(evector);
		CHECK(list.front() == evector[0]);
		CHECK(list.back() == evector[evector.dimensions() - 1]);
	}

	SECTION("Single dimensional vector") {
		auto evector = comp6771::euclidean_vector();
		auto list = std::vector<double>(evector);
		CHECK(list.front() == evector[0]);
		CHECK(list.back() == evector[0]);
		CHECK(list.size() == 1);
	}

	SECTION("Zero dimensional vector") {
		auto evector = comp6771::euclidean_vector(0);
		auto list = std::vector<double>(evector);
		CHECK(list.empty());
	}
}

TEST_CASE("Vector conversion function") {
	SECTION("Multi dimensional vector, match elements") {
		auto evector = comp6771::euclidean_vector{6.25, 3, 2.5};
		auto vvector = std::vector<double>(evector);
		CHECK(vvector.front() == evector[0]);
		CHECK(vvector[1] - vvector[2] == 0.5);
		CHECK(vvector.back() == evector[evector.dimensions() - 1]);
	}

	SECTION("Single dimensional vector") {
		auto evector = comp6771::euclidean_vector();
		auto vvector = std::vector<double>(evector);
		CHECK(vvector.front() == evector[0]);
		CHECK(vvector.back() == evector[0]);
		CHECK(vvector.size() == 1);
	}

	SECTION("Zero dimensional vector") {
		auto evector = comp6771::euclidean_vector(0);
		auto vvector = std::vector<double>(evector);
		CHECK(vvector.empty());
	}
}
