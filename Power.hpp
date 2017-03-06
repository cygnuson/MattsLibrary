#pragma once

#include <cstdint>

namespace cg {

const std::uintmax_t MaxPowerOf2 = ((sizeof(std::uintmax_t) * 8) - 1);
 
/**Compiletime power operator.
*
*Get the power at compile time.
*
*\tparam num The number that is the base of the operation.
*\tparam power The power in which to raise the base.
*/
template<std::uintmax_t num, std::uintmax_t power>
class Power {
private:
	/**\sa Power*/
	template<std::uintmax_t num, std::uintmax_t power>
	class Pow {
	public:
		static const std::uintmax_t value =num* Pow<num, power - 1>::value;
	};
	/**\sa Power*/
	template<std::uintmax_t num>
	class Pow<num, 0> {
	public:
		static const std::uintmax_t value = 1;
	};
public:
	static const std::uintmax_t value = num* Pow<num, power - 1>::value;
};

template<std::uintmax_t num>
class Power<num, 0>
{
public:
	static const std::uintmax_t value = 1;
};

template<std::uintmax_t N, std::uintmax_t I = 0>
class IsPowerOf2
{
public:
	const static std::uintmax_t Power = cg::Power<2, I>::value;
	const static bool value = (N == Power) ?
		true :
		IsPowerOf2<N, I + 1>::value;
};

template<std::uintmax_t N>
class IsPowerOf2<N, ((sizeof(std::size_t) * 8) - 1)>
{
public:
	const static bool value =
		N == cg::Power<2, MaxPowerOf2>::value;
};

}