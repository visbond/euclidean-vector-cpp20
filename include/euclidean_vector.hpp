#ifndef COMP6771_EUCLIDEAN_VECTOR_HPP
#define COMP6771_EUCLIDEAN_VECTOR_HPP

#include <compare>
#include <cstddef>
#include <functional>
#include <list>
#include <memory>
#include <ostream>
#include <range/v3/algorithm.hpp>
#include <range/v3/iterator.hpp>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace comp6771 {
	class euclidean_vector_error : public std::runtime_error {
	public:
		explicit euclidean_vector_error(std::string const& what) noexcept
		: std::runtime_error(what) {}
	};

	class euclidean_vector {
	public:
		// constructors
		euclidean_vector() ;
		explicit euclidean_vector(int); // explicit only in function declaration
		euclidean_vector(int, double); // const arguments only in fn definition
		euclidean_vector(std::vector<double>::const_iterator,
		                 std::vector<double>::const_iterator);
		euclidean_vector(std::initializer_list<double>) noexcept;
		euclidean_vector(euclidean_vector const&) noexcept; // copy constructor
		euclidean_vector(euclidean_vector&&) noexcept; // move constructor

		~euclidean_vector() noexcept = default; // destructor, explicitly declared as default (spec)

		auto operator=(euclidean_vector const&) -> euclidean_vector&; // copy assignment
		auto operator=(euclidean_vector&&) noexcept -> euclidean_vector&; // move assignment
		auto operator[](int) const -> double; // to read value
		auto operator[](int) -> double&; // to set value

		auto operator+() const -> euclidean_vector; // Unary plus (just returns a copy of object)
		auto operator-() const -> euclidean_vector; // Returns a (negated) copy, so is const
		auto operator+=(euclidean_vector const&) -> euclidean_vector&;
		auto operator-=(euclidean_vector const&) -> euclidean_vector&;
		auto operator*=(double) -> euclidean_vector&;
		auto operator/=(double) -> euclidean_vector&;

		// type conversions
		explicit operator std::vector<double>() const noexcept;
		explicit operator std::list<double>() const noexcept;

		// Member functions

		[[nodiscard]] auto at(int) const -> double;
		auto at(int) -> double&;
		[[nodiscard]] auto dimensions() const noexcept -> int;

		// Friend functions

		friend auto operator==(euclidean_vector const&, euclidean_vector const&) -> bool;
		friend auto operator!=(euclidean_vector const&, euclidean_vector const&) -> bool;
		friend auto operator+(euclidean_vector const&, euclidean_vector const&) -> euclidean_vector;
		friend auto operator-(euclidean_vector const&, euclidean_vector const&) -> euclidean_vector;

		// scalar multiplication is commutative, so should have two functions
		friend auto operator*(euclidean_vector const&, double) -> euclidean_vector;
		friend auto operator*(double, euclidean_vector const&) -> euclidean_vector;

		friend auto operator/(euclidean_vector const&, double) -> euclidean_vector;
		friend auto operator<<(std::ostream&, euclidean_vector const&) -> std::ostream&;

	private:
		// ass2 spec requires we use pointers to double[] instead of std::vector
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		std::unique_ptr<double[]> magnitudes_;

		// size_t is the default value for size types. But spec requires dimensions to be passed as
		// int in constructors, using casts as required. dimensions() also returns int. Declaring
		// dimensions_ as size_t, since this is the standard, and making it int needs many more casts
		// in the code
		std::size_t dimensions_;

		// tested that norm algorithm is efficient and calculates norm of million element vector in a
		// few milliseconds, so not using norm caching, as it was adding unnecessary complexity and
		// potential for unknown bugs (without knowing enough test cases)
	};

	// Utility functions

	auto euclidean_norm(euclidean_vector const& v) -> double;
	auto unit(euclidean_vector const&) -> euclidean_vector;
	auto dot(euclidean_vector const&, euclidean_vector const&) -> double;

} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
