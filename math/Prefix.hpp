#pragma once

#include <string>

namespace cg {
namespace math {

/**The types of units.*/
enum Unit
{
	/**A scalat type with no unit.*/
	Scalar = 0,
	/**The unit for distance.*/
	Meter,
	/**The unit for mass.*/
	Gram,
	/**The unit for time.*/
	Second,
	/**The unit for current.*/
	Ampere,
	/**The unit for temprature*/
	Kelvin,
	/**The unit for amount of substance*/
	Mole,
};

/**The types of prefix.*/
enum Prefix : int8_t
{
	Yocto = -24,
	Zepto = -21,
	Atto = -18,
	Femto = -15,
	Pico = -12,
	Nano = -9,
	Micro = -6,
	Milli = -3,
	Centi = -2,
	Deci = -1,
	None = 0,
	Deca = 1,
	Hecto = 2,
	Kilo = 3,
	Mega = 6,
	Giga = 9,
	Tera = 12,
	Peta = 15,
	Exa = 18,
	Zetta = 21,
	Yotta = 24,
};

/**Convert a prefix into a name.
\param pref The prefi.
return A string that is the name of the prefix.*/
inline std::string ToString(Prefix pref)
{
	switch (pref)
	{
	case cg::math::Yocto:
		return "Yocto";
	case cg::math::Zepto:
		return "Zepto";
	case cg::math::Atto:
		return "Atto";
	case cg::math::Femto:
		return "Femto";
	case cg::math::Pico:
		return "Pico";
	case cg::math::Nano:
		return "Nano";
	case cg::math::Micro:
		return "Micro";
	case cg::math::Milli:
		return "Milli";
	case cg::math::Centi:
		return "Centi";
	case cg::math::Deci:
		return "Deci";
	case cg::math::Deca:
		return "Deca";
	case cg::math::Hecto:
		return "Hecto";
	case cg::math::Kilo:
		return "Kilo";
	case cg::math::Mega:
		return "Mega";
	case cg::math::Giga:
		return "Giga";
	case cg::math::Tera:
		return "Tera";
	case cg::math::Peta:
		return "Peta";
	case cg::math::Exa:
		return "Exa";
	case cg::math::Zetta:
		return "Zetta";
	case cg::math::Yotta:
		return "Yotta";
	default:
		return "";
	}
}
/**Convert a regular number to another prefix.*/
template<typename NumType>
inline NumType PrefixCast(Prefix fromPref,
	Prefix toPref,
	NumType num)
{
	if (fromPref == toPref)
		return num;

	int factor_prefix = (int)(toPref - fromPref);
	bool factorUp = factor_prefix < 0;
	uintmax_t factor;
	while (factor_prefix != 0)
	{
		if (factor_prefix < 0)
		{
			num /= 10;
			++factor_prefix;
		}
		else
		{
			num *= 10;
			--factor_prefix;
		}
	}
	return num;
}

}
}