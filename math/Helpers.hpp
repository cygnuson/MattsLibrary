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

}
}