#pragma once

#include "../LogAdaptor.hpp"

namespace cg {
namespace math {

/**A unit type.*/
class Unit
{
public:
	
private:

};

/**Keep track of measurements*/
template<typename NumType>
class Measurement
{
public:
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
	/**Create a measurement.
	\param num The number for the value of the measurement.
	\param unit The unit for the measurement.*/
	Measurement(const NumType& num = 0, Unit unit = Scalar);
private:
	/**The unit for this measurement.*/
	 m_unit;
	/**The value of the measurement.*/
	NumType m_val;
};

template<typename NumType>
inline Measurement<NumType>::Measurement(const NumType & num, Unit unit)
{

}

}
}