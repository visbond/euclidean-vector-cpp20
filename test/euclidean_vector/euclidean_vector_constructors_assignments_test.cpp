// tests constructors and move and copy assignments
#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>

TEST_CASE("Basic constructor test") { // this is the constructor with (dimensions, magnitude)
	                                   // arguments
	SECTION("Basic constructor with const objects and rvalue arguments") {
		auto const a1 = comp6771::euclidean_vector(3, 3.0);
		auto const a2 = comp6771::euclidean_vector(4, 1.0);
		CHECK(a1.dimensions() == 3);
		CHECK(a2.dimensions() == 4);
		CHECK((a1.at(0) - a2.at(3)) == 2.0); // 3.0 - 1.0
	}

	SECTION("With const lvalue arguments") {
		auto const dims = 4;
		auto const mag = 2.3;
		auto const ev = comp6771::euclidean_vector(dims, mag);
		CHECK(ev.dimensions() == dims);
		CHECK(ev.at(0) == mag);
	}

	SECTION("Verifying norm and dot product") {
		auto const ev = comp6771::euclidean_vector(4, 1.0);
		CHECK(comp6771::dot(ev, ev) == 4);
		CHECK(comp6771::euclidean_norm(ev) == 2);
	}

	SECTION("Construct very large object") {
		auto const ev = comp6771::euclidean_vector(100000, 4.0);
		CHECK(ev.dimensions() == 100000);
		CHECK(ev[0] == ev[ev.dimensions() - 1]);
	}
}

TEST_CASE("Constructors that delegate basic constructor") {
	// since the above has many tests, those that simply delegate it have very basic tests
	SECTION("Default constructor") {
		auto ev = comp6771::euclidean_vector();
		CHECK(ev.dimensions() == 1);
		CHECK(ev[0] == 0);
	}

	SECTION("Single argument constructor with 3 dimensions") {
		auto const ev = comp6771::euclidean_vector(3);
		CHECK(ev.dimensions() == 3);
		CHECK(ev[2] == 0);
	}

	SECTION("Zero dimensions") {
		auto ev = comp6771::euclidean_vector(0);
		CHECK(ev.dimensions() == 0);
	}
}

TEST_CASE("std::vector iterator constructor") {
	SECTION("Match elements and length") {
		auto v = std::vector<double>{1.0, 2.0, 3.4, 5.6};
		auto ev = comp6771::euclidean_vector(v.begin(), v.end());
		CHECK(v.front() == ev.at(0));
		CHECK(v.back() == ev.at(ev.dimensions() - 1)); // this matches length automatically, since all
		                                               // elements are unique
	}
	SECTION("Convert to vector again and match") {
		auto v = std::vector<double>{7.6, 7.8, 7.7};
		auto ev = comp6771::euclidean_vector(v.begin(), v.end());
		auto v2 = std::vector<double>(ev); // uses type conversion function
		auto ev2 = comp6771::euclidean_vector(v2.begin(), v2.end());
		CHECK(v == v2);
		CHECK(ev == ev2); // this uses the class operator== overload (has been tested already)
	}
}

TEST_CASE("Copy constructor tests") {
	SECTION("Using basic constructor and const input to copy constructor") {
		auto const a1 = comp6771::euclidean_vector(3, 3.0);
		auto a2 = comp6771::euclidean_vector(a1);
		CHECK(a1 == a2);
	}

	SECTION("Vector iterator to construct first one, then copy") {
		auto v = std::vector<double>{1.0, 2.0, 3.4, 5.6};
		auto ev = comp6771::euclidean_vector(v.begin(), v.end());
		auto ev2 = comp6771::euclidean_vector(ev);
		CHECK(ev == ev2);
	}
}

TEST_CASE("Move constructor test") {
	auto ev = comp6771::euclidean_vector(3, 5.0); // note that if ev is const, move below won't work
	auto emv = comp6771::euclidean_vector(std::move(ev));
	CHECK(emv.dimensions() == 3);
	CHECK(emv[0] == 5);
	CHECK(emv[0] == emv[emv.dimensions() - 1]);
	// CHECK(ev.dimensions() == 0); //not allowed, as already moved
}

TEST_CASE("Initialiser list constructor tests") {
	SECTION("Verify equality of objects made with main constructor and initialiser list") {
		auto ev1 = comp6771::euclidean_vector{3, 3, 3, 3, 3};
		auto ev2 = comp6771::euclidean_vector(5, 3);
		CHECK(ev1 == ev2);
	}
	SECTION("Verify size and elements") {
		auto ev1 = comp6771::euclidean_vector{1.1, 2.2, 3.3};
		CHECK(ev1.dimensions() == 3);
		// CHECK(ev1[2] - ev1[1] == 1.1); // fails with expansion 1.1 == 1.1 (floating point accuracy
		// issue)
		auto difference = ev1[2] - ev1[1];
		CHECK(difference > 1.09);
		CHECK(difference < 1.11); // Catch doesn't allow use of && to make a single expression
	}
}

TEST_CASE("Copy assignment tests") {
	SECTION("Basic constructor constructed") {
		auto ev = comp6771::euclidean_vector(3, 5.0);
		comp6771::euclidean_vector ev2; // shouldn't construct this, else won't be pure assignment
		                                // below
		ev2 = ev;
		CHECK(ev2 == ev);
	}

	SECTION("Initialiser list constructed") {
		auto ev = comp6771::euclidean_vector{1.1, 3.0, 1.2};
		comp6771::euclidean_vector ev2;
		ev2 = ev;
		CHECK(ev2 == ev);
	}

	SECTION("Zero dimensions") {
		auto ev = comp6771::euclidean_vector(0);
		comp6771::euclidean_vector ev2;
		ev2 = ev;
		CHECK(ev2 == ev);
		CHECK(ev2.dimensions() == 0);
	}
	// self assignment case difficult to test, requires turning off some clang/catch checks (Piazza
	// @491)
}

TEST_CASE("Move assignment tests") {
	SECTION("Initializer list constructed") {
		auto ev = comp6771::euclidean_vector{4.1, 3.0, 4.2};
		comp6771::euclidean_vector evm; // shouldn't construct this, else won't be assignment below
		evm = std::move(ev);
		CHECK(evm.dimensions() == 3);
		// CHECK(evm[2] - evm[0] == 0.1); //failed with expansion 0.1 == 0.1 (floating point
		// inaccuracy)
		auto difference = evm[2] - evm[0];
		CHECK(difference > 0.09);
		CHECK(difference < 0.11); // Catch doesn't allow use of && to make a single expression
		// can't check if ev's dimensions have been reset to 0, since access to it is not allowed
	}

	SECTION("Basic constructor constructed") {
		auto ev = comp6771::euclidean_vector(3, 5.0);
		comp6771::euclidean_vector ev2;
		ev2 = std::move(ev);
		CHECK(ev2.dimensions() == 3);
		CHECK(ev2.at(0 == 5.0));
	}

	SECTION("Zero dimensions") {
		auto ev = comp6771::euclidean_vector(0);
		comp6771::euclidean_vector ev2;
		ev2 = std::move(ev);
		CHECK(ev2.dimensions() == 0);
	}
}
