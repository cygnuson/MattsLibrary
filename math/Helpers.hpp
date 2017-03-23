#pragma once

#include <utility>

namespace cg {
namespace math {

/*Neither may be zero.

Euclidean algorithm to find the GCD.
https://en.wikipedia.org/wiki/Euclidean_algorithm#Background:_greatest_common_divisor
\param first The first number.
\param second The second number.
*/
template<typename T>
T GCD(const T& first, const T& second)
{
	auto larger = first > second ? first : second;
	auto lesser = first == larger ? second : first;
	T remainder = 1;
	while (remainder != 0)
	{
		remainder = larger % lesser;
		if (remainder == 0)
			return lesser;
		larger = lesser;
		lesser = remainder;
	}
	return 1;
}

/**Get the least common multiple of a pair of numbers.
\param first The first number.
\param second The second number.
\return The number that is the least common multiple.*/
template<typename T>
T LCM(const T& first, const T& second)
{
	auto div = cg::math::GCD<T>(first, second);
	return (first / div) * second;
}


}
}