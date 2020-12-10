// Euclidean vector implementation in C++ 20
// Assignment requirement for Advanced C++ course in Masters in IT at university.
// Refer Readme.md for context and exact assignment specification.

// Original Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// Original license notice retained as per Apache 2 requirements. Christopher Di Bella is
// the name of one of the course instructors. The file originally contained just a few header
// declarations and an empty namespace, in both this cpp and the header hpp files.
// The class methods and declarations in both the files have been coded by me.
// (Apache 2.0 license requires changes to the original work to be prominently declared.)
//
// Class methods code Copyright (c) Vishal Bondwal, Apache 2.0 license

#include "euclidean_vector.hpp"
#include <cstddef>
#include <gsl/gsl-lite.hpp>
#include <iostream>
#include <numeric>
#include <range/v3/functional.hpp>
#include <string>

namespace comp6771 {

	// constructors

	// main constructor, others delegate it, so defining this first (for convenience of reader's
	// understanding, not a C++ or spec requirement)
	euclidean_vector::euclidean_vector(const int dimensions, const double magnitude) {
		// spec states that dimensions would never be negative, but too risky to let it in, so
		// asserting
		assert(dimensions >= 0);
		dimensions_ = gsl_lite::narrow_cast<std::size_t>(dimensions); // losing signedness
		                                                              // information, so lossy cast
		// ass2 spec requires we use pointers to double[] instead of std::vector
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		magnitudes_ = std::make_unique<double[]>(dimensions_);
		auto magnitude_span = std::span<double>(magnitudes_.get(), dimensions_);
		ranges::fill(magnitude_span, magnitude);
	}

	// explicit dimension-based constructor (specified explicit in header file)
	euclidean_vector::euclidean_vector(const int dim)
	: euclidean_vector(dim, 0.0) {} // delegates the previous constructor

	// default constructor
	euclidean_vector::euclidean_vector()
	: euclidean_vector(1) {} // delegates the previous constructor (which calls the one before it)

	// vector iterator based constructor
	euclidean_vector::euclidean_vector(std::vector<double>::const_iterator begin,
	                                   std::vector<double>::const_iterator end)
	: euclidean_vector(gsl_lite::narrow_cast<int>(ranges::distance(begin, end))) {
		// due to the delegated constructor call above, our object is already made and filled with
		// 0s, just need to copy passed vector into it

		// std::copy() below can be made to work directly with magnitudes_ as well, but avoiding
		// direct pointer access. Span is safer.
		auto magnitude_span = std::span<double>(magnitudes_.get(), dimensions_);
		std::copy(begin, end, magnitude_span.begin());
	}

	// initialiser list constructor
	euclidean_vector::euclidean_vector(std::initializer_list<double> input_list) noexcept
	: euclidean_vector(gsl_lite::narrow_cast<int>(input_list.size())) {
		dimensions_ = input_list.size();
		auto magnitude_span = std::span<double>(magnitudes_.get(), dimensions_);
		std::copy(input_list.begin(), input_list.end(), magnitude_span.begin());
	}

	// copy constructor
	euclidean_vector::euclidean_vector(euclidean_vector const& input_evector) noexcept
	: euclidean_vector(gsl_lite::narrow_cast<int>(input_evector.dimensions_)) {
		// turn both input object and this object into spans, and copy. Safer than handling pointers
		auto passed_object_span =
		   std::span<double>(input_evector.magnitudes_.get(), input_evector.dimensions_);
		auto this_object_span = std::span<double>(magnitudes_.get(), dimensions_);
		std::copy(passed_object_span.begin(), passed_object_span.end(), this_object_span.begin());
	}

	// move constructor
	euclidean_vector::euclidean_vector(euclidean_vector&& input_evector) noexcept {
		magnitudes_ = std::move(input_evector.magnitudes_); // moves all values in locations
		                                                    // associated with unique pointer, thus
		                                                    // automatically invalidating them
		dimensions_ = input_evector.dimensions_;
		input_evector.dimensions_ = 0;
		// our input vector is in an "unspecified" state now
	}

	// destructor explicitly declared as default in header file already

	// assignment operators

	// copy assignment
	auto euclidean_vector::operator=(euclidean_vector const& input_evector) -> euclidean_vector& {
		if (this != &input_evector) { // this line handles self-assignment
			                           // cases (a = a;)
			dimensions_ = input_evector.dimensions_;
			// ass2 spec requires we use pointers to double[] instead of std::vector
			// NOLINTNEXTLINE(modernize-avoid-c-arrays)
			magnitudes_ = std::make_unique<double[]>(dimensions_);

			// get spans on both objects and copy
			auto passed_object_span =
			   std::span<double>(input_evector.magnitudes_.get(), input_evector.dimensions_);
			auto this_object_span = std::span<double>(magnitudes_.get(), dimensions_);
			std::copy(passed_object_span.begin(), passed_object_span.end(), this_object_span.begin());
		}

		return *this;
	} // end copy assignment

	// move assignment
	auto euclidean_vector::operator=(euclidean_vector&& input_evector) noexcept -> euclidean_vector& {
		magnitudes_ = std::move(input_evector.magnitudes_); // new object 'takes over' old object
		                                                    // pointer, which goes in unspecified
		                                                    // state
		dimensions_ = input_evector.dimensions_;
		input_evector.dimensions_ = 0;
		return *this;
	}

	auto euclidean_vector::operator[](const int index) const -> double {
		assert(index >= 0 && index < gsl_lite::narrow_cast<int>(dimensions_)); // spec asks to assert
		                                                                       // check. C++ allows
		                                                                       // negative indexes in
		                                                                       // raw arrays, but not
		                                                                       // implementing that,
		                                                                       // as too much risk
		                                                                       // without knowing what
		                                                                       // test cases would be
		                                                                       // used.
		return magnitudes_[gsl_lite::narrow_cast<std::size_t>(index)]; // size_t to int is narrow cast as
		                                                          // lose as much range (size_t is
		                                                          // unsigned long in most
		                                                          // implementations.) int to size_t
		                                                          // is narrow as lose sign
		                                                          // information.
	}

//V: note following is not const method as returns reference (presumably used to change value). Unlike the one above, which is a getter.
	auto euclidean_vector::operator[](const int index) -> double& {
		assert(index >= 0 && index < gsl_lite::narrow_cast<int>(dimensions_));
		return magnitudes_[gsl_lite::narrow_cast<std::size_t>(index)];
	}

	// unary operator overloads

	auto euclidean_vector::operator+() const -> euclidean_vector { // returns a copy, so is const
		auto ev = euclidean_vector(*this); // construct and return copy of vector
		return ev;
	}

	auto euclidean_vector::operator-() const -> euclidean_vector { // returns a copy, so is const
		auto result = euclidean_vector(*this); // initialize result vector with this object
		auto result_span = std::span<double>(result.magnitudes_.get(), result.dimensions_);

//V: the following is one overload of transform(), there are more, one is in the next function. This one uses begin & end iterators for source range, and begin iterator for destination.
		ranges::transform(result_span.begin(), // source
		                  result_span.end(),
		                  result_span.begin(), // destination
		                  [](auto& c) { return c * (-1); });
		return result;
	}

	// compound mathematical operator overloads

	// class methods can access private variables of other class objects, so using them directly
	// instead of via getters, for efficiency

	auto euclidean_vector::operator+=(euclidean_vector const& rhs) -> euclidean_vector& {
		if (dimensions_ != rhs.dimensions_) {
			auto except_string = "Dimensions of LHS(" + std::to_string(dimensions_) + ") and RHS("
			                     + std::to_string(rhs.dimensions_) + ") do not match";
			throw euclidean_vector_error(except_string);
		}

		// get spans on this object(acts as accumulator) and the other source vector
		auto sum_span = std::span<double>(magnitudes_.get(), dimensions_);
		auto rhs_span = std::span<double>(rhs.magnitudes_.get(), rhs.dimensions_);
		// and add them into sum (which is a span over this object)

//V: (ref transform() comment in previous function) this overload takes the two ranges to accumulate, then the destination, and then the numeric function
		ranges::transform(sum_span, rhs_span, sum_span.begin(), ranges::plus{});
		return *this;
	}

	auto euclidean_vector::operator-=(euclidean_vector const& rhs) -> euclidean_vector& {
		if (dimensions_ != rhs.dimensions_) {
			auto except_string = "Dimensions of LHS(" + std::to_string(dimensions_) + ") and RHS("
			                     + std::to_string(rhs.dimensions_) + ") do not match";
			throw euclidean_vector_error(except_string);
		}

		// get spans on this result vector and the other source vector
		auto diff_span = std::span<double>(magnitudes_.get(), dimensions_);
		auto rhs_span = std::span<double>(rhs.magnitudes_.get(), rhs.dimensions_);
		// and subtract them into diff (this vector)
		ranges::transform(diff_span, rhs_span, diff_span.begin(), ranges::minus{});
		return *this;
	}

	// Compound multiplication.
	// Scalar can only be to the right of the vector with this syntax.
	// Friend functions handle commutative syntax cases
	auto euclidean_vector::operator*=(double const scalar) -> euclidean_vector& {
		// get span on this object, where product would be stored
		auto product_span = std::span<double>(magnitudes_.get(), dimensions_);
		ranges::transform(product_span.begin(), // source
		                  product_span.end(),
		                  product_span.begin(), // destination
		                  [&scalar](auto& c) { return c * scalar; }); // ranges::multiplies works with
		                                                              // vectors/ranges, can be made
		                                                              // to work with scalars but
		                                                              // complicated
		return *this;
	}

	auto euclidean_vector::operator/=(double const scalar) -> euclidean_vector& {
		if (scalar == 0) {
			throw("Invalid vector division by 0");
		}
		// get span on this object, where quotient would be stored
		auto quotient_span = std::span<double>(magnitudes_.get(), dimensions_);
		ranges::transform(quotient_span.begin(), // source
		                  quotient_span.end(),
		                  quotient_span.begin(), // destination
		                  [&scalar](auto& c) { return c / scalar; });
		return *this;
	}

	// type conversion functions

	euclidean_vector::operator std::vector<double>() const noexcept {
		auto copy_vector = std::vector<double>(dimensions_);
		// get a span on our own values and copy into above variable
		auto magnitude_span = std::span<double>(magnitudes_.get(), dimensions_);
		std::copy(magnitude_span.begin(), magnitude_span.end(), copy_vector.begin());
		return copy_vector;
	}

	euclidean_vector::operator std::list<double>() const noexcept {
		auto copy_list = std::list<double>(dimensions_);
		// get a span on our own values and copy into above variable
		auto magnitude_span = std::span<double>(magnitudes_.get(), dimensions_);
		std::copy(magnitude_span.begin(), magnitude_span.end(), copy_list.begin());
		return copy_list;
	}

	// other class methods

	[[nodiscard]] auto euclidean_vector::at(int index) const -> double {
		if (index < 0 or index >= gsl_lite::narrow_cast<int>(dimensions_))
		{ // losing precision / numerical range (long to int), so is narrow cast
			auto except_string =
			   "Index " + std::to_string(index) + " is not valid for this euclidean_vector object";
			throw euclidean_vector_error(except_string);
		}

		return magnitudes_[gsl_lite::narrow_cast<std::size_t>(index)]; // losing sign information, so is
		                                                          // narrow cast
	}

	auto euclidean_vector::at(int index) -> double& {
		if (index < 0 or index >= gsl_lite::narrow_cast<int>(dimensions_)) {
			auto except_string =
			   "Index " + std::to_string(index) + " is not valid for this euclidean_vector object";
			throw euclidean_vector_error(except_string);
		}

		return magnitudes_[gsl_lite::narrow_cast<std::size_t>(index)];
	}

	[[nodiscard]] auto euclidean_vector::dimensions() const noexcept -> int {
		return gsl_lite::narrow_cast<int>(dimensions_);
	}

	// friend function operator overloads

	// these are given to be friend functions in spec, so directly accessing private variables for
	// efficiency, rather than use getter function calls

	// binary == operator overload
	auto operator==(euclidean_vector const& lhs, euclidean_vector const& rhs) -> bool {
		if (lhs.dimensions_ != rhs.dimensions_) {
			return false;
		}
		auto lhs_span = std::span<double>(lhs.magnitudes_.get(), lhs.dimensions_);
		auto rhs_span = std::span<double>(rhs.magnitudes_.get(), rhs.dimensions_);

		bool result = ranges::equal(lhs_span, rhs_span);
		return result;
	}

	auto operator!=(euclidean_vector const& lhs, euclidean_vector const& rhs) -> bool {
		return !(lhs == rhs);
	}

	// binary + overload. Note that this is defined as a friend function, so accessing inner values
	// directly rather than using getter functions, for higher efficiency
	auto operator+(euclidean_vector const& lhs, euclidean_vector const& rhs) -> euclidean_vector {
		if (lhs.dimensions_ != rhs.dimensions_) {
			auto except_string = "Dimensions of LHS(" + std::to_string(lhs.dimensions_) + ") and RHS("
			                     + std::to_string(rhs.dimensions_) + ") do not match";

			throw euclidean_vector_error(except_string);
		}

		auto sum = euclidean_vector(lhs); // initialize result vector with one source vector
		// get spans on this accumulator and the other source vector
		auto sum_span = std::span<double>(sum.magnitudes_.get(), sum.dimensions_);
		auto rhs_span = std::span<double>(rhs.magnitudes_.get(), rhs.dimensions_);
		// and add them into sum
		ranges::transform(sum_span, rhs_span, sum_span.begin(), ranges::plus{});
		return sum;
	}

	// binary - overload
	auto operator-(euclidean_vector const& lhs, euclidean_vector const& rhs) -> euclidean_vector {
		if (lhs.dimensions_ != rhs.dimensions_) {
			auto except_string = "Dimensions of LHS(" + std::to_string(lhs.dimensions_) + ") and RHS("
			                     + std::to_string(rhs.dimensions_) + ") do not match";

			throw euclidean_vector_error(except_string);
		}

		auto diff_vector = euclidean_vector(lhs); // initialize result vector with one source vector
		// get spans on this result vector and the other source vector
		auto diff_span = std::span<double>(diff_vector.magnitudes_.get(), diff_vector.dimensions_);
		auto rhs_span = std::span<double>(rhs.magnitudes_.get(), rhs.dimensions_);
		// and subtract them into diff
		ranges::transform(diff_span, rhs_span, diff_span.begin(), ranges::minus{});
		return diff_vector;
	}

	// scalar multiplication, first form (vector * scalar)
	// scalar is not passed by reference because it would cause problems if it's an rvalue
	auto operator*(euclidean_vector const& ev, double const scalar) -> euclidean_vector {
		auto product_vector = euclidean_vector(ev); // initialize result vector
		auto product_span =
		   std::span<double>(product_vector.magnitudes_.get(), product_vector.dimensions_);
		ranges::transform(product_span.begin(), // source
		                  product_span.end(),
		                  product_span.begin(), // destination
		                  [&scalar](auto& c) { return c * scalar; });
		return product_vector;
	}

	// scalar multiplication, second form (scalar * vector). Scalar multiplication is commutative.
	auto operator*(double const scalar, euclidean_vector const& ev) -> euclidean_vector {
		return ev * scalar; // call the commutative function
	}

	// scalar division
	// scalar is not passed by reference because it would cause problems if it's an rvalue
	auto operator/(euclidean_vector const& ev, double const scalar) -> euclidean_vector {
		if (scalar == 0) {
			throw("Invalid vector division by 0");
		}
		auto quotient = euclidean_vector(ev); // initialize result vector
		auto quotient_span = std::span<double>(quotient.magnitudes_.get(), quotient.dimensions_);
		ranges::transform(quotient_span.begin(), // source
		                  quotient_span.end(),
		                  quotient_span.begin(), // destination
		                  [&scalar](auto& c) { return c / scalar; });
		return quotient;
	}

	// ostream << overload, will display vector (1,2,3) as [1 2 3]
	// note Chris clarified in a comment that empty vectors should be displayed as [] rather than [ ]
	// (i.e. no space in between)
	auto operator<<(std::ostream& os, euclidean_vector const& ev) -> std::ostream& {
		auto magnitude_span = std::span<double>(ev.magnitudes_.get(), ev.dimensions_);
		os << '['; // to follow expected format

		if (ev.dimensions_ > 0) { // else risk buffer-overflow error
			std::for_each_n(magnitude_span.begin(), ev.dimensions_ - 1, [&](auto& v) {
				os << v << ' ';
			}); // needed lambda capture defaults, as capturing os alone gave errors with <<
			auto lastval = *(magnitude_span.end() - 1); // can embed below, but keeping separate
			                                            // statement for clarity
			os << lastval;
		}
		os << ']';
		return os;
	}

	// utility functions

	// starting with dot() because it is used in norm calculation, so is helpful to understand first
	// how it works

	//  dot product of x and y e.g., [1 2] . [3 4] = 1 * 3 + 2 * 4 = 11
	//  Note in spec: We will not be testing the case of multiplying two 0-dimension vectors
	//  together. (still putting assert for that, for safety)
	auto dot(euclidean_vector const& lhs, euclidean_vector const& rhs) -> double {
		if (lhs.dimensions() != rhs.dimensions()) {
			auto except_string = "Dimensions of LHS(" + std::to_string(lhs.dimensions()) + ") and RHS("
			                     + std::to_string(rhs.dimensions()) + ") do not match";

			throw euclidean_vector_error(except_string);
		}
		assert(lhs.dimensions() > 0);

		// converting both euclidean vectors to std vectors, as much faster than accessing with at()
		// or [] again and again (note that this is not a friend fn, so can't access directly)
		auto xvector = std::vector<double>(lhs);
		auto yvector = std::vector<double>(rhs);
		auto result = 0.0;
		result = std::inner_product(xvector.begin(), xvector.end(), yvector.begin(), 0.0);

		return result;
	}

	// Returns the Euclidean norm of the vector as a double. The Euclidean norm is the
	// square root of the sum of the squares of the magnitudes in each dimension. E.g, for the vector
	// [1 2 3] the Euclidean norm is sqrt(1*1 + 2*2 + 3*3) = 3.74.
	auto euclidean_norm(euclidean_vector const& v) -> double {
		if (v.dimensions() == 0) {
			throw euclidean_vector_error("euclidean_vector with no dimensions does not have a norm");
		}

		// note that squaring of each value means multiplying a vector with itself (dot product)
		auto sqnorm = dot(v, v); // square of the norm
		return sqrt(sqnorm);
	}

	// Returns a Euclidean vector that is the unit vector of v. The magnitude for each
	// dimension in the unit vector is the original vector's magnitude divided by the Euclidean norm.
	auto unit(euclidean_vector const& v) -> euclidean_vector {
		if (v.dimensions() == 0) {
			throw euclidean_vector_error("euclidean_vector with no dimensions does not have a unit "
			                             "vector");
		}

		auto norm = euclidean_norm(v);

		if (norm == 0) {
			throw euclidean_vector_error("euclidean_vector with zero euclidean normal does not have a "
			                             "unit vector");
		}

		// convert euclidean to std vector, as is much faster to access, than to access elements again
		// and again with [] or at()
		auto vr = std::vector<double>(v);
		ranges::transform(vr.begin(), // source
		                  vr.end(),
		                  vr.begin(), // destination
		                  [&norm](auto& c) { return c / norm; });

		// now construct a euclidean vector with this std::vector, and return
		return (euclidean_vector(vr.begin(), vr.end()));
	}

} // namespace comp6771
